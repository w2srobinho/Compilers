%{
  /*
#include "ast.h"
#include "st.h"

ST::SymbolTable symtab;  /* main symbol table */
const char *programRoot; /* the root node of our program AST:: */

extern int yylex();
extern void yyerror(const char* s, ...);
%}

/*Suddenly, the union decided not to find things included before...
%code requires{
#include "support.h"
}*/

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    //AST::Node *node;        //Node of the syntax tree
    //AST::Block *block;      //List of nodes of syntax trees
    //AST::VarDeclaration *vars; //List of variables in a declaration
    const char *name;       //Names of variables and values
    //Ops::Operation comp;    //Comparison operations ( =, ~=, <, <=, >, >= )
}

/* token defines our terminal symbols (tokens).
 */
%token OP_PLUS OP_TIMES OP_DIV OP_MINUS OP_AND OP_OR OP_NOT
%token ASSIGN SEMI_COLON COMMA
%token VAR FOR FUNC
%token L_PARENT R_PARENT L_BRACES R_BRACES
/*%token <comp> COMP*/
%token <name> ID INT DOUBLE BOOL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <name> block vardeclaration funcdeclaration funcall for forscope assignment value param varlist
%type <name> blocks program

/* Operator precedence for mathematical operators
 * The later it is listed, the higher the precedence
 */
%left OP_AND OP_OR
%left OP_NOT
%nonassoc COMP
%left OP_PLUS OP_MINUS
%left OP_TIMES OP_DIV
%nonassoc OP_MIN_UN
%nonassoc error

%start program

%%

program : blocks { programRoot = $1; }
        ;

blocks  : block
        | blocks block {$$ = $2; }
        ;

block   : vardeclaration SEMI_COLON
        | funcdeclaration SEMI_COLON
        | funcall SEMI_COLON
        | assignment SEMI_COLON
        | for SEMI_COLON
        | error SEMI_COLON {yyerrok; $$ = NULL;}
        ;

/* for definition
* e.g. for (12; 2) { scope }
*/
for : FOR L_PARENT forvalue SEMI_COLON forvalue R_PARENT L_BRACES forscope R_BRACES {$$ = $8; }
    ;
forscope : vardeclaration
         | for
         | assignment
         ;
forvalue : INT
         | DOUBLE
         | ID
         ;
/* end for */

/* Variable declaration region.
 * e.g. var umnome, segundonome;
 */
vardeclaration : VAR varlist { $$ = $2; }
               ;
varlist : ID
        | varlist COMMA ID { $$ = $3; }
        ;
/* end declaration */

/* Variable assignment region.
* e.g. umnome = 1; segundonome = umnome;
*/
assignment : ID ASSIGN value { $$ = $3; }
           ;
value : INT
      | DOUBLE
      | BOOL
      | ID
      ;
/* end assignment */

/* function region
* e.g. func umafuncao(idparam1, idparam2) { }
*/
funcdeclaration : FUNC ID L_PARENT varlist R_PARENT L_BRACES block R_BRACES { $$ = $2; }
                ;
funcall : ID L_PARENT param R_PARENT
        ;
param : value
      | param COMMA value { $$ = $3; }
      ;
/*end region */
%%
