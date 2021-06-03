#include <iostream>
#include "SemanticsHandler.h"

using namespace std;

extern char *yytext;

// global variables definitions:
string GlobalSemanticStateHandler::currentRunningFunctionScopeId;
vector<shared_ptr<SymbolTable>> GlobalSemanticStateHandler::symTabStack;
vector<int> GlobalSemanticStateHandler::offsetStack;
vector<string> GlobalSemanticStateHandler::varTypes = {"VOID", "INT", "BYTE", "BOOL", "STRING"};
int GlobalSemanticStateHandler::loopCounter = 0;
bool GlobalSemanticStateHandler::inSwitch = false;

// global functions definitions:
void GlobalSemanticStateHandler::printVector(vector<string> vec) {
    for (auto &i : vec) {
        std::cout << i << " ";
    }
}

void GlobalSemanticStateHandler::printMessage(string message) {
    std::cout << message << std::endl;
}

void GlobalSemanticStateHandler::printSymTabRow(shared_ptr<SymbolTableRow> row) {
    std::cout << row->name << " | ";
    printVector(row->type);
    std::cout << " | " << row->offset << " | " << row->isFunc << std::endl;
}

void GlobalSemanticStateHandler::printSymTableStack() {
    std::cout << "Size of global symbol table stack is: " << symTabStack.size() << std::endl;
    std::cout << "id | parameter types | offset | is func" << std::endl;
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->rows) {
            printSymTabRow(row);
        }
    }
}

void GlobalSemanticStateHandler::enterSwitchState() {
    inSwitch = true;
}

void GlobalSemanticStateHandler::exitSwitchState() {
    inSwitch = false;
}

void GlobalSemanticStateHandler::enterLoopState() {
    loopCounter++;
}

void GlobalSemanticStateHandler::exitLoopState() {
    loopCounter--;
}

void GlobalSemanticStateHandler::exitProgramFuncsState() {
    currentRunningFunctionScopeId = "";
}

void GlobalSemanticStateHandler::exitProgramRuntimeState() {
    shared_ptr<SymbolTable> globalScope = symTabStack.front();
    bool mainFunc = false;
    for (auto &row : globalScope->rows) {
        if (row->isFunc && row->name == "main" && row->type.back() == "VOID" && row->type.size() == 1) {
            mainFunc = true;
        }
    }
    if (!mainFunc) {
        output::errorMainMissing();
        exit(0);
    }
    closeScope();
}

void GlobalSemanticStateHandler::openScope() {
    shared_ptr<SymbolTable> nScope = make_shared<SymbolTable>();
    symTabStack.push_back(nScope);
    offsetStack.push_back(offsetStack.back());
}

void GlobalSemanticStateHandler::closeScope() {
    output::endScope();
    shared_ptr<SymbolTable> currentScope = symTabStack.back();
    for (auto &row : currentScope->rows) {
        if (!row->isFunc) {
            // Print a normal variable
            output::printID(row->name, row->offset, row->type[0]);
        } else {
            string funcReturnType = row->type.back();
            // Taking out the return type from the vector for easy printing
            row->type.pop_back();
            output::printID(row->name, row->offset,
                            output::makeFunctionType(funcReturnType, row->type));
        }
    }

    currentScope->rows.clear();
    symTabStack.pop_back();
    offsetStack.pop_back();

}

bool GlobalSemanticStateHandler::isDeclared(const string &name) {
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->rows) {
            if (row->name == name) {
                return true;
            }
        }
    }

    return false;
}

bool GlobalSemanticStateHandler::isDeclaredVariable(const string &name) {
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->rows) {
            if (row->name == name && !row->isFunc) {
                return true;
            }
        }
    }

    return false;
}

void GlobalSemanticStateHandler::insertFunctionParameters(Formals *formals) {
    for (unsigned int i = 0; i < formals->formals.size(); ++i) {
        vector<string> nType = {formals->formals[i]->type};
        shared_ptr<SymbolTableRow> nParameter = make_shared<SymbolTableRow>(formals->formals[i]->value, nType, -i - 1, false);
        symTabStack.back()->rows.push_back(nParameter);
    }
}

// Symbol-Table classes implementation:

SymbolTableRow::SymbolTableRow(string name, vector<string> type, int offset, bool isFunc)
        : name(std::move(name)), type(std::move(type)), offset(offset), isFunc(isFunc) { }

// Variable classes implementation:

Variable::Variable(string str) : value() {
    if (str == "void") {
        value = "VOID";
    } else if (str == "bool") {
        value = "BOOL";
    } else if (str == "int") {
        value = "INT";
    } else if (str == "byte") {
        value = "BYTE";
    } else {
        value = str;
    }
}

Variable::Variable() {
    value = "";
}

