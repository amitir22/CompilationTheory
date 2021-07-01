%{
/* Declarations section */
#include <stdio.h>
#include "Classes.h"
#include "parser.tab.hpp"
#include "hw3_output.hpp"
%}

%option yylineno
%option noyywrap
%option nounput

digit   ([0-9])
nozerodigit ([1-9])
letter  ([a-zA-Z])
whitespace  ([\t\n\r ])



%%
{whitespace}                         ;
(void)                              yylval=new Variable(yytext); return VOID;
(int)                               yylval=new Variable(yytext); return INT;
(byte)                              yylval=new Variable(yytext); return BYTE;
(b)                                 yylval=new Variable(yytext); return B;
(bool)                              yylval=new Variable(yytext); return BOOL;
(and)                               yylval=new Variable(yytext); return AND;
(or)                                yylval=new Variable(yytext); return OR;
(not)                               yylval=new Variable(yytext); return NOT;
(true)                              yylval=new Variable(yytext); return TRUE;
(false)                             yylval=new Variable(yytext); return FALSE;
(return)                            yylval=new Variable(yytext); return RETURN;
(if)                                yylval=new Variable(yytext); return IF;
(else)                              yylval=new Variable(yytext); return ELSE;
(while)                             yylval=new Variable(yytext); return WHILE;
(continue)                          yylval=new Variable(yytext); return CONTINUE;
(break)                             yylval=new Variable(yytext); return BREAK;
(switch)                            yylval=new Variable(yytext); return SWITCH;
(case)                              yylval=new Variable(yytext); return CASE;
(default)                           yylval=new Variable(yytext); return DEFAULT;
(\:)                                yylval=new Variable(yytext); return COLON;
(\;)                                yylval=new Variable(yytext); return SC;
(\,)                                yylval=new Variable(yytext); return COMMA;
(\()                                yylval=new Variable(yytext); return LPAREN;
(\))                                yylval=new Variable(yytext); return RPAREN;
(\{)                                yylval=new Variable(yytext); return LBRACE;
(\})                                yylval=new Variable(yytext); return RBRACE;
(=)                                 yylval=new Variable(yytext); return ASSIGN;
((==)|(!=))                         yylval=new Variable(yytext); return EQ;
((\<=)|(\>=)|(\<)|(\>))             yylval=new Variable(yytext); return REL;
((\+)|(\-))                         yylval=new Variable(yytext); return PLUS_MINUS;
((\*)|(\/))                         yylval=new Variable(yytext); return MUL_DIV;
({letter}({letter}|{digit})*)       yylval=new Variable(yytext); return ID;
(0|{nozerodigit}{digit}*)           yylval=new Variable(yytext); return NUM;
(\/\/[^\r\n]*(\r|\n|\r\n)?)          ;
(\"([^\n\r\"\\]|\\[rnt"\\])+\")     yylval=new Variable(yytext); return STRING;
.                                   output::errorLex(yylineno); exit(0);

%%