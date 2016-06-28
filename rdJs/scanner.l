%{
#include <string>
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

digs [0-9]+
chars [a-z]+
reals [0-9]+\.[0-9]*|[0-9]*\.[0-9]+

%%

[ \t\n]              ; //ignores spaces
"true"              {yylval.name = strdup(yytext); return BOOL;}
"false"             {yylval.name = strdup(yytext); return BOOL;}

{digs}              {yylval.name = strdup(yytext); return INT;}
{reals}             {yylval.name = strdup(yytext); return DOUBLE;}

"+"                 {return OP_PLUS;}
"*"                 {return OP_TIMES;}
"/"                 {return OP_DIV;}
"-"                 {return OP_MINUS;}

"&&"                {return OP_AND;}
"||"                {return OP_OR;}
"!"                 {return OP_NOT;}

"=="                {return COMP;}
"!="                {return COMP;}
"<"                 {return COMP;}
">"                 {return COMP;}
"<="                {return COMP;}
">="                {return COMP;}

"="                 {return ASSIGN;}
";"                 {return SEMI_COLON;}
","                 {return COMMA;}

"var"               {return VAR;}
"for"               {return FOR;}
"if"                {return IF;}
"else"              {return ELSE;}
"func"              {return FUNC;}
"return"            {return RETURN;}

"("                 {return L_PARENT;}
")"                 {return R_PARENT;}
"{"                 {return L_BRACES;}
"}"                 {return R_BRACES;}

{chars}             {yylval.name = strdup(yytext); return ID;}
.                   {yyerror("lexico: simbolo desconhecido: %c.", *yytext);} /*complains about every unknown character*/

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Linha %d] Erro ", yylineno);
    std::vfprintf(stderr, s, ap);
    std::fprintf(stderr, "\n");
}
