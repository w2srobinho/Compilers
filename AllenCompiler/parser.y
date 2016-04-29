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
%token T_ASSIGN T_END_LINE T_COMMA T_NL
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
%nonassoc error

/* Starting rule
 */
%start program

%%

program     : lines { std::cout << "linha 57" << std::endl;programRoot = $1; }
            ;

lines       : line { std::cout << "linha 60" << std::endl;
                     $$ = new AST::Block(); $$->lines.push_back($1); }
            | lines line { std::cout << "linha 61" << std::endl;
                           if($2 != NULL) $1->lines.push_back($2); }
            ;

line        : T_NL { std::cout << "linha 65" << std::endl;
                     $$ = NULL; } /*nothing here to be used */
            | expr T_END_LINE T_NL /*$$ = $1 when nothing is said*/
            | vartype varlist T_END_LINE T_NL { std::cout << "linha 67" << std::endl;
                                     symtab.updateSymbolTable($1);
                                     $$ = $2; }
            | T_ID T_ASSIGN expr T_END_LINE { std::cout << "linha 68" << std::endl;
                                   AST::Node* node = symtab.assignVariable($1);
                                   $$ = new AST::BinOp(node, AST::assign, $3); }
            ;

            /*$$ = $1 when nothing is said*/
vartype     : T_TYPE_INT  { std::cout << "linha 72" << std::endl; }
            | T_TYPE_REAL { std::cout << "linha 73" << std::endl; }
            | T_TYPE_BOOL { std::cout << "linha 74" << std::endl; }
            ;

expr        : T_INT  { $$ = new AST::Value($1); }
            | T_REAL { $$ = new AST::Value($1); }
            | T_BOOL { $$ = new AST::Value($1); }
            | T_ID { $$ = symtab.useVariable($1); }
            | expr T_PLUS expr { $$ = new AST::BinOp($1,AST::plus,$3); }
            | expr T_TIMES expr { $$ = new AST::BinOp($1,AST::times,$3); }
            | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
            ;

varlist     : T_ID { std::cout << "linha 88 "<< $1 << std::endl;
                     $$ = symtab.newVariable($1, NULL); }
            | varlist T_COMMA T_ID { std::cout << "linha 89" << std::endl;
                                     $$ = symtab.newVariable($3, $1); }
            ;

%%
