//
// Created by Michael on 01-Jun-20.
//

#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <map>
#include "hw3_output.hpp"
#include <vector>

struct symbol {
    string name;
    string type;
    int offset;
    int order;
    bool isFunction;
    std::vector<struct param>* listParams;
};

class SymbolTable{
public:

    int tableOffset;
    std::map<string, struct symbol> table;
    int size;
    string scopeType;

    SymbolTable(int offset);
    ~SymbolTable();
    void addVariableToTable(string name, string type, int offset);
    void addFunctionToTable(string name, string type, std::vector<struct param>* args = nullptr);
    void printTableVariables();

    static bool listParamIsValid(struct symbol func, std::vector<struct param>* query)
    {
        if (!query || query->empty())
        {
            return false;
        }
        int funcArgSize = func.listParams->size();
        int queryArgSize = query->size();
        if (funcArgSize != queryArgSize)
        {
            return false;
        }
        std::vector<struct param>* funcArgs = func.listParams;
        for (int i=0; i < funcArgSize; i++)
        {
            if( funcArgs->at(i).type != query->at(i).type )
            {
                return false;
            }
        }
        return true;
    }
};

class SymbolTableStack{
public:
    std::vector<SymbolTable*> scopes;
    SymbolTableStack();
    ~SymbolTableStack();
    void addScope(SymbolTable* newScope);
    void exitScope(); //removes the last scope in the vector scope.
    struct symbol getElement(string name);
    string getLastDefinedFunction();
    bool isInsideWhile();
};


#endif /* SYMBOL_TABLE_H_ */