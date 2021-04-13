#include "tokens.hpp"
#include <iostream>
#include <string>
#include <regex>

using std::string;
using std::cout;
using std::endl;
using std::regex;


string enumToString[34] = {"","VOID",
        "INT",
        "BYTE",
        "B",
        "BOOL",
        "AND",
        "OR",
        "NOT",
        "TRUE",
        "FALSE",
        "RETURN",
        "IF",
        "ELSE",
        "WHILE",
        "BREAK",
        "CONTINUE",
        "SWITCH",
        "CASE",
        "DEFAULT",
        "COLON",
        "SC",
        "COMMA",
        "LPAREN",
        "RPAREN",
        "LBRACE",
        "RBRACE",
        "ASSIGN",
        "RELOP",
        "BINOP",
        "COMMENT",
        "ID",
        "NUM",
        "STRING"};

void illegalLexima(){
    //TODO: Handle error
}

void printString(){
    //TODO: apply the string print requirements
}

int main()
{
	int token;
	while(token = yylex()) {
	// Your code here
	    if(token == -1){
	        illegalLexima();
	    }
	    else if (token == COMMENT){
            cout << yylineno << " " << enumToString[token] << " //" << endl;
	    }
	    else if (token == STRING){
            printString();
	    }
	    else{
            cout << yylineno << " " << enumToString[token] << " " << yytext << endl;
        }
	}
	return 0;
}