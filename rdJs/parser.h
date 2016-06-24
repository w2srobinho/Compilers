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
    OP_PLUS = 258,
    OP_TIMES = 259,
    OP_DIV = 260,
    OP_MINUS = 261,
    OP_AND = 262,
    OP_OR = 263,
    OP_NOT = 264,
    ASSIGN = 265,
    SEMI_COLON = 266,
    COMMA = 267,
    VAR = 268,
    FOR = 269,
    FUNC = 270,
    RETURN = 271,
    L_PARENT = 272,
    R_PARENT = 273,
    L_BRACES = 274,
    R_BRACES = 275,
    ID = 276,
    INT = 277,
    DOUBLE = 278,
    BOOL = 279,
    COMP = 280,
    OP_MIN_UN = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "parser.y" /* yacc.c:1909  */

    //AST::Node *node;        //Node of the syntax tree
    //AST::Block *block;      //List of nodes of syntax trees
    //AST::VarDeclaration *vars; //List of variables in a declaration
    const char *name;       //Names of variables and values
    //Ops::Operation comp;    //Comparison operations ( =, ~=, <, <=, >, >= )

#line 89 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
