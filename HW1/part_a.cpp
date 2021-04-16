#include <iostream>
#include <string>
#include <regex>
#include "tokens.hpp"

using std::string;
using std::cout;
using std::endl;
using std::regex;


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
            cout << yylineno << " " << token_to_string(token) << " //" << endl;
	    }
	    else if (token == STRING){
            printString();
	    }
	    else{
            cout << yylineno << " " << token_to_string(token) << " " << yytext << endl;
        }
	}
	return 0;
}