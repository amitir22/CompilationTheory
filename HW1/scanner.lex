%{
    #include <stdlib.h>
    #include "tokens.hpp"
%}

%option yylineno
%option noyywrap


digit   ([0-9])
nozerodigit ([1-9])
letter  ([a-zA-Z])
whitespace  ([\t\n\r ])


%%
{whitespace}                        ;
(void)                              return VOID;
(int)                               return INT;
(byte)                              return BYTE;
(b)                                 return B;
(and)                               return AND;
(or)                                return OR;
(not)                               return NOT;
(true)                              return TRUE;
(false)                             return FALSE;
(return)                            return RETURN;
(if)                                return IF;
(else)                              return ELSE;
(while)                             return WHILE;
(break)                             return BREAK;
(continue)                          return CONTINUE;
(switch)                            return SWITCH;
(case)                              return CASE;
(default)                           return DEFAULT;
(\:)                                return COLON;
(\;)                                return SC;
(\,)                                return COMMA;
(\()                                return LPAREN;
(\))                                return RPAREN;
(\{)                                return LBRACE;
(\})                                return RBRACE;
(=)                                 return ASSIGN;
(\/\/[^\n\r]+)                      return COMMENT;
((==)|(!=)|(\<=)|(\>=)|(\<)|(\>))   return RELOP;
((\+)|(\-)|(\*)|(\/))               return BINOP;
({letter}({letter}|{digit})*)       return ID;
(0|{nozerodigit}{digit}*)           return NUM;
(\"(?:[^"\\]|\\.)*\")               return STRING;
.                                   return -1;
%%