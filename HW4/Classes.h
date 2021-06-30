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
#include "bp.hpp"

#define YYSTYPE Variable*

using namespace std;

extern int yylineno;
extern char * yytext;

class SymbolTableRecord {
public:
    string name; // basically the identifier (variable/function name)
    vector<string> type; // will have singular value if a variable and if a function the last type is the RetType
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

// todo: remove comments here:
// added from reference as the equivalent class: TypeNode in Semantics.h
class Variable {
public:
    string regName; // added from reference todo: delete comment and maybe reorganize/rename members
    string value;
    string instruction; // added from reference todo: delete comment

    Variable();

    explicit Variable(string str);

    virtual ~Variable() = default;

    friend ostream &operator<<(ostream &os, const Variable &node);
};

// declarations used for cyclic class dependencies and for readability
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
class M;
class N;
class P;

// todo: unfinished in parser.ypp
class Program : public Variable {
private:
    // todo: maybe should delete:
    shared_ptr<SymbolTableRecord> initPrintFunc();
    shared_ptr<SymbolTableRecord> initPrintIFunc();
public:
    Program();
};

class Funcs : public Variable {
public:
    Funcs();
};

// todo: unfinished in parser.ypp
class FuncDecl : public Variable {
private:
    shared_ptr<SymbolTableRecord> buildSymbolTableRecordFromFuncDecl();

public:
    // function parameters types
    vector<string> functionParamsTypes;

    // FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE
    FuncDecl(RetType *retType, Variable *id, Formals *funcParams);
};

class RetType : public Variable {
public:
    explicit RetType(Variable *retType) : Variable(retType->value) { }
};

class Formals : public Variable {
public:
    vector<FormalDecl *> formals;

    // Formals -> Epsilon
    Formals() = default;

    // Formals -> FormalsList
    explicit Formals(FormalsList *formalList);
};

class FormalsList : public Variable {
public:
    vector<FormalDecl *> formals;

    // FormalsList -> FormalDecl
    explicit FormalsList(FormalDecl *formalDecl);

    // FormalsList -> FormalDecl COMMA FormalsList
    FormalsList(FormalDecl *formalDecl, FormalsList *formalsList);
};

class FormalDecl : public Variable {
public:
    string paramType;

    // FormalDecl -> Type ID
    FormalDecl(Type *type, Variable *id) : Variable(id->value), paramType(type->value) { }
};

// todo: unfinished in parser.ypp
class Statements : public Variable {
public:
    // todo: rename
    vector<pair<int, BranchLabelIndex>> breakList;
    vector<pair<int, BranchLabelIndex>> continueList;

    // Statements -> Statement
    explicit Statements(Statement *statement);

    // Statements -> Statements Statement
    Statements(Statements *statements, Statement *statement);
};

// todo: unfinished in parser.ypp
class Statement : public Variable {
public:
    // todo: need to be renamed or reorganized
    string dataTag;
    string regName;
    vector<pair<int, BranchLabelIndex>> breakList;
    vector<pair<int, BranchLabelIndex>> continueList;

    // Statement -> LBRACE Statements RBRACE
    explicit Statement(Statements *statements);

    // Statement -> Type ID SC
    Statement(Type *type, Variable *id);

    // Statement -> Type ID ASSIGN Exp SC
    Statement(Type *type, Variable *id, Exp *exp);

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
    explicit Statement(Variable *variable);

    // Statement -> SWITCH LPAREN Exp RPAREN LBRACE CaseList RBRACE
    Statement(Exp *exp, CaseList *caseList);
};

// todo: unfinished in parser.ypp
class Call : public Variable {
public:
    // Call -> ID LPAREN ExpList RPAREN
    Call(Variable *id, ExpList *list);

    // Call -> ID LPAREN RPAREN
    explicit Call(Variable *id);
};

// todo: unfinished in parser.ypp
class ExpList : public Variable {
public:
    vector<Exp> expressionsList;

    // ExpList -> Exp
    explicit ExpList(Exp *exp);

    // ExpList -> Exp COMMA ExpList
    ExpList(Exp *exp, ExpList *expList);
};

// todo: unfinished in parser.ypp
class Type : public Variable {
public:
    explicit Type(Variable *type) : Variable(type->value) { }
};

// todo: unfinished in parser.ypp
class Exp : public Variable {
public:
    string type; // used when bison creates the Exp object
    bool realBooleanValue; // useful only if a boolean expression

    // Exp -> LPAREN Exp RPAREN
    Exp(Exp *exp);

    // tl;dr: handles RELOP, MUL, DIV, ADD, SUB, OR, AND expressions
    Exp(Exp *exp1, Variable *op, Exp *exp2, const string &taggedType);

    // Exp -> ID
    explicit Exp(Variable *variable);

    // Exp -> Call
    explicit Exp(Call *call) : Variable(call->value), type(call->value) { }

    // tl;dr: handles NUM, NUM B, STRING, TRUE and FALSE
    Exp(Variable *variable, string taggedType);

    // Exp -> NOT Exp
    Exp(Variable *variable, Exp *exp);

    // tl;dr: handles Exp in switch scope
    Exp(Exp *exp, string tag);
};

// todo: unfinished in parser.ypp
class CaseList : public Variable {
public:
    vector<CaseDecl *> caseDeclarations;

    CaseList() { value = "case list"; }

    // CaseList -> CaseDecl CaseList
    CaseList(CaseDecl *caseDecl, CaseList *caseList);

    // CaseList -> CaseDecl
    explicit CaseList(CaseDecl *caseDecl);

    // CaseList -> DEFAULT COLON Statements
    explicit CaseList(Statements *statements) {}
};

// todo: unfinished in parser.ypp
class CaseDecl : public Variable {
private:
    int numericValue = 0;
    vector<pair<int, BranchLabelIndex>> breakList;
    vector<pair<int, BranchLabelIndex>> continueList;
public:
    // CaseDecl -> CASE NUM COLON Statements
    CaseDecl(Exp *expression, Statements *statements);
};

// todo: unfinished in parser.ypp
class M : public Variable {
public:
    string instruction;

    M();
};

// todo: unfinished in parser.ypp
class N : public Variable {
public:
    string instruction;
    int loc; // todo: maybe rename loc in N and P to pos (like in seek/tell)?

    N();
};

// todo: unfinished in parser.ypp
class P : public Variable {
    string instruction;
    int loc;

    // todo: rename parameter
    explicit P(Exp *leftHandInstruction);
};

#endif //CLASSES_H
