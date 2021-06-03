#include "SemanticsHandler.h"

extern char *yytext;

Variable::Variable() : value() { }

Variable::Variable(string str) {
    if (str == "void") {
        value = "VOID";
    } else if (str == "bool") {
        value = "BOOL";
    } else if (str == "byte") {
        value = "BYTE";
    } else if (str == "int") {
        value = "INT";
    } else {
        value = str;
    }
}

ostream &operator<<(ostream &os, const Variable &node) {
    os << "value: " << node.value;
    return os;
}


// TODO: edit
Program::Program() : Variable("Program") {
    shared_ptr<SymbolTable> symTab = std::make_shared<SymbolTable>();
    shared_ptr<SymbolTableRecord> printFunc =
            std::make_shared<SymbolTableRecord>(SymbolTableRecord("print", {"STRING", "VOID"}, 0, true));
    shared_ptr<SymbolTableRecord> printiFunc =
            std::make_shared<SymbolTableRecord>(SymbolTableRecord("printi", {"INT", "VOID"}, 0, true));

    // Placing the print and printi function at the bottom of the global symbol table
    symTab->records.push_back(printFunc);
    symTab->records.push_back(printiFunc);
    // Placing the global symbol table at the bottom of the global symbol table stack
    GlobalSemanticStateHandler::symbolTablesStack.push_back(symTab);
    // Placing the global symbol table at the bottom of the offset stack
    GlobalSemanticStateHandler::offsetStack.push_back(0);
}

// TODO: edit
Funcs::Funcs() {
    if (strcmp(yytext, "") != 0) {
        output::errorSyn(yylineno);
        exit(0);
    }
}

// TODO: edit
FuncDecl::FuncDecl(RetType *retType, Variable *id, Formals *funcParams) {
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
            functionParamsTypes.push_back(formal->paramType);
        }
    } else {
        // The func has no input parameters, no need to add a specific parameter type
    }

    // Saving the return type of the function
    functionParamsTypes.push_back(retType->value);

    // Adding the new function to the symTab
    shared_ptr<SymbolTableRecord> nFunc = std::make_shared<SymbolTableRecord>(value, functionParamsTypes, 0, true);
    GlobalSemanticStateHandler::symbolTablesStack.back()->records.push_back(nFunc);
    GlobalSemanticStateHandler::currentFunctionId = value;
}

// TODO: edit
Formals::Formals(FormalsList *formalList) {
    formals = vector<FormalDecl *>(formalList->formals);
}

// TODO: edit
FormalsList::FormalsList(FormalDecl *formalDecl) {
    formals.insert(formals.begin(), formalDecl);
}

// TODO: edit
FormalsList::FormalsList(FormalDecl *formalDecl, FormalsList *formalsList) {
    formals = vector<FormalDecl *>(formalsList->formals);
    formals.insert(formals.begin(), formalDecl);
}

// TODO: edit
Statement::Statement(Statements *statements) {
    dataTag = "statement block";
}

// TODO: edit
Statement::Statement(Type *type, Variable *id) {
    if (GlobalSemanticStateHandler::isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    // Creating a new variable on the stack will cause the next one to have a higher offset
    int offset = GlobalSemanticStateHandler::offsetStack.back()++;
    vector<string> varType = {type->value};
    shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);
    GlobalSemanticStateHandler::symbolTablesStack.back()->records.push_back(nVar);
    dataTag = type->value;
}

