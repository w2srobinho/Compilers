%{
  #include "st.h"
  #include <iostream>
  #include <string>

  ST::SymbolTable symtab;  /* main symbol table */
  const char *programRoot; /* the root node of our program AST:: */

  extern int yylex();
  extern void yyerror(const char* s, ...);
%}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
  int counter;
  const char *name;       //Names of variables and values
}

/* token defines our terminal symbols (tokens).
 */
%token OP_PLUS OP_TIMES OP_DIV OP_MINUS OP_AND OP_OR OP_NOT COMP
%token ASSIGN SEMI_COLON COMMA
%token VAR FOR FUNC RETURN IF ELSE
%token L_PARENT R_PARENT L_BRACES R_BRACES
/*%token <comp> COMP*/
%token <name> ID INT DOUBLE BOOL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <name> block vardeclaration funcdeclaration funcall for forscope assignment
%type <name> value varlist funcscope expr if else ifstmt
%type <name> blocks program funcblock
%type <counter> args params

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
        | funcall SEMI_COLON
        | assignment SEMI_COLON
        | funcdeclaration
        | for
        | if
        | error SEMI_COLON {yyerrok; $$ = NULL;}
        ;

/* Variable declaration region.
* e.g. var umnome, segundonome;
*/
vardeclaration : VAR varlist { $$ = $2; }
;

varlist : ID { symtab.newVariable($1, ST::Kind::variable, false); }
        | varlist COMMA ID { symtab.newVariable($3, ST::Kind::variable, false);
                             $$ = $3; }
        ;
/* end declaration */

/* Variable assignment region.
* e.g. umnome = 1; segundonome = umnome;
*/
assignment : ID ASSIGN expr { symtab.assignVariable($1);
                              $$ = $3; }
           ;

/* end assignment */

/* for definition
* e.g. for (12; 2) { scope }
*/
for : FOR L_PARENT forvalue SEMI_COLON forvalue R_PARENT L_BRACES forscope R_BRACES {$$ = $8; }
    ;

forscope : vardeclaration SEMI_COLON
         | for
         | if
         | assignment SEMI_COLON
         ;

forvalue : INT
         | DOUBLE
         | ID
         ;
/* end for */

/* if region
* e.g. if (expressão)
          Corpo do if;
       else {
          Corpo do else;
       }
*/
if : IF L_PARENT expr R_PARENT ifstmt { $$ = $5; }
   ;

ifstmt : L_BRACES blocks R_BRACES { $$ = $2; }
       | L_BRACES blocks R_BRACES else { $$ = $2; }
       //TODO do this without give any shift/reduce conflict warning
      //  | block { $$ = $1; }
      //  | block else { $$ = $1; }
       ;

else : ELSE L_BRACES blocks R_BRACES { $$ = $3; }
     //TODO | ELSE block { $$ = $2; }
     ;
/* function region
* e.g. func umafuncao(idparam1, idparam2) { }
*/
funcdeclaration : FUNC ID L_PARENT args R_PARENT L_BRACES funcscope R_BRACES { symtab.newFunction($2, ST::Kind::function, $4);
                                                                               $$ = $2; }
                ;

funcscope  : funcblock
           | funcscope funcblock { $$ = $2; }
           ;
funcblock : vardeclaration SEMI_COLON
          | funcall SEMI_COLON
          | assignment SEMI_COLON
          | funcdeclaration
          | for
          | if
          | RETURN expr SEMI_COLON { $$ = $2; }
          ;

funcall : ID L_PARENT params R_PARENT { symtab.useFunction($1, $3); }
        ;

args : ID { symtab.newVariable($1, ST::Kind::variable, true); $$ = 1;}
     | args COMMA ID { symtab.newVariable($3, ST::Kind::variable, true); $$ += 1; }
     ;
params : value { $$ = 1; }
      | params COMMA value { $$ += 1; }
      ;
/*end region */

/* Expression region
*/
expr : value
     | expr COMP expr { $$ = $1; }
     | expr OP_PLUS expr { $$ = $1; }
     | expr OP_TIMES expr { $$ = $1; }
     | expr OP_DIV expr { $$ = $1; }
     | expr OP_MINUS expr { $$ = $1; }
     | expr OP_AND expr { $$ = $1; }
     | expr OP_OR expr { $$ = $1; }
     | OP_NOT expr { $$ = $2; }
     | OP_MINUS expr %prec OP_MIN_UN { $$ = $2; }
     | L_PARENT expr R_PARENT { $$ = $2; }
     ;
/*end region */

/* General region */
value : INT
      | DOUBLE
      | BOOL
      | ID { symtab.useVariable($1); }
      ;
/*end region */
%%
