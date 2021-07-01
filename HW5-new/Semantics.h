#ifndef HW3_SEMANTICS_H
#define HW3_SEMANTICS_H

#include <memory>
#include "vector"
#include <string>
#include <utility>
#include <ostream>
#include "hw3_output.hpp"
#include "bp.hpp"

#define YYSTYPE Variable*


extern int yylineno;
extern char *yytext;
using namespace std;
class Statement;
void enterSwitchState();

void exitSwitchState();

void enterLoop();

void exitProgramRuntimeState();

void openScope();

void closeScope();

string loadVariableFromSymTab(int offset, string type);

void printMessage(string message);

bool isDeclared(const string &name);

bool isDeclaredVariable(const string &name);

string GetLLVMType(string type);

// Single row in the table of a scope
class SymbolTableRecord {
public:
    string name;
    // This is for variables and function definitions
    // For a variable, the type at cell 0 is the actual type, other cells should be empty
    // For a function, all types except the last one, are parameter types, the last type is the return type of the function
    vector<string> type;
    int offset;
    bool isFunc;

    SymbolTableRecord(string name, vector<string> type, int offset, bool isFunc);
};

// The object storing the entries of the current scope
class SymbolTable {
public:
    vector<shared_ptr<SymbolTableRecord>> records;

    SymbolTable() = default;
};

class Variable {
public:
    string register_name;
    string value;
    string instruction;

    explicit Variable(string str);

    Variable();

    virtual ~Variable() = default;

    friend ostream &operator<<(ostream &os, const Variable &node);
};

// declarations used for cyclic class dependencies and for readability
class M;
class N;
class P;

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
    explicit Type(Variable *type);
};

class M : public Variable {
public:
    string instruction;

    M();
};

class N : public Variable {
public:
    string instruction;
    int loc;

    N();
};

class P : public Variable {
public:
    string instruction;
    int loc;

    explicit P(Exp *leftInstruction);
};

class Program : public Variable {
public:
    Program();
};

class Funcs : public Variable {
public:
    Funcs();
};

class FuncDecl : public Variable {
public:
    // function parameters types
    vector<string> functionParamsTypes;

    FuncDecl(RetType *rType, Variable *id, Formals *funcParams);
};

class RetType : public Variable {
public:
    explicit RetType(Variable *retType) : Variable(retType->value){};
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
    FormalDecl(Type *type, Variable *id) : Variable(id->value), paramType(type->value){};
};

class Statements : public Variable {
public:
    vector<pair<int, BranchLabelIndex>> break_list;
    vector<pair<int, BranchLabelIndex>> continue_list;

    // Statements -> Statement
    explicit Statements(Statement* statement);

    // Statements -> Statements Statement
    Statements(Statements* statements, Statement* statement);
};

class Statement : public Variable {
public:
    string dataTag;
    string register_name;
    vector<pair<int, BranchLabelIndex>> break_list;
    vector<pair<int, BranchLabelIndex>> continue_list;

    // Statement -> LBRACE Statements RBRACE
    explicit Statement(Statements* statements);

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
    Statement(string type, Exp *exp, Statement *wrappedStatement = nullptr);

    // Statement -> tl;dr: handles break, continue
    explicit Statement(Variable *type);

    // Statement -> SWITCH LPAREN Exp RPAREN LBRACE CaseList RBRACE
    Statement(Exp *exp, CaseList *cList);
};

class Call : public Variable {
public:
    // Call -> ID LPAREN ExpList RPAREN
    Call(Variable *id, ExpList *list);

    // Call -> ID LPAREN RPAREN
    explicit Call(Variable *id);
};

class ExpList : public Variable {
public:
    vector<Exp> expressionsList;

    // ExpList -> Exp
    explicit ExpList(Exp *exp);

    // ExpList -> Exp COMMA ExpList
    ExpList(Exp *exp, ExpList *expList);
};

class Exp : public Variable {
public:
    string type;
    bool actualBoolean; // useful only if a boolean expression
    vector<pair<int, BranchLabelIndex>> true_list;
    vector<pair<int, BranchLabelIndex>> false_list;


    // Exp -> LPAREN Exp RPAREN
    Exp(Exp *ex);

    // tl;dr: handles RELOP, MUL, DIV, ADD, SUB, OR, AND expressions
    Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedType, P *leftInstruction = nullptr);

    // Exp -> ID
    explicit Exp(Variable *id);

    // Exp -> Call
    explicit Exp(Call *call);

    // tl;dr: handles NUM, NUM B, STRING, TRUE and FALSE
    Exp(Variable *variable, string taggedType);

    // Exp -> NOT Exp
    Exp(Variable *variable, Exp *exp);

    // tl;dr: handles Exp in switch scope
    Exp(Exp *e1, string tag, N* label = nullptr);
};

class CaseList : public Variable {
public:
    vector<pair<int, BranchLabelIndex>> break_list;
    vector<pair<int, BranchLabelIndex>> continue_list;
    vector<CaseDecl *> cases;

    CaseList() { value = "case list"; }

    // CaseList -> CaseDecl CaseList
    CaseList(CaseDecl *caseDecl, CaseList *caseList);

    // CaseList -> CaseDecl
    explicit CaseList(CaseDecl *caseDecl);

    // CaseList -> DEFAULT COLON Statements
    explicit CaseList(Statements* statements, N* label);
};

class CaseDecl : public Variable {
public:
    int numToInt = 0;
    vector<pair<int, BranchLabelIndex>> break_list;
    vector<pair<int, BranchLabelIndex>> continue_list;

    // CaseDecl -> CASE NUM COLON Statements
    CaseDecl(Exp *num, Statements *states, Variable* caseLabel);

    CaseDecl();
};

Variable *parseBooleanCondition(Exp *leftInstruction);

void exitProgramFuncs(RetType *ret);

Statement *addElseStatementToBlock(Statement *ifStatement, Statement *elseStatment);

void exitLoop(N *first, P *second, Statement *statement);

void insertFunctionParameters(Formals *formals);

void backpatchIf(M *label, Exp *exp);

void backpatchIfElse(M *firstLabel, N *secondlabel, Exp *exp);

Statement *mergeIfElseLists(Statement *ifStatement, Statement *elseStatement);

string DeclareCaseLabel();

#endif //HW3_SEMANTICS_H
