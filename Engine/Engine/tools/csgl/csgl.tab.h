/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_CSGL_TAB_H_INCLUDED
# define YY_YY_CSGL_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_IFNDEF = 258,
     TOKEN_IFNDEF_END = 259,
     TOKEN_IFDEF = 260,
     TOKEN_IFDEF_END = 261,
     TOKEN_IFNDEF_FILE = 262,
     TOKEN_IFDEF_FILE = 263,
     TOKEN_ENDIF = 264,
     TOKEN_COMMENT_START = 265,
     TOKEN_COMMENT_END = 266,
     TOKEN_COMMENT_EX_START = 267,
     TOKEN_COMMENT_EX_END = 268,
     TOKEN_COMMENT_FILE = 269,
     TOKEN_COMMENT_EX_FILE = 270,
     TOKEN_INCLUDE_START = 271,
     TOKEN_INCLUDE_END = 272,
     TOKEN_INCLUDE_SIGN = 273,
     TOKEN_INCLUDE_FILE = 274,
     TOKEN_ENUM_START = 275,
     TOKEN_ENUM_CONTENT = 276,
     TOKEN_STRUCT_START = 277,
     TOKEN_STRUCT_END = 278,
     TOKEN_STRUCT_NAME = 279,
     TOKEN_STRUCT_CONTENT = 280,
     TOKEN_ID_START = 281,
     TOKEN_ID_END = 282,
     TOKEN_ID_NUM = 283,
     TOKEN_INTERFACE_END = 284,
     TOKEN_INTERFACE_ID_START = 285,
     TOKEN_INTERFACE_ID_NUM = 286,
     TOKEN_INTERFACE_ID_END = 287,
     TOKEN_INTERFACE_NAME = 288,
     TOKEN_INTERFACE_CONTENT = 289,
     TOKEN_INTERFACE_FUNCTION_NAME = 290
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_CSGL_TAB_H_INCLUDED  */
