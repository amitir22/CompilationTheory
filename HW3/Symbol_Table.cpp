#include <functional>
#include <set>
#include <iostream>
#include <sstream>
#include "Symbol_Table.h"

using namespace std;

// SymbolTable:

SymbolTable::SymbolTable(int offset) : table(), tableOffset(offset), size(0), scopeType("REGULAR") {}

SymbolTable::~SymbolTable() {}

void SymbolTable::addVariableToTable(string name, string type, int offset) {
    struct symbol element;
    element.name = name;
    element.type = type;
    element.offset = offset;
    if (offset >= 0) {
        this->tableOffset++;
    }
    element.order = size++;
    element.isFunction = false;
    element.listParams = nullptr;
    table[name] = element;
}

void SymbolTable::addFunctionToTable(string name, string type, vector<struct param> *listParams) {
    struct symbol element;
    element.name = name;
    element.type = type;
    element.offset = 0;
    element.order = size++;
    element.isFunction = true;
    element.listParams = listParams;
    table[name] = element;
}

void SymbolTable::printTableVariables() {
    typedef function<bool(pair<string, struct symbol>, pair<string, struct symbol>)> Comparator;

    Comparator compFunctor = [](pair<string, struct symbol> elem1, pair<string, struct symbol> elem2) {
        return elem1.second.order < elem2.second.order;
    };
    set<pair<string, struct symbol>, Comparator> tableElementSet(table.begin(), table.end(), compFunctor);
    for (pair<string, struct symbol> element : tableElementSet) {
        if (element.second.isFunction) {
            cout << element.first << " " << output::makeFunctionType(element.second.type, element.second.listParams)
                 << " 0" << endl;
        } else {
            output::printID(element.second.name, element.second.offset, element.second.type);
        }
    }
}


// SymbolTableStack:

SymbolTableStack::SymbolTableStack() {};

SymbolTableStack::~SymbolTableStack() {
    while (!scopes.empty()) {
        scopes.pop_back();
    }
}

void SymbolTableStack::addScope(SymbolTable *newScope) {
    scopes.push_back(newScope);
}

void SymbolTableStack::exitScope() {
    if (!scopes.empty()) {
        scopes.back()->SymbolTable::printTableVariables();

        delete scopes.back();
        scopes.pop_back();
    }
    // todo: should we call an error here?
}

struct symbol SymbolTableStack::getElement(string name) {
    vector<SymbolTable *>::iterator it = scopes.end();
    do {
        it--;
        map<string, struct symbol> &symbolTable = (*it)->table;
        for (auto const &x : symbolTable) {
            if (x.first == name) {
                return x.second;
            }
        }
    } while (it != scopes.begin());
    struct symbol element;
    element.name = name;
    element.type = "NONE";
    element.order = -1;
    element.offset = 100;
    element.listParams = nullptr;
    return element;
}

string SymbolTableStack::getLastDefinedFunction() {
    vector<SymbolTable *>::iterator it = scopes.end();
    do {
        it--;
        map<string, struct symbol> &symbolTable = (*it)->table;
        for (auto const &x : symbolTable) {
            if (x.second.order == ((*it)->size - 1) && x.second.isFunction) {
                return x.second.type;
            }
        }
    } while (it != scopes.begin());
    return "NONE";
}

// tl;dr: iterating in reverse to find a while scope type
bool SymbolTableStack::isInsideWhile() {
    vector<SymbolTable *>::iterator it = scopes.end();

    do {
        it--;
        if ((*it)->scopeType == "WHILE") {
            return true;
        }
    } while (it != scopes.begin());
    return false;
}

// todo: test
bool SymbolTableStack::isInsideSwitch() {
    vector<SymbolTable *>::iterator it = scopes.end();

    do {
        it--;
        if ((*it)->scopeType == "CASE" || (*it)->scopeType == "DEFAULT") {
            return true;
        }
    } while (it != scopes.begin());

    return false;
}