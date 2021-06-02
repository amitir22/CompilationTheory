#ifndef HW3_SEMANTICS_HANDLER_H
#define HW3_SEMANTICS_HANDLER_H

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "hw3_output.hpp"

#define YYSTYPE Variable*

extern int yylineno;
extern char * yytext;
using namespace std;

void enterSwitch();
void exitSwitch();
void enterLoop();
void exitLoop();
void exitProgramFuncs();
void exitProgramRuntime();
void openNewScope();
void closeCurrentScope();
void printMessage(string message);
bool isDeclared(const string &name);
bool isDeclaredVariable(const string &name);

class Variable {
public:
    string value;

    explicit Variable(string str);

    Variable();

    virtual ~Variable() = default;

    friend ostream &operator<<(ostream &os, const Variable &node);
};

// Single row in the table of a scope
class SymbolTableRow {
public:
    string name;
    // This is for variables and function definitions
    // For a variable, the type at cell 0 is the actual type, other cells should be empty
    // For a function, all types except the last one, are parameter types, the last type is the return type of the function
    vector<string> type;
    int offset;
    bool isFunc;

    SymbolTableRow(string name, vector<string> type, int offset, bool isFunc);
};

// The object storing the entries of the current scope
class SymbolTable {
public:
    vector<shared_ptr<SymbolTableRow>> rows;

    SymbolTable() = default;
};

class Type : public Variable {
public:
    explicit Type(Variable *type);
};

class Call;

class Exp : public Variable {
public:
    // Type is used for tagging in bison when creating the Exp object
    string type;
    bool valueAsBooleanValue;

    // This is for NUM, NUM B, STRING, TRUE and FALSE
    Exp(Variable *terminal, string taggedTypeFromParser);

    // for Call
    explicit Exp(Call *call);

    // for exp in switch
    Exp(Exp *e1, string tag);

    // for NOT Exp
    Exp(Variable *notNode, Exp *exp);

    // for Exp RELOP, MUL, DIV, ADD, SUB, OR, AND Exp
    Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedTypeFromParser);

    // for Exp ID
    explicit Exp(Variable *id);

    // for Lparen Exp Rparen, need to just remove the parentheses
    Exp(Exp *ex);
};

class ExpList : public Variable {
public:
    vector<Exp> list;

    explicit ExpList(Exp *exp);

    ExpList(Exp *exp, ExpList *expList);
};

class Call : public Variable {
public:
    Call(Variable *id, ExpList *list);

    explicit Call(Variable *id);
};

class RetType : public Variable {
public:
    explicit RetType(Variable *type);
};

class Statements;

class CaseList;


class Statement : public Variable {
public:
    string dataTag;

    // For Lbrace Statements Rbrace
    explicit Statement(Statements *states);

    // For Type ID SC
    Statement(Type *t, Variable *id);

    // For Type ID Assign Exp SC
    Statement(Type *t, Variable *id, Exp *exp);

    // For ID Assign Exp SC
    Statement(Variable *id, Exp *exp);

    // For Call SC
    explicit Statement(Call *call);

    // For Return SC -> this is for a function with a void return type
    explicit Statement(const string& funcReturnType);

    // For Return Exp SC -> This is for a non-void function, exp stores the type so it is enough
    explicit Statement(Exp *exp);

    // For if,if/else,while
    Statement(string type, Exp *exp);

    // For break,continue
    explicit Statement(Variable *type);

    // For Switch LParen Exp RParen Lbrace CaseList Rbrace
    Statement(Exp *exp, CaseList *cList);
};

class Statements : public Variable {
public:
    // For Statement
    explicit Statements(Statement *state);

    // For Statements Statement
    Statements(Statements *states, Statement *state);
};

class CaseDecl : public Variable {
public:
    // For Case Num Colon Statements
    CaseDecl(Exp *num, Statements *states);
};

class CaseList : public Variable {
public:
    vector<CaseDecl *> cases;

    // For CaseDecl CaseList
    CaseList(CaseDecl *cDec, CaseList *cList);

    // For CaseDecl
    explicit CaseList(CaseDecl *cDec);

    // For Default Colon Statements
    explicit CaseList(Statements *states);
};

class FormalDecl : public Variable {
public:
    // The parameter type
    string type;

    // for Type ID
    FormalDecl(Type *t, Variable *id);
};

class FormalsList : public Variable {
public:
    vector<FormalDecl *> formals;

    // To initialize from an empty formal list
    explicit FormalsList(FormalDecl *formal);

    // To append a new formal to an existing formal list
    FormalsList(FormalDecl *formal, FormalsList *fList);
};

class Formals : public Variable {
public:
    vector<FormalDecl *> formals;

    // for Epsilon
    Formals();

    // for formalList
    explicit Formals(FormalsList *formList);
};

class FuncDecl : public Variable {
public:
    // This is an array to denote the types of the func parameters, with the func return type being the last elemtn of the array
    vector<string> type;

    FuncDecl(RetType *rType, Variable *id, Formals *funcParams);
};

class Funcs : public Variable {
public:
    Funcs();
};

class Program : public Variable {
public:
    Program();
};

void insertFunctionParameters(Formals *formals);

#endif //HW3_SEMANTICS_HANDLER_H
