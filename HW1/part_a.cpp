#include "tokens.hpp"
#include <iostream>
#include <string>
#include <regex>

using std::string;
using std::cout;
using std::endl;
using std::regex;
using std::smatch;
extern "C"{
    int yylex();
}

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
        cout << "Error " << yytext << endl;
        exit (0);
}
string xddToASCII(string s) {
    smatch m;
    regex xdd("\\\\x([1-7][0-9A-Fa-f]|0[1-9A-Fa-f])"); // all xdd but x00 which is treated differently

    while (regex_search(s, m, xdd)) {
        string actual_xdd = string(1, (char) strtol(m.str().substr(2,2).c_str(), nullptr,16));
        s = regex_replace(s, xdd, actual_xdd, std::regex_constants::format_first_only);
    }
    return s;
}

string cleanQuot(string s) {
    string s1 = s.substr(1, s.length() - 2);
    s1 = regex_replace(s1, regex("\\\\\\\\"), "TEMPSLASH");
    s1 = regex_replace(s1, regex("\\\\\""), "\"");
    return s1;
}

string checkEscaping(string s){
    string s1 = regex_replace(s, regex("\\\\x00"), "\\0");
    s1 = s1.substr(0, s1.find("\\0"));
    s1 = regex_replace(s1 , regex("\\\\n"), "\n");
    s1 = regex_replace(s1, regex("\\\\r"), "\r");
    s1 = regex_replace(s1, regex("\\\\t"), "\t");
    s1 = regex_replace(s1, regex("TEMPSLASH"), "\\");
    return s1;
}


void isValidString(string s){
    smatch m;
    if (regex_search(s, m, regex("(\\\\((x0[^0])|(x[^0]0)|(x[^0][^0])))|(\\\\[^0xnrt])|(\\\\x.{0,1}$)"))) {
        cout << "Error undefined escape sequence " << regex_replace(m.str(), regex("\\\\"), "", std::regex_constants::format_first_only) << endl;
        exit(0);
    }
    else if (regex_search(s, m, regex("\\\\(\\\\\\\\)*$")) and (m.prefix().str().back() != '\\')) {
        cout << "Error unclosed string" << endl;
        exit(0);
    }
}


void printString() {
    string s = string(yytext);
    s = xddToASCII(s);
    s = cleanQuot(s);
    isValidString(s);
    s = checkEscaping(s);
    cout << yylineno << " STRING " << s << endl;
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