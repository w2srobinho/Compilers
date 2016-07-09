%{
  #include "st.h"
  #include "ast.h"
  #include <iostream>
  #include <string>
  #include <vector>

  AST::Block *programRoot; /* the root node of our program AST:: */

  ST::SymbolTable *tablePointer = ST::scopeStack.back(); /*Initialize table pointer pointing to main table*/

  extern int yylex();
  extern void yyerror(const char* s, ...);
%}

/*Suddenly, the union decided not to find things included before...*/
%code requires{
  #include "ast.h"
}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
  AST::Node *node;
  AST::VarDeclaration *vardeclaration;
  AST::Block *block; //used to define basic blocks
  std::vector<AST::Node*>* nodelist;
  int counter;
  const char *name;       //Names of variables and values
  AST::Operation comp;
}

/* token defines our terminal symbols (tokens).
 */
%token OP_PLUS OP_TIMES OP_DIV OP_MINUS OP_AND OP_OR OP_NOT
%token ASSIGN SEMI_COLON COMMA
%token VAR FOR FUNC RETURN IF ELSE
%token L_PARENT R_PARENT L_BRACES R_BRACES
%token <comp> COMP
%token <name> ID INT DOUBLE BOOL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> block vardeclaration funcdeclaration funcall for assignment
%type <node> forvalue value expr else if createscope funcblock forblock

%type <block> blocks program funcscope forscope
%type <vardeclaration> varlist
%type <nodelist> args params

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

blocks  : block { $$ = new AST::Block(); if($1 != NULL) $$->lines.push_back($1); }
        | blocks block { if($2 != NULL) $1->lines.push_back($2); }
        ;

block   : vardeclaration SEMI_COLON
        | funcall SEMI_COLON
        | assignment SEMI_COLON
        | funcdeclaration
        | for
        | if
        | error SEMI_COLON { yyerrok; $$ = NULL; }
        ;

/* Variable declaration region.
* e.g. var umnome, segundonome;
*/
vardeclaration : VAR varlist { $$ = $2; }
               ;

varlist : ID { $$ = new AST::VarDeclaration(AST::Type::unknown);
               tablePointer->newVariable($1, ST::Kind::variable, false);
               $$->variables.push_back(new AST::Variable($1, AST::Type::unknown)); }
        | varlist COMMA ID { tablePointer->newVariable($3, ST::Kind::variable, false);
                             $1->variables.push_back(new AST::Variable($3, AST::Type::unknown)); }
        ;
/* end declaration */

/* Variable assignment region.
* e.g. umnome = 1; segundonome = umnome;
*/
assignment : ID ASSIGN expr { tablePointer->assignVariable($1);
                              AST::Node *node = new AST::Variable($1, AST::Type::unknown);
                              $$ = new AST::BinOp(node, AST::Operation::assign, $3); }
           ;

/* end assignment */

/* for definition
* e.g. for (12; 2) { scope }
*/
for : FOR L_PARENT forvalue SEMI_COLON forvalue R_PARENT L_BRACES forscope R_BRACES { $$ = new AST::For($3, $5, $8); }
    ;

forscope : forblock { $$ = new AST::Block(); if($1 != NULL) $$->lines.push_back($1); }
         | forscope forblock { if($2 != NULL) $1->lines.push_back($2); }
         ;

forblock : vardeclaration SEMI_COLON
         | for
         | if
         | assignment SEMI_COLON
         ;

forvalue : INT { $$ = new AST::Value($1, AST::Type::integer); }
         | DOUBLE { $$ = new AST::Value($1, AST::Type::real); }
         | ID { tablePointer->useVariable($1);
             $$ = new AST::Variable($1, AST::Type::unknown); }
         ;
/* end for */

/* if region
* e.g. if (expressão)
          Corpo do if;
       else {
          Corpo do else;
       }
*/
// if : IF L_PARENT expr R_PARENT ifstmt { $5->expression = $3; $$ = $5; }
//    ;

