%{

#include "parser.h"
#define YYSTYPE struct parser

#include <stdio.h>
#include "tokens.hpp"
#include "parser.tab.hpp"
#include "hw3_output.hpp"

%}

%option yylineno
%option noyywrap

relop			(<|>|<=|>=)
eqop			(==|!=)
addop			([+-])
mulop			([*/])

%%

void    										return VOID;
int												return INT;
bool											return BOOL;
and    											return AND;
or    											return OR;
not    											return NOT;
true    										return TRUE;
false    										return FALSE;
return     										return RETURN;
if						    					return IF;
while    										return WHILE;
break    										return BREAK;
continue    									return CONTINUE;
;						    					return SC;
,    											return COMMA;
\(    											return LPAREN;
\)    											return RPAREN;
\{    											return LBRACE;
\}    											return RBRACE;
=						    					return ASSIGN;
{relop}					    					return RELOP;
{eqop}											return EQOP;
{addop}					    					return ADDOP;
{mulop}											return MULOP;
[a-zA-Z][0-9a-zA-Z]*							{yylval.var_name = yytext; return ID; }
([1-9][0-9]*|[0])				    			return NUM;
[/][/]+[^\r\n]*[\r|\n|\r\n]?                    ;
[ \n\t\r]                                       ;
.	                                            output::errorLex(yylineno);

%%

