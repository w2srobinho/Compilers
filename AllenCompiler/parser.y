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
  char *types;
  AST::Node *node;
  AST::Block *block;
  const char *name;
}

/* token defines our terminal symbols (tokens).
 */
%token <types> T_INT T_REAL T_BOOL
%token T_PLUS T_MINUS T_TIMES T_DIVISION
%token T_EQUALS T_DIFFER T_GREATER T_LESS T_GREATER_OR_EQUAL T_LESS_OR_EQUAL
%token T_AND T_OR T_NOT T_OPEN_PARENTS T_CLOSE_PARENTS
%token T_ASSIGN T_END_LINE T_COMMA T_COLON T_NL
%token <name> T_TYPE_INT T_TYPE_REAL T_TYPE_BOOL T_ID

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <name> vartype
%type <node> expr line varlist
%type <block> lines program

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
            | expr T_END_LINE T_NL /*$$ = $1 when nothing is said*/
            | vartype varlist T_END_LINE T_NL { symtab.updateSymbolTable($1);
                                                $$ = $2; }
            | T_ID T_ASSIGN expr T_END_LINE { AST::Node* node = symtab.assignVariable($1);
                                              $$ = new AST::BinOp(node, AST::assign, $3); }
            ;

            /*$$ = $1 when nothing is said*/
vartype     : T_TYPE_INT  { }
            | T_TYPE_REAL { }
            | T_TYPE_BOOL { }
            ;

expr        : T_INT  { $$ = new AST::Value($1, AST::integer); }
            | T_REAL { $$ = new AST::Value($1, AST::real); }
            | T_BOOL { $$ = new AST::Value($1, AST::boolean); }
            | T_ID { $$ = symtab.useVariable($1); }
            | expr T_PLUS expr { $$ = new AST::BinOp($1,AST::plus,$3); }
            | expr T_MINUS expr { $$ = new AST::BinOp($1,AST::minus,$3); }
            | expr T_DIVISION expr { $$ = new AST::BinOp($1,AST::division,$3); }
            | expr T_TIMES expr { $$ = new AST::BinOp($1,AST::times,$3); }
            | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
            ;

varlist     : T_ID { $$ = symtab.newVariable($1, NULL); }
            | varlist T_COMMA T_ID { $$ = symtab.newVariable($3, $1); }
            ;

%%