ostream &operator<<(ostream &os, const Variable &node) {
    os << "value: " << node.value;
    return os;
}


Type::Type(Variable *type) : Variable(type->value) { }


Exp::Exp(Call *call) {
    // Need to just take the return value of the function and use it as the return type of the expression
    value = call->value;
    type = call->value;
}

Exp::Exp(Variable *id) {
    // Need to make sure that the variable/func we want to use is declared
    if (!GlobalSemanticStateHandler::isDeclaredVariable(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }

    // Need to save the type of the variable function as the type of the expression
    for (int i = GlobalSemanticStateHandler::symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : GlobalSemanticStateHandler::symTabStack[i]->rows) {
            if (row->name == id->value) {
                // We found the variable/func we wanted to use in the expression
                value = id->value;
                // Getting the type of the variable, or the return type of the function
                type = row->type.back();
                return;
            }
        }
    }
}

Exp::Exp(Variable *notNode, Exp *exp) {
    if (exp->type != "BOOL") {
        // This is not a boolean expression, can't apply NOT
        output::errorMismatch(yylineno);
        exit(0);
    }
    type = "BOOL";
    valueAsBooleanValue = !valueAsBooleanValue;
}

Exp::Exp(Variable *terminal, string taggedTypeFromParser) : Variable(terminal->value) {
    type = taggedTypeFromParser;
    if (taggedTypeFromParser == "NUM") {
        type = "INT";
    }
    if (type == "BYTE") {
        // Need to check that BYTE size is legal
        if (stoi(terminal->value) > 255) {
            // Byte is too large
            output::errorByteTooLarge(yylineno, terminal->value);
            exit(0);
        }
    }
    if (type == "BOOL") {
        if (terminal->value == "true") {
            valueAsBooleanValue = true;
        } else {
            valueAsBooleanValue = false;
        }
    }
}

Exp::Exp(Exp *ex) {
    value = ex->value;
    type = ex->type;
    valueAsBooleanValue = ex->valueAsBooleanValue;
}

Exp::Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedTypeFromParser) {
    // Need to check the type of the expressions on each side, to make sure that a logical operator (AND/OR) is used on a boolean type
    if ((e1->type == "INT" || e1->type == "BYTE") && (e2->type == "INT" || e2->type == "BYTE")) {
        if (taggedTypeFromParser == "EQ_NEQ_RELOP" || taggedTypeFromParser == "REL_RELOP") {
            // This is a boolean operation performed between 2 numbers (>=,<=,>,<,!=,==)
            type = "BOOL";
        } else if (taggedTypeFromParser == "ADD_SUB_BINOP" || taggedTypeFromParser == "MUL_DIV_BINOP") {
            // This is an arithmetic operation between two numbers
            if (e1->type == "INT" || e2->type == "INT") {
                // An automatic cast to int will be performed in case one of the operands is of integer type
                type = "INT";
            } else {
                type = "BYTE";
            }
        }
    } else if (e1->type == "BOOL" && e2->type == "BOOL") {
        // Both operands are boolean so this should be a boolean operation
        type = "BOOL";
        if (taggedTypeFromParser == "AND" || taggedTypeFromParser == "OR") {
            if (op->value == "AND") {
                if (e1->valueAsBooleanValue && e2->valueAsBooleanValue) {
                    valueAsBooleanValue = true;
                } else {
                    valueAsBooleanValue = false;
                }
            } else if (op->value == "OR") {
                if (e1->valueAsBooleanValue || e2->valueAsBooleanValue) {
                    valueAsBooleanValue = true;
                } else {
                    valueAsBooleanValue = false;
                }
            }
        } else {
            output::errorMismatch(yylineno);
            exit(0);
        }
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Exp::Exp(Exp *e1, string tag) {
    if (tag == "switch" && (e1->type != "INT" && e1->type != "BYTE")) {
        output::errorMismatch(yylineno);
        exit(0);
    }
}


ExpList::ExpList(Exp *exp) {
    list.insert(list.begin(), exp);
}

ExpList::ExpList(Exp *exp, ExpList *expList) {
    list = vector<Exp>(expList->list);
    list.insert(list.begin(), exp);
}


Call::Call(Variable *id) {
    for (auto &symTab : GlobalSemanticStateHandler::symTabStack) {
        for (auto &row : symTab->rows) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    // We found a declaration of a variable with the same name, illegal
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == 1) {
                    // We found the right function, has the same name, it really is a function, and receives no parameters
                    // Saving the type of the function call return value
                    value = row->type.back();
                    return;
                } else {
                    row->type.pop_back();
                    output::errorPrototypeMismatch(yylineno, id->value, row->type);
                    exit(0);
                }
            }
        }
    }
    // We didn't find a declaration of the desired function
    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}

