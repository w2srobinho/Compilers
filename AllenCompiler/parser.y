%{
  /*
  #include "ast.h"
  #include "st.h"
  ST::SymbolTable symtab;  /* main symbol table
  AST::Block *programRoot; /* the root node of our program AST::
  */
  extern int yylex();
  extern void yyerror(const char* s, ...);
%}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    std::string t_integer,
                t_real,
                t_bools;
    AST::Node *node;
    AST::Block *block;
    const char *name;
}

/* token defines our terminal symbols (tokens).
 */
%token <t_integer> T_INT
%token <t_real> T_REAL
%token <t_bools> T_BOOL
%token T_PLUS T_MINUS T_TIMES T_DIVISION
%token T_EQUALS T_DIFFER T_GREATER T_LESS T_GREATER_OR_EQUAL T_LESS_OR_EQUAL
%token T_AND T_OR T_NOT T_OPEN_PARENTS T_CLOSE_PARENTS
%token T_ASSIGN T_END_LINE T_COMMA T_NL
%token T_TYPE_INT T_TYPE_REAL T_TYPE_BOOL
%token <name> T_ID

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line varlist var_type
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

program     : lines { programRoot = $1; }
            ;

lines       : line { $$ = new AST::Block(); $$->lines.push_back($1); }
            | lines line { if($2 != NULL) $1->lines.push_back($2); }
            ;

line        : T_NL { $$ = NULL; } /*nothing here to be used */
            | expr T_NL /*$$ = $1 when nothing is said*/
            | var_type varlist T_NL { $$ = symbol.update_symbols_table($1); }
            | T_ID T_ASSIGN expr {  AST::Node* node = symtab.assignVariable($1);
                                    $$ = new AST::BinOp(node, AST::assign, $3); }
            ;

var_type    : T_TYPE_INT { $$ = $1; }
            | T_TYPE_DOUBLE { $$ = $1; }
            ;

expr        : T_INT { $$ = new AST::Integer($1); }
            | T_ID { $$ = symtab.useVariable($1); }
            | expr T_PLUS expr { $$ = new AST::BinOp($1,AST::plus,$3); }
            | expr T_TIMES expr { $$ = new AST::BinOp($1,AST::times,$3); }
            | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
            ;

varlist     : T_ID { $$ = symtab.newVariable($1, NULL); }
            | varlist T_COMMA T_ID { $$ = symtab.newVariable($3, $1); }
            ;

%%