// TODO: edit
Statement::Statement(Type *type, Variable *id, Exp *exp) {
    if (GlobalSemanticStateHandler::isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    if ((type->value == exp->type) || (type->value == "INT" && exp->type == "BYTE")) {
        dataTag = type->value;
        // Creating a new variable on the stack will cause the next one to have a higher offset
        int offset = GlobalSemanticStateHandler::offsetStack.back()++;
        vector<string> varType = {type->value};
        shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);
        GlobalSemanticStateHandler::symbolTablesStack.back()->records.push_back(nVar);
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

// TODO: edit
Statement::Statement(Variable *id, Exp *exp) {
    if (!GlobalSemanticStateHandler::isDeclared(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }

    // Searching for the variable in the symtab
    for (int i = GlobalSemanticStateHandler::symbolTablesStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symbolTablesStack[i]->records) {
            if (!row->isFunc && row->name == id->value) {
                // We found the desired variable
                if ((row->type.back() == exp->type) || (row->type.back() == "INT" && exp->type == "BYTE")) {
                    dataTag = row->type.back();
                }
            }
        }
    }
}

// TODO: edit
Statement::Statement(Call *call) {
    dataTag = "function call";
}

// TODO: edit
Statement::Statement(const string &funcReturnType) {
    // Need to check if the current running function is of void type
    for (int i = GlobalSemanticStateHandler::symbolTablesStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symbolTablesStack[i]->records) {
            if (row->isFunc &&
                row->name == GlobalSemanticStateHandler::currentFunctionId) {
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

// TODO: edit
Statement::Statement(Exp *exp) {
    // Need to check if the current running function is of the specified type
    if (exp->type == "VOID") {
        // Attempt to return a void expression from a value returning function
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (int i = GlobalSemanticStateHandler::symbolTablesStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : GlobalSemanticStateHandler::symbolTablesStack[i]->records) {
            if (row->isFunc &&
                row->name == GlobalSemanticStateHandler::currentFunctionId) {
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

// TODO: edit
Statement::Statement(string type, Exp *exp) {
    if (exp->type != "BOOL") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    dataTag = "if if else while";
}

// TODO: edit
Statement::Statement(Variable *variable) {
    if (GlobalSemanticStateHandler::nestedLoopDepth == 0 && !GlobalSemanticStateHandler::isInSwitchState) {
        // We are not inside any loop, so a break or continue is illegal in this context
        if (variable->value == "break") {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        } else if (variable->value == "continue") {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        } else {
        }
    } else if (variable->value == "continue" && GlobalSemanticStateHandler::isInSwitchState) {
        output::errorUnexpectedContinue(yylineno);
        exit(0);
    }
    dataTag = "break or continue";
}

// TODO: edit
Statement::Statement(Exp *exp, CaseList *caseList) {
    // Need to check that exp is a number (int,byte) and that all case decl in caselist are int or byte
    GlobalSemanticStateHandler::enterSwitchState();

    if (exp->type != "INT" && exp->type != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (auto &i : caseList->caseDeclarations) {
        if (i->value != "INT" && i->value != "BYTE") {
            output::errorMismatch(yylineno);
            exit(0);
        }
    }

    dataTag = "switch block";
}

// TODO: edit
Call::Call(Variable *id) {
    for (auto &symTab : GlobalSemanticStateHandler::symbolTablesStack) {
        for (auto &row : symTab->records) {
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

// TODO: edit
Call::Call(Variable *id, ExpList *list) {
    for (auto &symTab : GlobalSemanticStateHandler::symbolTablesStack) {
        for (auto &row : symTab->records) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    // We found a declaration of a variable with the same name, illegal
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == list->expressionsList.size() + 1) {
                    // We found the right function, has the same name, it really is a function
                    // Now we need to check that the parameter types are correct between what the function accepts, and what was sent
                    for (unsigned int i = 0; i < list->expressionsList.size(); ++i) {
                        if (list->expressionsList[i].type == row->type[i]) {
                            // This parameter is of matching type so it is ok
                            continue;
                        } else if (list->expressionsList[i].type == "BYTE" && row->type[i] == "INT") {
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

ExpList::ExpList(Exp *exp) {
    expressionsList.insert(expressionsList.begin(), exp);
}

ExpList::ExpList(Exp *exp, ExpList *expList) {
    expressionsList = vector<Exp>(expList->expressionsList);
    expressionsList.insert(expressionsList.begin(), exp);
}

Exp::Exp(Exp *exp) {
    value = exp->value;
    type = exp->type;
    realBooleanValue = exp->realBooleanValue;
}

Exp::Exp(Exp *exp1, Variable *op, Exp *exp2, const string &taggedType) {
    bool isE1Num = exp1->type == "INT" || exp1->type == "BYTE";
    bool isE2Num = exp2->type == "INT" || exp2->type == "BYTE";
    bool isRelOp = taggedType == "EQ" || taggedType == "REL";
    bool isBinOp = taggedType == "PLUS_MINUS" || taggedType == "MUL_DIV";
    bool isBoolOp = taggedType == "AND" || taggedType == "OR";

    if (isE1Num && isE2Num) {
        if (isRelOp) {
            type = "BOOL";
        } else if (isBinOp) {
            if (exp1->type == "INT" || exp2->type == "INT") {
                type = "INT"; // automatic cast to the wider number type
            } else {
                type = "BYTE";
            }
        }
    } else if (exp1->type == "BOOL" && exp2->type == "BOOL") {
        type = "BOOL";

        if (isBoolOp) {
            if (op->value == "AND") {
                realBooleanValue = exp1->realBooleanValue && exp2->realBooleanValue;
            } else if (op->value == "OR") {
                realBooleanValue = exp1->realBooleanValue || exp2->realBooleanValue;
            }
        } else {
            // only a boolean operation is allowed between 2 boolean expressions
            output::errorMismatch(yylineno);
            exit(0);
        }
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Exp::Exp(Variable *variable) {
    // making sure the variable is declared
    if (!GlobalSemanticStateHandler::isDeclaredVariable(variable->value)) {
        output::errorUndef(yylineno, variable->value);
        exit(0);
    }

    // TODO: edit:
    // Need to save the type of the variable function as the type of the expression
    for (int i = GlobalSemanticStateHandler::symbolTablesStack.size() - 1; i >= 0; --i) {
        for (auto &row : GlobalSemanticStateHandler::symbolTablesStack[i]->records) {
            if (row->name == variable->value) {
                // We found the variable/func we wanted to use in the expression
                value = variable->value;
                // Getting the type of the variable, or the return type of the function
                type = row->type.back();
                return;
            }
        }
    }
}

Exp::Exp(Call *call) {
    value = call->value;
    type = call->value;
}

Exp::Exp(Variable *variable, string taggedType) : Variable::Variable(variable->value) {
    type = taggedType;

    if (type == "NUM") {
        type = "INT";
    } else if (type == "BYTE") {
        // check that size is legal for type BYTE
        if (stoi(variable->value) > 255) {
            output::errorByteTooLarge(yylineno, variable->value);
            exit(0);
        }
    } else if (type == "BOOL") {
        if (variable->value == "true") {
            realBooleanValue = true;
        } else {
            realBooleanValue = false;
        }
    }
}

Exp::Exp(Variable *variable, Exp *exp) {
    if (exp->type != "BOOL") {
        // not a boolean expression, can't apply NOT
        output::errorMismatch(yylineno);
        exit(0);
    }

    type = "BOOL";
    realBooleanValue = !realBooleanValue; // applying not
}

Exp::Exp(Exp *exp, string tag) {
    if (tag == "switch" && exp->type != "INT" && exp->type != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

CaseList::CaseList(CaseDecl *caseDecl, CaseList *caseList) : CaseList::CaseList() {
    caseDeclarations = vector<CaseDecl *>(caseList->caseDeclarations);
    caseDeclarations.push_back(caseDecl);
}

CaseList::CaseList(CaseDecl *caseDecl) : CaseList::CaseList() {
    caseDeclarations.push_back(caseDecl);
}

CaseDecl::CaseDecl(Exp *expression, Statements *statements) {
    if (expression->type != "INT" && expression->type != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    value = expression->type;
}

// GlobalSemanticStateHandler namespace

// global variables definitions:
vector<shared_ptr<SymbolTable>> GlobalSemanticStateHandler::symbolTablesStack;
vector<int> GlobalSemanticStateHandler::offsetStack;
int GlobalSemanticStateHandler::nestedLoopDepth = 0;
bool GlobalSemanticStateHandler::isInSwitchState = false;
string GlobalSemanticStateHandler::currentFunctionId;

// global functions definitions:

void GlobalSemanticStateHandler::openScope() {
    symbolTablesStack.push_back(make_shared<SymbolTable>());
    offsetStack.push_back(offsetStack.back());
}

// TODO: edit
void GlobalSemanticStateHandler::closeScope() {
    output::endScope();
    shared_ptr<SymbolTable> currentScope = symbolTablesStack.back();
    for (auto &row : currentScope->records) {
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

    currentScope->records.clear();
    symbolTablesStack.pop_back();
    offsetStack.pop_back();

}

void GlobalSemanticStateHandler::enterLoopState() {
    nestedLoopDepth++;
}

void GlobalSemanticStateHandler::exitLoopState() {
    nestedLoopDepth--;
}

void GlobalSemanticStateHandler::enterSwitchState() {
    isInSwitchState = true;
}

void GlobalSemanticStateHandler::exitSwitchState() {
    isInSwitchState = false;
}

void GlobalSemanticStateHandler::exitProgramFuncsState() {
    currentFunctionId = "";
}

// TODO: edit
void GlobalSemanticStateHandler::exitProgramRuntimeState() {
    shared_ptr<SymbolTable> globalScope = symbolTablesStack.front();
    bool mainFunc = false;
    for (auto &row : globalScope->records) {
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

// TODO: edit
void GlobalSemanticStateHandler::insertFunctionParameters(Formals *formals) {
    for (unsigned int i = 0; i < formals->formals.size(); ++i) {
        vector<string> nType = {formals->formals[i]->paramType};
        shared_ptr<SymbolTableRecord> nParameter = make_shared<SymbolTableRecord>(formals->formals[i]->value, nType, -i - 1, false);
        symbolTablesStack.back()->records.push_back(nParameter);
    }
}

// TODO: edit
bool GlobalSemanticStateHandler::isDeclaredVariable(const string &name) {
    for (int i = symbolTablesStack.size() - 1; i >= 0; --i) {
        for (auto &row : symbolTablesStack[i]->records) {
            if (row->name == name && !row->isFunc) {
                return true;
            }
        }
    }

    return false;
}

// TODO: edit
bool GlobalSemanticStateHandler::isDeclared(const string &name) {
    for (int i = symbolTablesStack.size() - 1; i >= 0; --i) {
        for (auto &row : symbolTablesStack[i]->records) {
            if (row->name == name) {
                return true;
            }
        }
    }

    return false;
}
