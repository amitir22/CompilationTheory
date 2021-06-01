#include <iostream>
#include "hw3_output.hpp"
#include <sstream>

using namespace std;

void output::endScope(){
    cout << "---end scope---" << endl;
}

void output::printID(const string& id, int offset, const string& type) {
    cout << id << " " << type <<  " " << offset <<  endl;
}

string typeListToString(const std::vector<string>& argTypes) {
	stringstream res;
	res << "(";
	for(int i = 0; i < argTypes.size(); ++i) {
		res << argTypes[i];
		if (i + 1 < argTypes.size())
			res << ",";
	}
	res << ")";
	return res.str();
}

string ourTypeListToString(std::vector<struct param>* listParams) {
    if(listParams == nullptr)
        return "()";
    stringstream res;
    int size = listParams->size();
    res << "(";
    int i = 0;
    for(std::vector<struct param>::iterator it = listParams->begin(); it != listParams->end(); ++it) {
        res << (*it).type;
        if ((i++) + 1 < size)
            res << ",";
    }
    res << ")";
    return res.str();
}

string valueListsToString(const std::vector<string>& values) {
	stringstream res;
	res << "{";
	for(int i = 0; i < values.size(); ++i) {
		res << values[i];
		if (i + 1 < values.size())
			res << ",";
	}
	res << "}";
	return res.str();
}

string output::makeFunctionType(const string& retType, std::vector<struct param>* argTypes)
{
	stringstream res;
	res << ourTypeListToString(argTypes) << "->" << retType;
	return res.str();
}

void output::errorLex(int lineno){
    cout << "line " << lineno << ":" << " lexical error" << endl;
	exit(0);
}

void output::errorSyn(int lineno){
    cout << "line " << lineno << ":" << " syntax error" << endl;
	exit(0);
}

void output::errorUndef(int lineno, const string& id){
    cout << "line " << lineno << ":" << " variable " << id << " is not defined" << endl;
	exit(0);
}

void output::errorDef(int lineno, const string& id){
    cout << "line " << lineno << ":" << " identifier " << id << " is already defined" << endl;
	exit(0);
}

void output::errorUndefFunc(int lineno, const string& id) {
    cout << "line " << lineno << ":" << " function " << id << " is not defined" << endl;
	exit(0);
}

void output::errorMismatch(int lineno){
    cout << "line " << lineno << ":" << " type mismatch" << endl;
	exit(0);
}

void output::errorPrototypeMismatch(int lineno, const string& id, std::vector<string>& argTypes) {
    cout << "line " << lineno << ": prototype mismatch, function " << id << " expects arguments " << typeListToString(argTypes) << endl;
	exit(0);
}
	
void output::errorUnexpectedBreak(int lineno) {
	cout << "line " << lineno << ":" << " unexpected break statement" << endl;
	exit(0);	
}

void output::errorUnexpectedContinue(int lineno) {
	cout << "line " << lineno << ":" << " unexpected continue statement" << endl;
	exit(0);	
}

void output::errorMainMissing() {
	cout << "Program has no 'void main()' function" << endl;
	exit(0);
}