Call::Call(Variable *id, ExpList *list) {
    for (auto &symTab : GlobalSemanticStateHandler::symTabStack) {
        for (auto &row : symTab->rows) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    // We found a declaration of a variable with the same name, illegal
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == list->list.size() + 1) {
                    // We found the right function, has the same name, it really is a function
                    // Now we need to check that the parameter types are correct between what the function accepts, and what was sent
                    for (unsigned int i = 0; i < list->list.size(); ++i) {
                        if (list->list[i].type == row->type[i]) {
                            // This parameter is of matching type so it is ok
                            continue;
                        } else if (list->list[i].type == "BYTE" && row->type[i] == "INT") {
                            // The function receives int as a paramter, in this instance a byte was sent, but it is ok to cast from BYTE to INT
                            continue;
                        }

                        // Removing the return type of the function so we have an easy list of requested parameters to print
                        row->type.pop_back();
                        output::errorPrototypeMismatch(yylineno, id->value, row->type);
                        exit(0);
                    }

                    // Saving the type of the function call return value
                    value = row->type.back();
                    return;
                } else {
                    // The number of parameters we received does not match the number the function takes as arguments
                    // Removing the return type of the function so we have an easy list of requested parameters to print
                    row->type.pop_back();
                    output::errorPrototypeMismatch(yylineno, id->value, row->type);
                    exit(0);
                }
            }
        }
    }

    // We didn't find a declaration of the desired function
    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}


RetType::RetType(Variable *type) : Variable(type->value) { }


Statement::Statement(Variable *type) {
    if (GlobalSemanticStateHandler::loopCounter == 0 && !GlobalSemanticStateHandler::inSwitch) {
        // We are not inside any loop, so a break or continue is illegal in this context
        if (type->value == "break") {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        } else if (type->value == "continue") {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        } else {
        }
    } else if (type->value == "continue" && GlobalSemanticStateHandler::inSwitch) {
        output::errorUnexpectedContinue(yylineno);
        exit(0);
    }
    dataTag = "break or continue";
}

Statement::Statement(string type, Exp *exp) {
    if (exp->type != "BOOL") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    dataTag = "if if else while";
}

Statement::Statement(const string &funcReturnType) {
    // Need to check if the current running function is of void type
    for (int i = GlobalSemanticStateHandler::symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symTabStack[i]->rows) {
            if (row->isFunc &&
                row->name == GlobalSemanticStateHandler::currentRunningFunctionScopeId) {
                // We found the current running function
                if (row->type.back() == funcReturnType) {
                    dataTag = "void return value";
                } else {
                    output::errorMismatch(yylineno);
                    exit(0);
                }
            }
        }
    }
}

Statement::Statement(Exp *exp) {
    // Need to check if the current running function is of the specified type
    if (exp->type == "VOID") {
        // Attempt to return a void expression from a value returning function
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (int i = GlobalSemanticStateHandler::symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symTabStack[i]->rows) {
            if (row->isFunc &&
                row->name == GlobalSemanticStateHandler::currentRunningFunctionScopeId) {
                // We found the current running function
                if (row->type.back() == exp->type) {
                    dataTag = exp->value;
                } else if (row->type.back() == "INT" && exp->type == "BYTE") {
                    // Allowing automatic cast from byte to int
                    dataTag = row->type.back();
                } else {
                    output::errorMismatch(yylineno);
                    exit(0);
                }
            }
        }
    }
}

Statement::Statement(Call *call) {
    dataTag = "function call";
}

Statement::Statement(Variable *id, Exp *exp) {
    if (!GlobalSemanticStateHandler::isDeclared(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }

    // Searching for the variable in the symtab
    for (int i = GlobalSemanticStateHandler::symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symTabStack[i]->rows) {
            if (!row->isFunc && row->name == id->value) {
                // We found the desired variable
                if ((row->type.back() == exp->type) || (row->type.back() == "INT" && exp->type == "BYTE")) {
                    dataTag = row->type.back();
                }
            }
        }
    }
}

