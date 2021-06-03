#ifndef HW3_SEMANTICS_HANDLER_H
#define HW3_SEMANTICS_HANDLER_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <utility>
#include "hw3_output.hpp"

#define YYSTYPE Variable*

extern int yylineno;
extern char * yytext;

using namespace std;

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

class Variable {
public:
    string value;

    explicit Variable(string str);

    Variable();

    virtual ~Variable() = default;

    friend ostream &operator<<(ostream &os, const Variable &node);
};

class Type : public Variable {
public:
    explicit Type(Variable *type);
};

class Call; // declaration used for cyclic class dependencies

class Exp : public Variable {
public:
    string type; // used when bison creates the Exp object
    bool valueAsBooleanValue;

    // tl;dr: handles NUM, NUM B, STRING, TRUE and FALSE
    Exp(Variable *terminal, string taggedTypeFromParser);

    // Exp -> Call
    explicit Exp(Call *call);

    // tl;dr: handles Exp in switch scope
    Exp(Exp *e1, string tag);

    // Exp -> NOT Exp
    Exp(Variable *notNode, Exp *exp);

    // tl;dr: handles RELOP, MUL, DIV, ADD, SUB, OR, AND expressions
    Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedTypeFromParser);

    // Exp -> ID
    explicit Exp(Variable *id);

    // Exp -> LPAREN Exp RPAREN
    Exp(Exp *ex);
};

class ExpList : public Variable {
public:
    vector<Exp> list;

    // ExpList -> Exp
    explicit ExpList(Exp *exp);

    // ExpList -> Exp COMMA ExpList
    ExpList(Exp *exp, ExpList *expList);
};

class Call : public Variable {
public:
    // Call -> ID LPAREN ExpList RPAREN
    Call(Variable *id, ExpList *list);

    // Call -> ID LPAREN RPAREN
    explicit Call(Variable *id);
};

class RetType : public Variable {
public:
    explicit RetType(Variable *type);
};

class Statements; // declaration used for cyclic class dependencies

class CaseList; // declaration used for cyclic class dependencies

class Statement : public Variable {
public:
    string dataTag;

    // Statement -> LBRACE Statements RBRACE
    explicit Statement(Statements *states);

    // Statement -> Type ID SC
    Statement(Type *t, Variable *id);

    // Statement -> Type ID ASSIGN Exp SC
    Statement(Type *t, Variable *id, Exp *exp);

    // Statement -> ID ASSIGN Exp SC
    Statement(Variable *id, Exp *exp);

    // Statement -> Call SC
    explicit Statement(Call *call);

    // Statement -> RETURN SC
    explicit Statement(const string& funcReturnType);

    // Statement -> RETURN Exp SC
    explicit Statement(Exp *exp);

    // Statement -> tl;dr: handles if, if-else, while
    Statement(string type, Exp *exp);

    // Statement -> tl;dr: handles break, continue
    explicit Statement(Variable *type);

    // Statement -> SWITCH LPAREN Exp RPAREN LBRACE CaseList RBRACE
    Statement(Exp *exp, CaseList *cList);
};

class Statements : public Variable {
public:
    // Statements -> Statement
    explicit Statements(Statement *state);

    // Statements -> Statements Statement
    Statements(Statements *states, Statement *state);
};

class CaseDecl : public Variable {
public:
    // CaseDecl -> CASE NUM COLON Statements
    CaseDecl(Exp *num, Statements *states);
};

class CaseList : public Variable {
public:
    vector<CaseDecl *> cases;

    // CaseList -> CaseDecl CaseList
    CaseList(CaseDecl *cDec, CaseList *cList);

    // CaseList -> CaseDecl
    explicit CaseList(CaseDecl *cDec);

    // CaseList -> DEFAULT COLON Statements
    explicit CaseList(Statements *states);
};

class FormalDecl : public Variable {
public:
    // The parameter type
    string type;

    // FormalDecl -> Type ID
    FormalDecl(Type *t, Variable *id);
};

class FormalsList : public Variable {
public:
    vector<FormalDecl *> formals;

    // FormalsList -> FormalDecl
    explicit FormalsList(FormalDecl *formal);

    // FormalsList -> FormalDecl COMMA FormalsList
    FormalsList(FormalDecl *formal, FormalsList *fList);
};

class Formals : public Variable {
public:
    vector<FormalDecl *> formals;

    // Formals -> Epsilon
    Formals();

    // Formals -> FormalsList
    explicit Formals(FormalsList *formList);
};

class FuncDecl : public Variable {
public:
    // function parameters types
    vector<string> type; // todo: rename to function_parameters_types

    // FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE
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

namespace GlobalSemanticStateHandler {
    // global variables declarations:
    extern string currentRunningFunctionScopeId;
    extern vector<shared_ptr<SymbolTable>> symTabStack;
    extern vector<int> offsetStack;
    extern vector<string> varTypes;
    extern int loopCounter;
    extern bool inSwitch;

    // global functions declarations:

    // printers:
    void printVector(vector<string> vec);
    void printMessage(string message);
    void printSymTabRow(shared_ptr<SymbolTableRow> row);
    void printSymTableStack();

    // state handlers:
    void enterSwitchState();
    void exitSwitchState();
    void enterLoopState();
    void exitLoopState();
    void exitProgramFuncsState(); // entering this state is already handled and happens once
    void exitProgramRuntimeState(); // same as here ^
    void openScope();
    void closeScope();

    // helper functions:
    bool isDeclared(const string &name);
    bool isDeclaredVariable(const string &name);
    void insertFunctionParameters(Formals *formals);
}

#endif //HW3_SEMANTICS_HANDLER_H
