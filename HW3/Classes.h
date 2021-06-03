#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <utility>
#include "hw3_output.hpp"

#define YYSTYPE Variable*

extern int yylineno;
extern char * yytext;

using namespace std;

class SymbolTableRecord {
public:
    string name;
    vector<string> type;
    int offset;
    bool isFunc;

    SymbolTableRecord(string name, vector<string> type, int offset, bool isFunc)
            : name(std::move(name)), type(std::move(type)), offset(offset), isFunc(isFunc) { }
};

class SymbolTable {
public:
    vector<shared_ptr<SymbolTableRecord>> records;

    SymbolTable() = default;
};

class Variable {
public:
    string value;

    Variable();

    explicit Variable(string str);

    virtual ~Variable() = default;

    friend ostream &operator<<(ostream &os, const Variable &node);
};


class Program;
class Funcs;
class FuncDecl;
class RetType;
class Formals;
class FormalsList;
class FormalDecl;
class Statements;
class Statement;
class Call;
class ExpList;
class Type;
class Exp;
class CaseList;
class CaseDecl;

class Type : public Variable {
public:
    explicit Type(Variable *type) : Variable(type->value) { }
};

class Program : public Variable {
private:
    shared_ptr<SymbolTableRecord> initPrintFunc();
    shared_ptr<SymbolTableRecord> initPrintIFunc();
public:
    Program();
};

class Funcs : public Variable {
public:
    Funcs();
};

class FuncDecl : public Variable {
private:
    shared_ptr<SymbolTableRecord> buildSymbolTableRecordFromFuncDecl();

public:

    vector<string> functionParamsTypes;


    FuncDecl(RetType *retType, Variable *id, Formals *funcParams);
};

class RetType : public Variable {
public:
    explicit RetType(Variable *retType) : Variable(retType->value) { }
};

class Formals : public Variable {
public:
    vector<FormalDecl *> formals;


    Formals() = default;


    explicit Formals(FormalsList *formalList);
};

class FormalsList : public Variable {
public:
    vector<FormalDecl *> formals;


    explicit FormalsList(FormalDecl *formalDecl);


    FormalsList(FormalDecl *formalDecl, FormalsList *formalsList);
};

class FormalDecl : public Variable {
public:
    string paramType;


    FormalDecl(Type *type, Variable *id) : Variable(id->value), paramType(type->value) { }
};

class Statements : public Variable {
public:

    explicit Statements(Statement *statement) { }


    Statements(Statements *statements, Statement *statement) { }
};

class Statement : public Variable {
public:
    explicit Statement(Statements *statements) { }


    Statement(Type *type, Variable *id);


    Statement(Type *type, Variable *id, Exp *exp);


    Statement(Variable *id, Exp *exp);


    explicit Statement(Call *call) { }


    explicit Statement(const string& funcReturnType);


    explicit Statement(Exp *exp);


    Statement(string type, Exp *exp);


    explicit Statement(Variable *variable);


    Statement(Exp *exp, CaseList *caseList);
};

class Call : public Variable {
public:

    Call(Variable *id, ExpList *list);


    explicit Call(Variable *id);
};

class ExpList : public Variable {
public:
    vector<Exp> expressionsList;


    explicit ExpList(Exp *exp);


    ExpList(Exp *exp, ExpList *expList);
};


class Exp : public Variable {
public:
    string type;
    bool realBooleanValue;


    Exp(Exp *exp);


    Exp(Exp *exp1, Variable *op, Exp *exp2, const string &taggedType);


    explicit Exp(Variable *variable);


    explicit Exp(Call *call) : Variable(call->value), type(call->value) { }


    Exp(Variable *variable, string taggedType);


    Exp(Variable *variable, Exp *exp);


    Exp(Exp *exp, string tag);
};

class CaseList : public Variable {
public:
    vector<CaseDecl *> caseDeclarations;

    CaseList() { value = "case list"; }


    CaseList(CaseDecl *caseDecl, CaseList *caseList);


    explicit CaseList(CaseDecl *caseDecl);


    explicit CaseList(Statements *statements) {}
};

class CaseDecl : public Variable {
public:

    CaseDecl(Exp *expression, Statements *statements);
};




#endif //CLASSES_H
