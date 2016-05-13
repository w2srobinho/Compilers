%{
#include <string>
#include "ast.h"
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

digs [0-9]
chars [a-z]
reals [0-9]+\.[0-9]*|[0-9]*\.[0-9]+
bools "TRUE"|"FALSE"

%%

[ \t]    ; //ignores spaces
{digs}+    {yylval.types = yytext; return T_INT;}
{bools}    {yylval.types = yytext; return T_BOOL;}
{reals}    {yylval.types = yytext; return T_REAL;}

"+"        {return T_PLUS;}
"-"        {return T_MINUS;}
"*"        {return T_TIMES;}
"/"        {return T_DIVISION;}

":="       {return T_ASSIGN;}
"="        {return T_EQUALS;}
"~="       {return T_DIFFER;}
">"        {return T_GREATER;}
"<"        {return T_LESS;}
">="       {return T_GREATER_OR_EQUAL;}
">="       {return T_LESS_OR_EQUAL;}

"AND"      {return T_AND;}
"OR"       {return T_OR;}
"~"        {return T_NOT;}

"("        {return T_OPEN_PARENTHESES;}
")"        {return T_CLOSE_PARENTHESES;}
";"        {return T_END_LINE;}
":"        {return T_COLON;}

","        {return T_COMMA;}
\n         {return T_NL;}
"int"      {yylval.name = strdup(yytext); return T_TYPE_INT;}
"real"     {yylval.name = strdup(yytext); return T_TYPE_REAL;}
"bool"     {yylval.name = strdup(yytext); return T_TYPE_BOOL;}

{chars}+   {yylval.name = strdup(yytext); return T_ID;} // TODO Aumentar a range dos nomes das variáveis
.          {yyerror("Unknown token! %c\n", *yytext);} /*complains about every unknown character*/

%%

void yyerror(const char *s, ...) {
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "Line %d: error: ", yylineno);
    std::vfprintf(stderr, s, ap);
}
