#include <iostream>
#include <string>
#include <regex>
#include "tokens.hpp"


using std::string;
using std::cout;
using std::endl;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::regex_replace;
using std::smatch;
using std::swap;

/*extern "C"{
    int yylex();
}*/

void reverseStr(string& str)
{
    int n = str.length();

    // Swap character starting from two
    // corners
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

void illegalLexima(){
    cout << "Error " << yytext << endl;
    exit (0);
}
string xddToASCII(string s) {
    smatch m;
    regex xdd("\\\\x([1-7][0-9A-Fa-f]|0[1-9A-Fa-f])"); // all xdd but x00 which is treated differently

    while (regex_search(s, m, xdd)) {
        string actual_xdd = string(1, (char) strtol(m.str().substr(2,2).c_str(), nullptr,16));
        if(actual_xdd == "\""){
            actual_xdd = "MRHT";
        }
        s = regex_replace(s, xdd, actual_xdd);
    }
    return s;
}

string cleanQuot(string s) {
    string s1 = s.substr(1, s.length() - 2);
    s1 = regex_replace(s1, regex("\\\\\\\\"), string("TEMPSLASH"));
    s1 = regex_replace(s1, regex("\\\\\""), string("\""));
    return s1;
}

string checkEscaping(string s){
    string s1 = regex_replace(s, regex("\\\\x00"), string("\\0"));
    s1 = s1.substr(0, s1.find("\\0"));
    s1 = regex_replace(s1 , regex("\\\\n"), string("\n"));
    s1 = regex_replace(s1, regex("\\\\r"), string("\r"));
    s1 = regex_replace(s1, regex("\\\\t"), string("\t"));
    s1 = regex_replace(s1, regex("MRHT"), string("\""));
    s1 = regex_replace(s1, regex("TEMPSLASH"), string("\\"));
    return s1;
}


void isValidString(string s){
    smatch m;
    string s1 = regex_replace(s, regex("\\\\n"), string("NEWLINE@"));

    if (regex_search(s, m, regex("\\\\(\\\\\\\\)*$")) and (m.prefix().str().back() != '\\')) {
        cout << "Error unclosed string" << endl;
        exit(0);
    }
    else if (regex_search(s1, m, regex("\n"))) {
        cout << "Error unclosed string" << endl; // the string ends on a different line
        exit(0);
    }
    else if (regex_search(s, m, regex("(\\\\[^0xntr])|(\\\\x[89G-Zg-z])"))) {
        cout << "Error undefined escape sequence " << regex_replace(m.str(), regex("\\\\"), string(""), std::regex_constants::format_first_only) << endl;
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
	while((token = yylex())) {
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