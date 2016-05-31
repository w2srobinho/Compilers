/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_INT = 258,
    T_REAL = 259,
    T_BOOL = 260,
    T_PLUS = 261,
    T_MINUS = 262,
    T_TIMES = 263,
    T_DIVISION = 264,
    T_EQUALS = 265,
    T_DIFFER = 266,
    T_GREATER = 267,
    T_LESS = 268,
    T_GREATER_OR_EQUAL = 269,
    T_LESS_OR_EQUAL = 270,
    T_AND = 271,
    T_OR = 272,
    T_NOT = 273,
    T_OPEN_PARENTHESES = 274,
    T_CLOSE_PARENTHESES = 275,
    T_OPEN_BRACKETS = 276,
    T_CLOSE_BRACKETS = 277,
    T_ASSIGN = 278,
    T_END_LINE = 279,
    T_COMMA = 280,
    T_COLON = 281,
    T_NL = 282,
    T_TYPE_INT = 283,
    T_TYPE_REAL = 284,
    T_TYPE_BOOL = 285,
    T_ID = 286,
    UnMinus_prec = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "parser.y" /* yacc.c:1909  */

  AST::NodeList *nodelist;
  AST::Node *node;
  AST::Block *block;
  const char *name;

#line 94 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
