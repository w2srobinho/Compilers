%{
  #include "ast.h"
  #include "st.h"
  #include <iostream>
  #include <string>
  ST::SymbolTable symtab;  /* main symbol table */
  AST::Block *programRoot; /* the root node of our program AST::*/
  extern int yylex();
  extern void yyerror(const char* s, ...);
%}

%define parse.trace

 /*yylval == %union
 * union informs the different ways we can store data
 */
%union {
  AST::NodeList *nodelist;
  AST::Node *node;
  AST::Block *block;
  const char *name;
  int int_val;
}

/* token defines our terminal symbols (tokens).
 */
%token <name> T_INT T_REAL T_BOOL
%token <int_val> ARRAY_SIZE
%token T_PLUS T_MINUS T_TIMES T_DIVISION
%token T_EQUALS T_DIFFER T_GREATER T_LESS T_GREATER_OR_EQUAL T_LESS_OR_EQUAL
%token T_AND T_OR T_NOT T_OPEN_PARENTHESES T_CLOSE_PARENTHESES T_OPEN_BRACKETS T_CLOSE_BRACKETS
%token T_ASSIGN T_END_LINE T_COMMA T_COLON T_NL
%token <name> T_TYPE_INT T_TYPE_REAL T_TYPE_BOOL T_ID

/* type defines the type of our nonterminal symbols.''
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <name> vartype
%type <node> expr line declaration assignment value arraytype
%type <block> lines program
%type <nodelist> varlist

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 A precedência dos operadores para valores inteiros e de ponto flutuante é a padrão.
 A precedência para expressões booleanas é:
 [maior] operadores relacionais, operador unário, operadores binários.
 */
%left T_PLUS T_MINUS
%left T_TIMES T_DIVISION
%left T_CLOSE_PARENTHESES
%right T_OPEN_PARENTHESES
%nonassoc UnMinus_prec
%nonassoc error

/* Starting rule
 */
%start program

%%

program     : lines { programRoot = $1; }
            ;

lines       : line { $$ = new AST::Block(); $$->lines.push_back($1); }
            | lines line { if($2 != NULL) $1->lines.push_back($2); }
            ;

line        : T_NL { $$ = NULL; } /*nothing here to be used */
            | declaration T_END_LINE
            | assignment T_END_LINE
            | line error { yyerrok; $$ = $1; } /*just a point for error recovery*/
            ;

declaration : arraytype T_COLON varlist { $$ = $1; };
            | vartype T_COLON varlist { $$ = new AST::VarDeclaration($1, $3);
                                        symtab.updateSymbolTable($1); }
            ;

            /*$$ = $1 when nothing is said */
vartype     : T_TYPE_INT
            | T_TYPE_REAL
            | T_TYPE_BOOL
            ;

arraytype   : vartype T_OPEN_BRACKETS ARRAY_SIZE T_CLOSE_BRACKETS { symtab.updateSymbolTable($1);
                                                                    $$ = new AST::ArrayDeclaration($1, $3); }
            ;

varlist     : T_ID { $$ = new AST::NodeList;
                     $$->push_back(symtab.newVariable($1)); }
            | varlist T_COMMA T_ID { $$ = $1;
                                     $$->push_back(symtab.newVariable($3)); }
            ;

assignment  : T_ID T_ASSIGN expr { AST::Node* node = symtab.assignVariable($1);
                                   $$ = new AST::BinOp(node, AST::assign, $3); }
            ;

expr        : value
            | T_OPEN_PARENTHESES expr T_CLOSE_PARENTHESES { $$ = new AST::UnOp(AST::par, $2); }
            | T_MINUS expr %prec UnMinus_prec { $$ = new AST::UnOp(AST::unMinus, $2); }
            | T_NOT expr { $$ = new AST::UnOp(AST::not_op, $2); }
            | expr T_AND expr { $$ = new AST::BinOp($1, AST::and_op, $3); }
            | expr T_OR expr { $$ = new AST::BinOp($1, AST::or_op, $3); }
            | expr T_PLUS expr { $$ = new AST::BinOp($1, AST::plus, $3); }
            | expr T_MINUS expr { $$ = new AST::BinOp($1, AST::minus, $3); }
            | expr T_DIVISION expr { $$ = new AST::BinOp($1, AST::division, $3); }
            | expr T_TIMES expr { $$ = new AST::BinOp($1, AST::times, $3); }
            | expr T_EQUALS expr { $$ = new AST::BinOp($1, AST::equals, $3); }
            | expr T_DIFFER expr { $$ = new AST::BinOp($1, AST::differ, $3); }
            | expr T_GREATER expr { $$ = new AST::BinOp($1, AST::greater, $3); }
            | expr T_LESS expr { $$ = new AST::BinOp($1, AST::less, $3); }
            | expr T_GREATER_OR_EQUAL expr { $$ = new AST::BinOp($1, AST::greaterOrEqual, $3); }
            | expr T_LESS_OR_EQUAL expr { $$ = new AST::BinOp($1, AST::lessOrEqual, $3); }
            | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
            ;
            /*TODO coersão, tratamento de erros*/

value       : T_INT  { $$ = new AST::Value($1, AST::integer); }
            | T_REAL { $$ = new AST::Value($1, AST::real); }
            | T_BOOL { $$ = new AST::Value($1, AST::boolean); }
            | T_ID { $$ = symtab.useVariable($1); }
            ;


%%