if : IF L_PARENT expr R_PARENT L_BRACES blocks R_BRACES { $$ = new AST::If($3, $6, NULL); }
   | IF L_PARENT expr R_PARENT L_BRACES blocks R_BRACES else { $$ = new AST::If($3, $6, $8); }
   ;
//        //TODO do this without give any shift/reduce conflict warning
//       //  | block { $$ = $1; }
//       //  | block else { $$ = $1; }
//        ;

else : ELSE L_BRACES blocks R_BRACES { $$ = $3; }
     //TODO | ELSE block { $$ = $2; }
     ;

/* function region
* e.g. func umafuncao(idparam1, idparam2) { }
*/
funcdeclaration : FUNC ID L_PARENT createscope args R_PARENT L_BRACES funcscope endscope R_BRACES {
                                                            tablePointer->newFunction($2, ST::Kind::function, $5->size());
                                                            $$ = new AST::FuncDeclaration($2, $5, $8); }
                ;
createscope : %empty { tablePointer = new ST::SymbolTable(tablePointer);
                ST::scopeStack.push_back(tablePointer);}
            ;
endscope : %empty { delete tablePointer;
             ST::scopeStack.pop_back();
             tablePointer = ST::scopeStack.back(); }
         ;

funcscope  : funcblock { $$ = new AST::Block(); if($1 != NULL) $$->lines.push_back($1); }
           | funcscope funcblock { if($2 != NULL) $1->lines.push_back($2); }
           ;

funcblock : vardeclaration SEMI_COLON
          | funcall SEMI_COLON
          | assignment SEMI_COLON
          | funcdeclaration
          | for
          | if
          | RETURN expr SEMI_COLON { $$ = $2; }
          ;

funcall : ID L_PARENT params R_PARENT { tablePointer->useFunction($1, $3->size()); $$ = new AST::FunCall($1, $3); }
        ;

args : %empty {}
     | ID { AST::Node *argument = new AST::Variable($1, AST::unknown);
            tablePointer->newVariable($1, ST::Kind::variable, true);
            $$ = new std::vector<AST::Node*> { argument };
          }
     | args COMMA ID { tablePointer->newVariable($3, ST::Kind::variable, true);
                       $$->push_back(new AST::Variable($3, AST::unknown));
                     }
     ;

params : %empty {}
     | value { $$ = new std::vector<AST::Node*> { $1 }; }
     | params COMMA value { $$->push_back($3); }
     ;
/*end region */

/* Expression region
*/
expr : value
     | expr OP_TIMES expr { $$ = new AST::BinOp($1, AST::Operation::times, $3); }
     | expr OP_MINUS expr { $$ = new AST::BinOp($1, AST::Operation::minus, $3); }
     | expr OP_PLUS expr { $$ = new AST::BinOp($1, AST::Operation::plus, $3); }
     | expr OP_DIV expr { $$ = new AST::BinOp($1, AST::Operation::division, $3); }
     | expr OP_AND expr { $$ = new AST::BinOp($1, AST::Operation::andop, $3); }
     | expr OP_OR expr { $$ = new AST::BinOp($1, AST::Operation::orop, $3); }
     | expr COMP expr { $$ = new AST::BinOp($1, $2, $3); }
     | OP_NOT expr { $$ = $2; }
     | OP_MINUS expr %prec OP_MIN_UN { $$ = $2; }
     | L_PARENT expr R_PARENT { $$ = $2; }
     ;
/*end region */

/* General region */
value : INT { $$ = new AST::Value($1, AST::Type::integer); }
      | DOUBLE { $$ = new AST::Value($1, AST::Type::real); }
      | BOOL { $$ = new AST::Value($1, AST::Type::boolean); }
      | ID { tablePointer->useVariable($1);
             $$ = new AST::Variable($1, AST::Type::unknown); }
      ;
/*end region */
%%