Statement::Statement(Type *t, Variable *id, Exp *exp) {
    if (GlobalSemanticStateHandler::isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    if ((t->value == exp->type) || (t->value == "INT" && exp->type == "BYTE")) {
        dataTag = t->value;
        // Creating a new variable on the stack will cause the next one to have a higher offset
        int offset = GlobalSemanticStateHandler::offsetStack.back()++;
        vector<string> varType = {t->value};
        shared_ptr<SymbolTableRow> nVar = std::make_shared<SymbolTableRow>(id->value, varType, offset, false);
        GlobalSemanticStateHandler::symTabStack.back()->rows.push_back(nVar);
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Statement::Statement(Type *t, Variable *id) {
    if (GlobalSemanticStateHandler::isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    // Creating a new variable on the stack will cause the next one to have a higher offset
    int offset = GlobalSemanticStateHandler::offsetStack.back()++;
    vector<string> varType = {t->value};
    shared_ptr<SymbolTableRow> nVar = std::make_shared<SymbolTableRow>(id->value, varType, offset, false);
    GlobalSemanticStateHandler::symTabStack.back()->rows.push_back(nVar);
    dataTag = t->value;
}

Statement::Statement(Statements *states) {
    dataTag = "statement block";
}

Statement::Statement(Exp *exp, CaseList *cList) {
    // Need to check that exp is a number (int,byte) and that all case decl in caselist are int or byte
    GlobalSemanticStateHandler::enterSwitchState();

    if (exp->type != "INT" && exp->type != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (auto &i : cList->cases) {
        if (i->value != "INT" && i->value != "BYTE") {
            output::errorMismatch(yylineno);
            exit(0);
        }
    }

    dataTag = "switch block";
}


Statements::Statements(Statement *state) { }

Statements::Statements(Statements *states, Statement *state) { }


CaseDecl::CaseDecl(Exp *num, Statements *states) {
    if (num->type != "INT" && num->type != "BYTE") {
        //todo if (num->value != "INT" && num->value != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    value = num->type;
}


CaseList::CaseList(CaseDecl *cDec, CaseList *cList) {
    cases = vector<CaseDecl *>(cList->cases);
    cases.push_back(cDec);
    value = "case list";
}

CaseList::CaseList(CaseDecl *cDec) {
    cases.push_back(cDec);
    value = "case list";
}

CaseList::CaseList(Statements *states) { }


FormalDecl::FormalDecl(Type *t, Variable *id) : Variable(id->value), type(t->value) { }


FormalsList::FormalsList(FormalDecl *formal) {
    formals.insert(formals.begin(), formal);
}

FormalsList::FormalsList(FormalDecl *formal, FormalsList *fList) {
    formals = vector<FormalDecl *>(fList->formals);
    formals.insert(formals.begin(), formal);
}


Formals::Formals() = default;

Formals::Formals(FormalsList *formList) {
    formals = vector<FormalDecl *>(formList->formals);
}


FuncDecl::FuncDecl(RetType *rType, Variable *id, Formals *funcParams) {
    if (GlobalSemanticStateHandler::isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    for (unsigned int i = 0; i < funcParams->formals.size(); ++i) {
        if (GlobalSemanticStateHandler::isDeclared(funcParams->formals[i]->value) ||
            funcParams->formals[i]->value == id->value) {
            // Trying to shadow inside the function a variable that was already declared
            // Or trying to name a function with the same name as one of the function parameters
            output::errorDef(yylineno, id->value);
            exit(0);
        }

        for (unsigned int j = i + 1; j < funcParams->formals.size(); ++j) {
            if (funcParams->formals[i]->value == funcParams->formals[j]->value) {
                // Trying to declare a function where 2 parameters or more have the same name
                output::errorDef(yylineno, funcParams->formals[i]->value);
                exit(0);
            }
        }
    }

    // This is the name of the newly declared function
    value = id->value;
    if (funcParams->formals.size() != 0) {
        // Saving the types of all the different function parameters
        for (auto &formal : funcParams->formals) {
            type.push_back(formal->type);
        }
    } else {
        // The func has no input parameters, no need to add a specific parameter type
    }
    // Saving the return type of the function
    type.push_back(rType->value);

    // Adding the new function to the symTab
    shared_ptr<SymbolTableRow> nFunc = std::make_shared<SymbolTableRow>(value, type, 0, true);
    GlobalSemanticStateHandler::symTabStack.back()->rows.push_back(nFunc);
    GlobalSemanticStateHandler::currentRunningFunctionScopeId = value;
}


Funcs::Funcs() {
    if (strcmp(yytext, "") != 0) {
        output::errorSyn(yylineno);
        exit(0);
    }
}


Program::Program() : Variable("Program") {
    shared_ptr<SymbolTable> symTab = std::make_shared<SymbolTable>();
    shared_ptr<SymbolTableRow> printFunc =
            std::make_shared<SymbolTableRow>(SymbolTableRow("print", {"STRING", "VOID"}, 0, true));
    shared_ptr<SymbolTableRow> printiFunc =
            std::make_shared<SymbolTableRow>(SymbolTableRow("printi", {"INT", "VOID"}, 0, true));

    // Placing the print and printi function at the bottom of the global symbol table
    symTab->rows.push_back(printFunc);
    symTab->rows.push_back(printiFunc);
    // Placing the global symbol table at the bottom of the global symbol table stack
    GlobalSemanticStateHandler::symTabStack.push_back(symTab);
    // Placing the global symbol table at the bottom of the offset stack
    GlobalSemanticStateHandler::offsetStack.push_back(0);
}
