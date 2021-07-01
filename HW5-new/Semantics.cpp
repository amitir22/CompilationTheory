#include "Semantics.h"
#include "Registers.h"

#include "iostream"
#include <memory>
#include <cstring>

extern char *yytext;
int DEBUG = 0;
int switchId = 0;
int maxSwitch = 0;
Registers registerPool;
CodeBuffer &buffer = CodeBuffer::instance();
vector<shared_ptr<SymbolTable>> symTabStack;
vector<int> offsetStack;
vector<string> varTypes = {"VOID", "INT", "BYTE", "BOOL", "STRING"};

string currentRunningFunctionScopeId;
int currentRunningFunctionArgumentsNumber = 0;

void printVector(vector<string> vec) {
    for (auto &i : vec) {
        std::cout << i << " ";
    }
}

void printMessage(string message) {
    std::cout << message << std::endl;
}

// Converts a FanC return type, to a LLVM return type
string GetLLVMType(string type) {
    if (type == "VOID") {
        if (DEBUG) printMessage("void type");
        return "void";
    } else if (type == "BOOL") {
        if (DEBUG) printMessage("i1 type");
        return "i1";
    } else if (type == "BYTE") {
        if (DEBUG) printMessage("i8 type");
        return "i8";
    } else if (type == "STRING") {
        if (DEBUG) printMessage("i8 pointer type");
        return "i8*";
    } else return "i32";
}

string zeroExtendType(string originalReg, string llvmType) {
    string destReg = registerPool.GetNewRegister();
    buffer.emit("%" + destReg + " = zext " + llvmType + " %" + originalReg + " to i32");
    return destReg;
}

void printSymTabRow(shared_ptr<SymbolTableRecord> row) {
    std::cout << row->name << " | ";
    printVector(row->type);
    std::cout << " | " << row->offset << " | " << row->isFunc << std::endl;
}

void printSymTableStack() {
    std::cout << "Size of global symbol table stack is: " << symTabStack.size() << std::endl;
    std::cout << "id | parameter types | offset | is func" << std::endl;
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->records) {
            printSymTabRow(row);
        }
    }
}

int loopCounter = 0;
int switchCounter = 0;

void enterSwitchState() {
    if (DEBUG) printMessage("Entering Switch block");
    switchCounter++;
    maxSwitch++;
    switchId = maxSwitch;
}

void exitSwitchState() {
    // (so we can jump outside without performing other case blocks)
    if (DEBUG) printMessage("Exiting Switch block");
    switchCounter--;
    if (switchCounter != 0) {
        switchId--;
    }
}

void enterLoop() {
    loopCounter++;
}

void exitLoop(N *first, P *second, Statement *statement) {
    loopCounter--;
    int loc = buffer.emit("br label @");
    string label = buffer.genLabel();
    if (DEBUG) {
        printMessage("Backpatching jumps back into another loop iteration");
    }
    // backpatch the jump back to the loop condition evaluation
    buffer.bpatch(buffer.makelist({first->loc, FIRST}), first->instruction);
    // backpatch the jump inside the loop if the condition is still true
    buffer.bpatch(buffer.makelist({second->loc, FIRST}), second->instruction);
    if (DEBUG) {
        printMessage("Backpatching jumps outside of loops");
    }
    // backpatch the jump outside the loop if the condition is false
    buffer.bpatch(buffer.makelist({second->loc, SECOND}), label);
    // backpatch the unconditional jump at the end of the loop to the condition evaluation
    buffer.bpatch(buffer.makelist({loc, FIRST}), first->instruction);
    if (!statement->continue_list.empty()) {
        // backpatch the jump to the condition evaluation in case of a continue
        buffer.bpatch(statement->continue_list, first->instruction);
    }
    if (!statement->break_list.empty()) {
        // backpatch the jump outside the loop in case of a break
        buffer.bpatch(statement->break_list, label);
    }
}

void exitProgramFuncs(RetType *ret) {
    if (ret->value == "VOID") {
        buffer.emit("ret void");
    } else {
        if (DEBUG) {
            printMessage("returning from a non void function");
        }
        string funcReturnType = GetLLVMType(ret->value);
        buffer.emit("ret " + funcReturnType + " 0");
    }
    buffer.emit("}");
    currentRunningFunctionScopeId = "";
    currentRunningFunctionArgumentsNumber = 0;
}

void exitProgramRuntimeState() {
    if (DEBUG) {
        printMessage("I am entering program runtime");
    }
    shared_ptr<SymbolTable> globalScope = symTabStack.front();
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
    buffer.printGlobalBuffer();
    buffer.printCodeBuffer();
    if (DEBUG) printMessage("I am exiting program runtime");
}

void openScope() {
    if (DEBUG) printMessage("creating scope");
    shared_ptr<SymbolTable> nScope = make_shared<SymbolTable>();
    symTabStack.push_back(nScope);
    offsetStack.push_back(offsetStack.back());
    if (DEBUG) printMessage("done creating");
}

void closeScope() {
    //output::endScope();
    shared_ptr<SymbolTable> currentScope = symTabStack.back();
    for (auto &row : currentScope->records) {
        if (!row->isFunc) {
            // Print a normal variable
            //output::printID(row->name, row->offset, row->type[0]);
        } else {
            string funcReturnType = row->type.back();
            // Taking out the return type from the vector for easy printing
            row->type.pop_back();
//            output::printID(row->name, row->offset,
//                            output::makeFunctionType(funcReturnType, row->type));
        }
    }

    currentScope->records.clear();
    symTabStack.pop_back();
    offsetStack.pop_back();

}

bool isDeclared(const string &name) {
    if (DEBUG) {
        printMessage("In is declared for");
        printMessage(name);
        printSymTableStack();
    }
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->records) {
            if (row->name == name) {
                if (DEBUG) printMessage("found id");
                return true;
            }
        }
    }
    if (DEBUG) printMessage("can't find id");
    return false;
}

bool isDeclaredVariable(const string &name) {
    if (DEBUG) {
        printMessage("In is declared for");
        printMessage(name);
        printSymTableStack();
    }
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->records) {
            if (row->name == name && !row->isFunc) {
                if (DEBUG) printMessage("found id");
                return true;
            }
        }
    }
    if (DEBUG) printMessage("can't find id");
    return false;
}

SymbolTableRecord::SymbolTableRecord(string name, vector<string> type, int offset, bool isFunc) : name(std::move(name)), type(std::move(type)),
                                                                                            offset(offset), isFunc(isFunc) {

}

Variable::Variable(string str) : register_name(""), value(), instruction("") {
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

    if (value == "OpenCase") {
        instruction = DeclareCaseLabel();
    }
}

Variable::Variable() {
    value = "";
    instruction = "";
    register_name = "";
}

ostream &operator<<(ostream &os, const Variable &node) {
    os << "value: " << node.value;
    return os;
}

Program::Program() : Variable("Program") {
    shared_ptr<SymbolTable> symTab = std::make_shared<SymbolTable>();
    shared_ptr<SymbolTableRecord> printFunc = std::make_shared<SymbolTableRecord>(SymbolTableRecord("print", {"STRING", "VOID"}, 0, true));
    shared_ptr<SymbolTableRecord> printiFunc = std::make_shared<SymbolTableRecord>(SymbolTableRecord("printi", {"INT", "VOID"}, 0, true));
    // Placing the print and printi function at the bottom of the global symbol table
    symTab->records.push_back(printFunc);
    symTab->records.push_back(printiFunc);
    // Placing the global symbol table at the bottom of the global symbol table stack
    if (DEBUG) printMessage("pushing global symtab");
    symTabStack.push_back(symTab);
    // Placing the global symbol table at the bottom of the offset stack
    offsetStack.push_back(0);

    // Declaring staff functions in LLVM code
    if (DEBUG) printMessage("declaring staff func headers");
    buffer.emitGlobal("declare i32 @printf(i8*, ...)");
    buffer.emitGlobal("declare void @exit(i32)");

    // Declaring staff int specifier
    if (DEBUG) printMessage("declaring const strings");
    buffer.emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    // Declaring staff string specifier
    buffer.emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    buffer.emitGlobal("@ThrowZeroException = constant [23 x i8] c\"Error division by zero\\00\"");

    // declaration of global printi function
    if (DEBUG) printMessage("declaring staff printi");
    buffer.emitGlobal("define void @printi(i32) {");
    buffer.emitGlobal("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
    buffer.emitGlobal("ret void");
    buffer.emitGlobal("}");

    // declaration of global print function
    if (DEBUG) printMessage("declaring staff print");
    buffer.emitGlobal("define void @print(i8*) {");
    buffer.emitGlobal("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
    buffer.emitGlobal("ret void");
    buffer.emitGlobal("}");
}


Type::Type(Variable *type) : Variable(type->value) {

}


FormalsList::FormalsList(FormalDecl *formalDecl) {
    formals.insert(formals.begin(), formalDecl);
}

FormalsList::FormalsList(FormalDecl *formalDecl, FormalsList *formalList) {
    formals = vector<FormalDecl *>(formalList->formals);
    formals.insert(formals.begin(), formalDecl);
}

Formals::Formals(FormalsList *formalList) {
    formals = vector<FormalDecl *>(formalList->formals);
}

FuncDecl::FuncDecl(RetType *rType, Variable *id, Formals *funcParams) {
    if (DEBUG) printMessage("I am in func decl");
    if (isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    for (unsigned int i = 0; i < funcParams->formals.size(); ++i) {
        if (isDeclared(funcParams->formals[i]->value) || funcParams->formals[i]->value == id->value) {
            // Trying to shadow inside the function a variable that was already declared
            // Or trying to name a function with the same name as one of the function parameters
            if (DEBUG) printMessage("attempt to shadow parameter");
            output::errorDef(yylineno, funcParams->formals[i]->value);
            exit(0);
        }

        for (unsigned int j = i + 1; j < funcParams->formals.size(); ++j) {
            if (funcParams->formals[i]->value == funcParams->formals[j]->value) {
                // Trying to declare a function where 2 parameters or more have the same name
                if (DEBUG) printMessage("attempt to redeclare");
                output::errorDef(yylineno, funcParams->formals[i]->value);
                exit(0);
            }
        }
    }

    // This is the name of the newly declared function
    value = id->value;
    if (funcParams->formals.size() != 0) {
        // Saving the types of all the different function parameters
        if (DEBUG) printMessage("pushing formals");
        for (auto &formal : funcParams->formals) {
            functionParamsTypes.push_back(formal->paramType);
        }
    } else {
        // The func has no input parameters, no need to add a specific parameter type
    }
    // Saving the return type of the function
    functionParamsTypes.push_back(rType->value);

    // Adding the new function to the symTab
    shared_ptr<SymbolTableRecord> nFunc = std::make_shared<SymbolTableRecord>(value, functionParamsTypes, 0, true);
    symTabStack.back()->records.push_back(nFunc);
    currentRunningFunctionScopeId = value;
    currentRunningFunctionArgumentsNumber = funcParams->formals.size();
    string funcArgumentString = "(";
    for (int i = 0; i < funcParams->formals.size(); ++i) {
        // The function has parameters which need to be printed in the LLVM function declaration
        funcArgumentString += GetLLVMType(funcParams->formals[i]->paramType);
        if (i != funcParams->formals.size() - 1) {
            funcArgumentString += ',';
        }
    }
    funcArgumentString.push_back(')');
    string funcReturnType = GetLLVMType(rType->value);
    buffer.emit("define " + funcReturnType + " @" + value + funcArgumentString + " {");

    // Declaring function arguments and stack, max number of variables in a single function is 50
    buffer.emit("%stack = alloca [50 x i32]");
    buffer.emit("%args = alloca [" + to_string(funcParams->formals.size()) + " x i32]");

    for (int i = 0; i < funcParams->formals.size(); ++i) {
        // creating registers for all func parameters
        if (DEBUG) {
            printMessage("creating registers for all func parameters");
        }
        string ptrRegister = registerPool.GetNewRegister();
        buffer.emit("%" + ptrRegister + " = getelementptr [" + to_string(funcParams->formals.size()) + " x i32], [" +
                    to_string(funcParams->formals.size()) + " x i32]* %args, i32 0, i32 " + to_string(currentRunningFunctionArgumentsNumber - i - 1));
        string dataRegister = to_string(i);
        string funcArgumentType = GetLLVMType(funcParams->formals[i]->paramType);
        if (funcArgumentType != "i32") {
            // need to zero extend
            if (DEBUG) {
                printMessage("zero extending func parameter register");
            }
            dataRegister = registerPool.GetNewRegister();
            buffer.emit("%" + dataRegister + " = zext " + funcArgumentType + " %" + to_string(i) + " to i32");
        }
        buffer.emit("store i32 %" + dataRegister + ", i32* %" + ptrRegister);
    }
    if (DEBUG) printMessage("exiting func decl");
}

Call::Call(Variable *id) {
    for (auto &symTab : symTabStack) {
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
                    string funcName = id->value;
                    string returnTypeFromFunc = GetLLVMType(value);
                    register_name = registerPool.GetNewRegister();
                    if (returnTypeFromFunc == "void") {
                        // Calling a void function
                        if (DEBUG) printMessage("calling a void func");
                        buffer.emit("call " + returnTypeFromFunc + " @" + funcName + " ()");
                    } else {
                        // Calling a non void function and saving the result in the register
                        if (DEBUG) printMessage("calling a non void func");
                        buffer.emit("%" + register_name + " = call " + returnTypeFromFunc + " @" + funcName + " ()");
                    }
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
    if (DEBUG) printMessage("no function found");
    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}

Call::Call(Variable *id, ExpList *list) {
    if (DEBUG) printMessage("in call id list");
    for (auto &symTab : symTabStack) {
        for (auto &row : symTab->records) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    // We found a declaration of a variable with the same name, illegal
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == list->expressionsList.size() + 1) {
                    // We found the right function, has the same name, it really is a function
                    // Now we need to check that the parameter types are correct between what the function accepts, and what was sent
                    // Need to generate a string with arguments to send to the function
                    string funcArgs = "(";
                    for (unsigned int i = 0; i < list->expressionsList.size(); ++i) {
                        if (list->expressionsList[i].type == row->type[i]) {
                            // This parameter is of matching type so it is ok
                            funcArgs += GetLLVMType(list->expressionsList[i].type) + " %" + list->expressionsList[i].register_name + ",";
                            continue;
                        } else if (list->expressionsList[i].type == "BYTE" && row->type[i] == "INT") {
                            // The function receives int as a parameter, in this instance a byte was sent, need to cast from BYTE to INT
                            if (DEBUG) printMessage("casting BYTE to INT");
                            string dataRegister = registerPool.GetNewRegister();
                            buffer.emit("%" + dataRegister + " = zext i8 %" + list->expressionsList[i].register_name + " to i32");
                            funcArgs += GetLLVMType("INT") + " %" + dataRegister + ",";
                            continue;
                        }
                        // Removing the return type of the function so we have an easy list of requested parameters to print
                        row->type.pop_back();
                        output::errorPrototypeMismatch(yylineno, id->value, row->type);
                        exit(0);
                    }
                    // Saving the type of the function call return value
                    // removing the last comma (,) and replacing with closing bracket
                    funcArgs.back() = ')';
                    value = row->type.back();
                    // Need to actually call the function now
                    string funcName = id->value;
                    string returnTypeFromFunc = GetLLVMType(value);
                    register_name = registerPool.GetNewRegister();
                    if (returnTypeFromFunc == "void") {
                        if (DEBUG) printMessage("calling void function");
                        // Calling a void function
                        buffer.emit("call " + returnTypeFromFunc + " @" + funcName + " " + funcArgs);
                    } else {
                        // Calling a non void function and saving the result in the register
                        if (DEBUG) printMessage("calling non void function, saving result");
                        buffer.emit("%" + register_name + " = call " + returnTypeFromFunc + " @" + funcName + " " + funcArgs);
                    }
                    // Creating a code section to return to after the function call has ended.
                    int loc = buffer.emit("br label @");
                    instruction = buffer.genLabel();
                    buffer.bpatch(buffer.makelist({loc, FIRST}), instruction);
                    return;
                } else {
                    // The number of parameters we received does not match the number the function takes as arguments
                    // Removing the return type of the function so we have an easy list of requested parameters to print
                    if (DEBUG) printMessage("error with number of function parameters sent");
                    row->type.pop_back();
                    output::errorPrototypeMismatch(yylineno, id->value, row->type);
                    exit(0);
                }
            }
        }
    }
    // We didn't find a declaration of the desired function
    if (DEBUG) printMessage("no function found");
    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}

Exp::Exp(Call *call) {
    // Need to just take the return value of the function and use it as the return type of the expression
    if (DEBUG) printMessage("in exp call");
    register_name = call->register_name;
    value = call->value;
    type = call->value;
    instruction = call->instruction;
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
}

Exp::Exp(Variable *id) {
    // Need to make sure that the variable/func we want to use is declared
    if (DEBUG) {
        printMessage("creating exp from id:");
        printMessage(id->value);
    }
    if (!isDeclaredVariable(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();

    // Need to save the type of the variable function as the type of the expression
    for (int i = symTabStack.size() - 1; i >= 0; --i) {
        for (auto &row : symTabStack[i]->records) {
            if (row->name == id->value) {
                if (DEBUG) {
                    printMessage("found a variable with name in symtab:");
                    printMessage(row->name);
                }
                // We found the variable/func we wanted to use in the expression
                value = id->value;
                // Getting the type of the variable, or the return type of the function
                type = row->type.back();
                register_name = loadVariableFromSymTab(row->offset, type);
                return;
            }
        }
    }
}

Exp::Exp(Variable *variable, Exp *exp) {
    if (exp->type != "BOOL") {
        // This is not a boolean expression, can't apply NOT
        output::errorMismatch(yylineno);
        exit(0);
    }
    type = "BOOL";
    register_name = registerPool.GetNewRegister();
    // reversing the value of exp because we apply NOT
    buffer.emit("%" + register_name + " = add i1 1, %" + exp->register_name);
    actualBoolean = !actualBoolean;
    // we reversed the logical boolean meaning, so we are reversing the true and false lists
    false_list = exp->true_list;
    true_list = exp->false_list;
}

Exp::Exp(Variable *variable, string taggedType) : Variable(variable->value) {
    if (DEBUG) {
        printMessage("in for num byte");
        printMessage(variable->value);
        printMessage("tagged type:");
        printMessage(taggedType);

    }
    // In this function after figuring out the type of the terminal we:
    // 1. create a new register for this data declaration
    // 2. Set the value of the new register to the value of the terminal
    type = taggedType;
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
    if (taggedType == "CASE_NUM") {
        type = "INT";
    }
    if (taggedType == "NUM") {
        type = "INT";
        register_name = registerPool.GetNewRegister();
        buffer.emit("%" + register_name + " = add i32 0," + variable->value);
    }
    if (type == "BYTE") {
        // Need to check that BYTE size is legal
        if (stoi(variable->value) > 255) {
            // Byte is too large
            output::errorByteTooLarge(yylineno, variable->value);
            exit(0);
        }
        register_name = registerPool.GetNewRegister();
        buffer.emit("%" + register_name + " = add i8 0," + variable->value);
    }
    if (type == "BOOL") {
        register_name = registerPool.GetNewRegister();
        if (variable->value == "true") {
            actualBoolean = true;
            buffer.emit("%" + register_name + " = add i1 0,1");
        } else {
            actualBoolean = false;
            buffer.emit("%" + register_name + " = add i1 0,0");
        }
    }
    // If we reached here, the type is STRING
    if (type == "STRING") {
        register_name = registerPool.GetNewRegister();
//    buffer.emitGlobal(terminal->value);
        // replacing the default string null terminator with an explicit null terminator for LLVM
//    terminal->value[terminal->value.size()] = '\0';
//    terminal->value.push_back('\00');
        int size = variable->value.size();
        string sub = variable->value.substr(1, variable->value.length() - 2);
        string num_size = to_string(variable->value.size() - 1);
//    buffer.emitGlobal("@" + register_name + "= constant [" + to_string(size) + " x i8] c\"" + terminal->value + "\\00\"");
        buffer.emitGlobal(
                "@" + register_name + " = constant [" + num_size + " x i8] c\"" + variable->value.substr(1, variable->value.length() - 2) + "\\00\"");
//        buffer.emit("%" + register_name + " = getelementptr [" + to_string(size) + " x i8], [" + to_string(size) + " x i8]* @" + register_name + ", i8 0, i8 0");
        buffer.emit("%" + register_name + " = getelementptr [" + num_size + " x i8], [" + num_size + " x i8]* @" + register_name + ", i8 0, i8 0");
    }
    if (DEBUG) {
        printMessage("now tagged as:");
        printMessage(type);
    }
}

Exp::Exp(Exp *ex) {
    if (DEBUG) {
        printMessage("=====exp ex=====");
        printMessage(ex->value);
        printMessage(ex->type);
    }
//    if (ex->type != "BOOL") {
//        output::errorMismatch(yylineno);
//        exit(0);
//    }
    value = ex->value;
    type = ex->type;
    actualBoolean = ex->actualBoolean;
    register_name = ex->register_name;
    instruction = ex->instruction;
    true_list = ex->true_list;
    false_list = ex->false_list;
}

// for Exp RELOP, MUL, DIV, ADD, SUB, OR, AND Exp
Exp::Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedType, P *leftInstruction) {
    register_name = registerPool.GetNewRegister();
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
    string end = "";
    bool bothByte = false;
    // Need to check the type of the expressions on each side, to make sure that a logical operator (AND/OR) is used on a boolean type
    if ((e1->type == "INT" || e1->type == "BYTE") && (e2->type == "INT" || e2->type == "BYTE")) {
        if (taggedType == "EQ_NEQ_RELOP" || taggedType == "REL_RELOP") {
            // This is a boolean operation performed between 2 numbers (>=,<=,>,<,!=,==)
            type = "BOOL";
            string llvmSize = "i8";
            if (e1->type == "INT" || e2->type == "INT") {
                llvmSize = "i32";
            }
            string relop;
            if (op->value == "==") {
                relop = "eq";
            } else if (op->value == "!=") {
                relop = "ne";
            } else if (op->value == ">") {
                relop = "sgt";
                if (llvmSize == "i8") {
                    relop = "ugt";
                }
            } else if (op->value == "<") {
                relop = "slt";
                if (llvmSize == "i8") {
                    relop = "ult";
                }
            } else if (op->value == ">=") {
                relop = "sge";
                if (llvmSize == "i8") {
                    relop = "uge";
                }
            } else if (op->value == "<=") {
                relop = "sle";
                if (llvmSize == "i8") {
                    relop = "ule";
                }
            }
            string leftRegister = e1->register_name;
            string rightRegister = e2->register_name;
            if (DEBUG) {
                printMessage("Checking the need for register extension based on type");
            }
            if (llvmSize == "i32") {
                if (e1->type == "BYTE") {
                    if (DEBUG) {
                        printMessage("zero extending left side");
                    }
                    // This is a byte register, need to zero extend to i32 for int operations
                    leftRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                }
                if (e2->type == "BYTE") {
                    if (DEBUG) {
                        printMessage("zero extending right side");
                    }
                    // This is a byte register, need to zero extend to i32 for int operations
                    rightRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
                }
            }
            buffer.emit("%" + register_name + " = icmp " + relop + " " + llvmSize + " %" + leftRegister + " , %" + rightRegister);
            if (e2->instruction != "") {
                end = e2->instruction;
            } else {
                end = e1->instruction;
            }
        } else if (taggedType == "PLUS_MINUS" || taggedType == "MUL_DIV") {
            // This is an arithmetic operation between two numbers
            if (e1->type == "BYTE" && e2->type == "BYTE") {
                bothByte = true;
            }
            register_name = registerPool.GetNewRegister();
            string operation;
            string leftRegister = e1->register_name;
            string rightRegister = e2->register_name;
            type = "BYTE";
            string llvmSize = "i8";
            if (e1->type == "INT" || e2->type == "INT") {
                type = "INT";
                llvmSize = "i32";
            }
            if (e1->type != e2->type) {
                if (e1->type == "BYTE") {
                    leftRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                }
                if (e2->type == "BYTE") {
                    rightRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
                }
            }
            if (op->value == "+") {
                operation = "add";
            } else if (op->value == "-") {
                operation = "sub";
            } else if (op->value == "*") {
                operation = "mul";
            } else if (op->value == "/") {
                string condition = registerPool.GetNewRegister();
                // Division will always be by integers
                if (bothByte) {
                    leftRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                    rightRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
                }
                // Checking if the right hand side is 0 and jumping accordingly
                buffer.emit("%" + condition + " = icmp eq i32 %" + rightRegister + ", 0");
                int zeroDivisionBranchCheck = buffer.emit("br i1 %" + condition + ", label @, label @");
                if (DEBUG) {
                    printMessage("Printing the zero division exception handler case");
                }
                string zeroDivisionCaseLabel = buffer.genLabel();
                string zeroDivisionExceptionReg = registerPool.GetNewRegister();
                if (DEBUG) {
                    printMessage("arrived here");
                }
                buffer.emit("%" + zeroDivisionExceptionReg + " = getelementptr [23 x i8], [23 x i8]* @ThrowZeroException, i32 0, i32 0");
                buffer.emit("call void @print(i8* %" + zeroDivisionExceptionReg + ")");
                buffer.emit("call void @exit(i32 0)");
                int normalDivisionCaseLabelLoc = buffer.emit("br label @");
                if (DEBUG) {
                    printMessage("Printing the normal division handler case");
                }
                string normalDivisionBranchLabel = buffer.genLabel();
                // Backpatching the check, so that it jumps to the handler for zero division
                buffer.bpatch(buffer.makelist({zeroDivisionBranchCheck, FIRST}), zeroDivisionCaseLabel);
                // Backpatching the check, so that is jumps to the normal divison handler
                buffer.bpatch(buffer.makelist({zeroDivisionBranchCheck, SECOND}), normalDivisionBranchLabel);
                if (DEBUG) {
                    printMessage("arrived here, no nullptr");
                }
                // Backpatching the emitted label, so it jumps to the correct place
                buffer.bpatch(buffer.makelist({normalDivisionCaseLabelLoc, FIRST}), normalDivisionBranchLabel);
                llvmSize = "i32";
                operation = "sdiv";
                end = normalDivisionBranchLabel;
            }
//            if (llvmSize == "i32") {
//                if (e1->type == "BYTE") {
//                    leftRegister = registerPool.GetNewRegister();
//                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
//                }
//                if (e2->type == "BYTE") {
//                    rightRegister = registerPool.GetNewRegister();
//                    buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
//                }
//            }


//            if (e1->type == "BYTE") {
//                leftRegister = registerPool.GetNewRegister();
//                buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
//            }
//            if (e2->type == "BYTE") {
//                rightRegister = registerPool.GetNewRegister();
//                buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
//            }

            buffer.emit("%" + register_name + " = " + operation + " " + llvmSize + " %" + leftRegister + ", %" + rightRegister);
            if (operation == "sdiv" && bothByte) {
                // Need to trunc the result back to i8
                string tempReg = registerPool.GetNewRegister();
                buffer.emit("%" + tempReg + " = trunc i32 %" + register_name + " to i8");
                register_name = tempReg;
            }
        }
    } else if (e1->type == "BOOL" && e2->type == "BOOL") {
        // Both operands are boolean so this should be a boolean operation
        type = "BOOL";
        if (taggedType == "AND" || taggedType == "OR") {
            string boolop;
            if (e2->instruction != "") {
                instruction = e2->instruction;
            } else {
                instruction = leftInstruction->instruction;
            }
            if (op->value == "and") {
                int firstCheckIsFalseBranchJump = buffer.emit("br label @");
                string leftIsFalseLabel = buffer.genLabel();
                if (DEBUG) printMessage("generating non short circuit labels");
                int secondCheckIsFalseLabel = buffer.emit("br label @");
                end = buffer.genLabel();
                // bpatch, so that if the first boolean expression in the AND is false, we give the register value 0 (false)
                // otherwise we take the value of e2.register_name
                if (DEBUG) printMessage("emitting phi instruction");
                buffer.emit("%" + register_name + " = phi i1 [%" + e2->register_name + ", %" + instruction + "],[0, %" + leftIsFalseLabel + "]");
                // backpatch the jump to check the second condition in the AND if it's true
                buffer.bpatch(buffer.makelist({leftInstruction->loc, FIRST}), leftInstruction->instruction);
                // backpatch the jump outside of the expression if the first condition is false
                if (DEBUG) printMessage("backpatching branch labels");
                buffer.bpatch(buffer.makelist({leftInstruction->loc, SECOND}), leftIsFalseLabel);
                // backpatch the jump outside of the expression to after the if block
                buffer.bpatch(buffer.makelist({firstCheckIsFalseBranchJump, FIRST}), end);
                buffer.bpatch(buffer.makelist({secondCheckIsFalseLabel, FIRST}), end);
                if (e1->actualBoolean && e2->actualBoolean) {
                    actualBoolean = true;
                } else {
                    actualBoolean = false;
                }
            } else if (op->value == "or") {
                if (DEBUG) printMessage("handling or case");
                int firstCheckIsTrueBranchJump = buffer.emit("br label @");
                string leftIsTrueLabel = buffer.genLabel();
                int secondCheckIsTrueLabel = buffer.emit("br label @");
                end = buffer.genLabel();
                // bpatch, so that if the first boolean expression in the OR is true, we give the register value 1 (true)
                // otherwise we take the value of e2.register_name (either true or false, but still the correct value)
                if (DEBUG) printMessage("emitting phi instruction");
                buffer.emit("%" + register_name + " = phi i1 [%" + e2->register_name + ", %" + instruction + "],[1, %" + leftIsTrueLabel + "]");
                // backpatch the jump to outside if the if, in case the first expression is true
                buffer.bpatch(buffer.makelist({leftInstruction->loc, FIRST}), leftIsTrueLabel);
                // backpatch the jump to the second expression of the if, in case the first expression is false
                if (DEBUG) printMessage("backpatching branch labels");
                buffer.bpatch(buffer.makelist({leftInstruction->loc, SECOND}), leftInstruction->instruction);
                // backpatch the jump outside of the expression to after the if block
                buffer.bpatch(buffer.makelist({firstCheckIsTrueBranchJump, FIRST}), end);
                buffer.bpatch(buffer.makelist({secondCheckIsTrueLabel, FIRST}), end);
                if (e1->actualBoolean || e2->actualBoolean) {
                    actualBoolean = true;
                } else {
                    actualBoolean = false;
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
    if (end != "") {
        instruction = end;
    }
}

Exp::Exp(Exp *e1, string tag, N *label) {
    if (tag == "switch" && (e1->type != "INT" && e1->type != "BYTE")) {
        output::errorMismatch(yylineno);
        exit(0);
    }
    if (label) {
        buffer.bpatch(buffer.makelist(pair<int, BranchLabelIndex>(label->loc, FIRST)), label->instruction);
    }
    //buffer.emit("I am using the special constructor");
    value = e1->value;
    type = e1->type;
    actualBoolean = e1->actualBoolean;
    register_name = e1->register_name;
    instruction = e1->instruction;
    if (tag != "switch") {
        int loc = buffer.emit("br i1 %" + register_name + ", label @, label @");
        true_list = buffer.makelist(pair<int, BranchLabelIndex>(loc, FIRST));
        false_list = buffer.makelist(pair<int, BranchLabelIndex>(loc, SECOND));
    } else {
        true_list = vector<pair<int, BranchLabelIndex>>();
        false_list = vector<pair<int, BranchLabelIndex>>();
    }
    if (tag == "switch") {
        string switchCheckerLabelName = "label_switch_" + to_string(switchId);
        buffer.emit("br label %" + switchCheckerLabelName);
    }
}

string loadVariableFromSymTab(int offset, string type) {
    string reg = registerPool.GetNewRegister();
    string ptrRegister = registerPool.GetNewRegister();
    if (offset >= 0) {
        if (DEBUG) {
            printMessage("Loading variable declared inside function");
        }
        // this variable is declared inside the function, and not part of the function parameters
        buffer.emit("%" + ptrRegister + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
    } else if (offset < 0 && currentRunningFunctionArgumentsNumber > 0) {
        if (DEBUG) {
            printMessage("Loading function parameter");
        }
        // this variable is one of the function parameters
        buffer.emit("%" + ptrRegister + " = getelementptr [" + to_string(currentRunningFunctionArgumentsNumber) + " x i32], [" +
                    to_string(currentRunningFunctionArgumentsNumber) + " x i32]* %args, i32 0, i32 " +
                    to_string(currentRunningFunctionArgumentsNumber + offset));
    } else {
        // The requested variable is at a negative offset, but there are no function parameters, this is an error
        if (DEBUG) {
            printMessage("SHOULDN'T GET HERE");
        }
    }
    // load the value pointed by the pointer, into the register
    buffer.emit("%" + reg + " = load i32, i32* %" + ptrRegister);
    string varType = GetLLVMType(type);
    string register_name = reg;
    if (varType != "i32") {
        if (DEBUG) {
            printMessage("truncating requested register width to correct size");
        }
        // Shortening the new register to it's correct width
        register_name = registerPool.GetNewRegister();
        buffer.emit("%" + register_name + " = trunc i32 %" + reg + " to " + varType);
    }
    return register_name;
}

ExpList::ExpList(Exp *exp) {
    expressionsList.insert(expressionsList.begin(), exp);
}

ExpList::ExpList(Exp *exp, ExpList *expList) {
    expressionsList = vector<Exp>(expList->expressionsList);
    expressionsList.insert(expressionsList.begin(), exp);
}

Statement::Statement(Variable *type) {
    if (DEBUG) {
        printMessage("In BREAK/CONTINUE");
        printMessage(type->value);
        printMessage(to_string(yylineno));
    }
//    int l = loopCounter;
//    int s = switchCounter;
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
    if (loopCounter == 0 && switchCounter == 0) {
        // We are not inside any loop, so a break or continue is illegal in this context
        if (type->value == "break") {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        } else if (type->value == "continue") {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        } else {
            if (DEBUG) {
                printMessage("not break or continue");
            }
        }
    } else if (loopCounter != 0) {
        // We are inside a loop so break and continue are both legal
        // Creating a list to patch later with the correct place to jump to, the patch would make the jump go back to the start of the loop
        // or exit from it
        int loc = buffer.emit("br label @");
        if (type->value == "break") {
            break_list = buffer.makelist({loc, FIRST});
        } else {
            continue_list = buffer.makelist({loc, FIRST});
        }
    } else if (switchCounter != 0) {
        // We are inside a switch, so a break and continue are both legal
        // Creating a list to patch later with the correct place to jump to, the patch would make the jump go outside of the switch
        int loc = buffer.emit("br label @");
        if (type->value == "break") {
            break_list = buffer.makelist({loc, FIRST});
        } else {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    } else if (type->value == "continue" && switchCounter != 0) {
        output::errorUnexpectedContinue(yylineno);
        exit(0);
    }
    dataTag = "break or continue";
}

Statement::Statement(string type, Exp *exp, Statement *wrappedStatement) {
    if (DEBUG) {
        printMessage("exp type ex");
        printMessage(type);
        printMessage(exp->type);
        printMessage(exp->value);
    }
    if (exp->type != "BOOL") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    if (wrappedStatement != nullptr) {
        break_list = wrappedStatement->break_list;
        continue_list = wrappedStatement->continue_list;
    } else {
        break_list = vector<pair<int, BranchLabelIndex>>();
        continue_list = vector<pair<int, BranchLabelIndex>>();
    }
    dataTag = "if if else while";
}

// For Return SC -> this is for a function with a void return type
Statement::Statement(const string &funcReturnType) {
    if (DEBUG) printMessage("statement func ret type");
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
    // Need to check if the current running function is of void type
    for (int i = symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : symTabStack[i]->records) {
            if (row->isFunc && row->name == currentRunningFunctionScopeId) {
                // We found the current running function
                if (row->type.back() == funcReturnType) {
                    dataTag = "void return value";
                    buffer.emit("ret void");
                } else {
                    output::errorMismatch(yylineno);
                    exit(0);
                }
            }
        }
    }
}

Statement::Statement(Exp *exp) {
    if (DEBUG) {
        printMessage("statement exp!!!!!!");
        printMessage(exp->type);
        printMessage(exp->value);
        printMessage("current func:");
        printMessage(currentRunningFunctionScopeId);
        //printSymTableStack();
    }
    // Need to check if the current running function is of the specified type
    if (exp->type == "VOID") {
        // Attempt to return a void expression from a value returning function
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (int i = symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : symTabStack[i]->records) {
            if (row->isFunc && row->name == currentRunningFunctionScopeId) {
                // We found the current running function
                if (row->type.back() == exp->type) {
                    dataTag = exp->value;
                    string realLLVMType = GetLLVMType(exp->type);
                    buffer.emit("ret " + realLLVMType + " %" + exp->register_name);
                } else if (row->type.back() == "INT" && exp->type == "BYTE") {
                    dataTag = row->type.back();
                    // Cast the Byte expression into an int for a proper return type
                    string dataRegister = registerPool.GetNewRegister();
                    buffer.emit("%" + dataRegister + " = zext i8 %" + exp->register_name + " to i32");
                    buffer.emit("ret i32 %" + dataRegister);
                } else {
                    output::errorMismatch(yylineno);
                    exit(0);
                }
            }
        }
    }
}

string saveNewDataOnStackVariable(string sourceReg, string sourceType, int offset) {
    string destReg = registerPool.GetNewRegister();
    string dataRegister = sourceReg;
    string regType = GetLLVMType(sourceType);
    if (regType != "i32") {
        dataRegister = zeroExtendType(sourceReg, regType);
    }
    buffer.emit("%" + destReg + " = add i32 0,%" + dataRegister);
    string regPtr = registerPool.GetNewRegister();
    if (offset >= 0) {
        if (DEBUG) {
            printMessage("Saving variable declared in function");
        }
        // This is a stack assigned variable
        buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
    } else if (offset < 0 && currentRunningFunctionArgumentsNumber > 0) {
        if (DEBUG) {
            printMessage("Saving function parameter");
        }
        // This is a function parameter
        buffer.emit("%" + regPtr + " = getelementptr [" + to_string(currentRunningFunctionArgumentsNumber) +
                    " x i32], [" + to_string(currentRunningFunctionArgumentsNumber) + " x i32]* %args, i32 0, i32 " +
                    to_string(currentRunningFunctionArgumentsNumber + offset));
    } else {
        // The variable that was requested is in an illegal place
        if (DEBUG) {
            printMessage("Issue getting variable from llvm stack");
        }
    }
    buffer.emit("store i32 %" + destReg + ", i32* %" + regPtr);
    return destReg;
}

Statement::Statement(Call *call) {
    dataTag = "function call";
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
}

Statement::Statement(Variable *id, Exp *exp) {
    if (!isDeclared(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }

    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
    // Searching for the variable in the symtab
    for (int i = symTabStack.size() - 1; i >= 0; i--) {
        // Need to search in the current symtab with no particular order for the current function
        for (auto &row : symTabStack[i]->records) {
            if (!row->isFunc && row->name == id->value) {
                // We found the desired variable
                if ((row->type.back() == exp->type) || (row->type.back() == "INT" && exp->type == "BYTE")) {
                    dataTag = row->type.back();
                    instruction = exp->instruction;
                    register_name = saveNewDataOnStackVariable(exp->register_name, exp->type, row->offset);
                }
            }
        }
    }
}

Statement::Statement(Type *t, Variable *id, Exp *exp) {
    if (DEBUG) printMessage("statement t id exp");
    if (isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    if ((t->value == exp->type) || (t->value == "INT" && exp->type == "BYTE")) {
        break_list = vector<pair<int, BranchLabelIndex>>();
        continue_list = vector<pair<int, BranchLabelIndex>>();
        dataTag = t->value;
        // Creating a new variable on the stack will cause the next one to have a higher offset
        int offset = offsetStack.back()++;
        vector<string> varType = {t->value};
        shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);
        symTabStack.back()->records.push_back(nVar);

        register_name = registerPool.GetNewRegister();
        string regType = GetLLVMType(t->value);
        string dataRegister = register_name;
        if (t->value == "INT" && exp->type == "BYTE") {
            // Need to zero extend the value to the assigned type
            dataRegister = registerPool.GetNewRegister();
            buffer.emit("%" + dataRegister + " = zext i8 %" + exp->register_name + " to i32");
        } else {
            dataRegister = exp->register_name;
        }
        buffer.emit("%" + register_name + " = add " + regType + " 0,%" + dataRegister);
        string regPtr = registerPool.GetNewRegister();
        buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
        dataRegister = register_name;
        if (regType != "i32") {
            // Need to extend the variable so it fits inside our stack
            dataRegister = registerPool.GetNewRegister();
            buffer.emit("%" + dataRegister + " = zext " + regType + " %" + register_name + " to i32");
        }

        // Storing the new declared variable in the stack of the current scope
        buffer.emit("store i32 %" + dataRegister + ", i32* %" + regPtr);
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Statement::Statement(Type *t, Variable *id) {
    if (isDeclared(id->value)) {
        // Trying to redeclare a name that is already used for a different variable/fucntion
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
    // Creating a new variable on the stack will cause the next one to have a higher offset
    int offset = offsetStack.back()++;
    vector<string> varType = {t->value};
    shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);
    symTabStack.back()->records.push_back(nVar);
    dataTag = t->value;
    register_name = registerPool.GetNewRegister();
    string regType = GetLLVMType(t->value);
    // This is a declaration of a new variable with no value assignment, so we initialize it to a default value of 0
    buffer.emit("%" + register_name + " = add " + regType + " 0,0");
    string regPtr = registerPool.GetNewRegister();
    // Getting a new pointer to store the declared variable inside the current scope
    buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
    if (DEBUG) printMessage("Getting a new pointer to store the declared variable inside the current scope");
    string dataRegister = register_name;
    if (regType != "i32") {
        if (DEBUG) printMessage("Need to extend the variable so it fits inside our stack");
        // Need to extend the variable so it fits inside our stack
        dataRegister = registerPool.GetNewRegister();
        buffer.emit("%" + dataRegister + " = zext " + regType + " %" + register_name + " to i32");
    }
    // Storing the new declared variable in the stack of the current scope
    buffer.emit("store i32 %" + dataRegister + ", i32* %" + regPtr);

    if (DEBUG) printSymTableStack();
}

Statement::Statement(Statements *states) {
    if (DEBUG) {
        printMessage("In statement from statements");
        printMessage(states->value);
    }
    break_list = states->break_list;
    continue_list = states->continue_list;
    dataTag = "statement block";
}

Statement::Statement(Exp *exp, CaseList *cList) {
    // Need to check that exp is a number (int,byte) and that all case decl in caselist are int or byte
    if (DEBUG) {
        if (!exp) {
            printMessage("RECEIVED A NULLPTR");
        }
        printMessage("statement exp caselist");
        printMessage(exp->value);
        printMessage(exp->type);
    }
    if (exp->type != "INT" && exp->type != "BYTE") {
        if (DEBUG) printMessage("Mismatch in exp type");
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (auto &i : cList->cases) {
        if (i->value != "INT" && i->value != "BYTE") {
            if (DEBUG) {
                printMessage("Mismatch in case type");
                printMessage(i->value);
            }
            output::errorMismatch(yylineno);
            exit(0);
        }
    }

    dataTag = "switch block";
    if (DEBUG) {
        printMessage("Exiting statement ctor after finishing switch");
    }
    // Print a jump to the checker of the first switch case
    buffer.emit("label_switch_" + to_string(switchId) + ":");
    int defaultIndex = 0;
    string endLabel = "label_switch_" + to_string(switchId) + "_end";
    for (int i = cList->cases.size() - 1; i >= 0; --i) {
        if (cList->cases[i]->register_name == "default") {
            defaultIndex = i;
            continue;
        }
        string tempReg = registerPool.GetNewRegister();
        string compReg = registerPool.GetNewRegister();
        buffer.emit("%" + tempReg + " = add i32 0, " + to_string(cList->cases[i]->numToInt));
        buffer.emit("%" + compReg + " = icmp eq i32 %" + exp->register_name + ", %" + tempReg);
        // true label is the first label
        string falseCaseLabel = "label_switch_" + to_string(switchId) + "_case_" + to_string(i);
        buffer.emit("br i1 %" + compReg + ", label %" + cList->cases[i]->instruction + ", label %" + falseCaseLabel);
        buffer.emit(falseCaseLabel + ":");
        if (!cList->cases[i]->break_list.empty()) {
            buffer.bpatch(cList->cases[i]->break_list, endLabel);
        }
        if (cList->cases.size() == 1) {
            // There is only 1 case, so we need to patch the ending
            buffer.emit("br label %" + endLabel);
        }
    }
    // Need to add a jump to the default case if none of the other cases was a hit
    buffer.emit("br label %" + cList->cases[defaultIndex]->instruction);
    buffer.bpatch(cList->break_list, endLabel);
    buffer.emit(endLabel + ":");

    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();
    continue_list.reserve(continue_list.size() + cList->continue_list.size());
    continue_list.insert(continue_list.end(), cList->continue_list.begin(), cList->continue_list.end());
    for (auto &i : cList->cases) {
        continue_list.reserve(continue_list.size() + i->continue_list.size());
        continue_list.insert(continue_list.end(), i->continue_list.begin(), i->continue_list.end());
    }
}

Statements::Statements(Statement* statement) {
    if (DEBUG) {
        printMessage("I reached this ctor");
    }
    break_list = statement->break_list;
    continue_list = statement->continue_list;
}

Statements::Statements(Statements* statements, Statement* statement) {
    break_list = buffer.merge(statements->break_list, statement->break_list);
    continue_list = buffer.merge(statements->continue_list, statement->continue_list);
}

CaseDecl::CaseDecl(Exp *num, Statements *states, Variable *caseLabel) {
    if (DEBUG) {
        printMessage("value of statements is:");
        printMessage(states->value);
        printMessage("value of exp:");
        printMessage(num->value);
        printMessage("type of exp:");
        printMessage(num->type);
    }
    instruction = caseLabel->instruction;
    if (num->type != "INT" && num->type != "BYTE") {
        //if (num->value != "INT" && num->value != "BYTE") {
        output::errorMismatch(yylineno);
        exit(0);
    }
    register_name = num->register_name;
    value = num->type;
    numToInt = stoi(num->value);
    break_list = states->break_list;
    continue_list = states->continue_list;
}

CaseDecl::CaseDecl() {

}

CaseList::CaseList(CaseDecl* caseDecl, CaseList* caseList) {
    cases = vector<CaseDecl *>(caseList->cases);
    cases.push_back(caseDecl);
    break_list = caseList->break_list;
    break_list.reserve(break_list.size() + caseDecl->break_list.size());
    break_list.insert(break_list.end(), caseDecl->break_list.begin(), caseDecl->break_list.end());
    continue_list = caseList->continue_list;
    continue_list.insert(continue_list.end(), caseDecl->continue_list.begin(), caseDecl->continue_list.end());

    value = "case list";
}

CaseList::CaseList(CaseDecl* caseDecl) {
    cases.push_back(caseDecl);
    value = "case list";
    break_list = caseDecl->break_list;
    continue_list = caseDecl->continue_list;
}

CaseList::CaseList(Statements* statements, N* label) {
    // This is the rule for default case
    CaseDecl *tempCase = new CaseDecl();
    tempCase->value = "INT";
    tempCase->register_name = "default";
    tempCase->instruction = label->instruction;
    buffer.bpatch(buffer.makelist({label->loc, FIRST}), label->instruction);
    this->cases.push_back(tempCase);
    // Printing the exit outside of default
    int loc = buffer.emit("br label @");
    this->break_list = buffer.merge(statements->break_list, buffer.makelist({loc, FIRST}));
    this->continue_list = statements->continue_list;
}

Variable *parseBooleanCondition(Exp *leftInstruction) {
    Variable *node = new P(leftInstruction);
    return node;
}

P::P(Exp *leftInstruction) {
    loc = buffer.emit("br i1 %" + leftInstruction->register_name + ", label @, label @");
    instruction = buffer.genLabel();
}

M::M() {
    instruction = buffer.genLabel();
}

N::N() {
    loc = buffer.emit("br label @");
    instruction = buffer.genLabel();
}

void backpatchIf(M *label, Exp *exp) {
    int loc = buffer.emit("br label @");
    string end = buffer.genLabel();
    if (DEBUG) {
        printMessage("Backpatching if statement");
    }
    // Patching the jump to the if instruction in case the IF condition is true
    buffer.bpatch(exp->true_list, label->instruction);
    // Patching the jump outside of the if in case the IF condition is false
    buffer.bpatch(exp->false_list, end);
    if (DEBUG) {
        printMessage("Backpatching lists");
    }
    buffer.bpatch(buffer.makelist({loc, FIRST}), end);
}

void insertFunctionParameters(Formals *formals) {
    for (unsigned int i = 0; i < formals->formals.size(); ++i) {
        vector<string> nType = {formals->formals[i]->paramType};
        shared_ptr<SymbolTableRecord> nParameter = make_shared<SymbolTableRecord>(formals->formals[i]->value, nType, -i - 1, false);
        symTabStack.back()->records.push_back(nParameter);
    }
}

void backpatchIfElse(M *label1, N *label2, Exp *exp) {
    int loc = buffer.emit("br label @");
    string end = buffer.genLabel();
    if (DEBUG) {
        printMessage("Backpatching if else statement");
    }
    // Patching the jump to the if instruction in case the IF condition is true
    buffer.bpatch(exp->true_list, label1->instruction);
    // Patching the jump to the else instruction in case the IF condition is false
    buffer.bpatch(exp->false_list, label2->instruction);
    if (DEBUG) {
        printMessage("Backpatching lists");
    }
    buffer.bpatch(buffer.makelist({label2->loc, FIRST}), end);
    buffer.bpatch(buffer.makelist({loc, FIRST}), end);
}

Funcs::Funcs() {
    if (DEBUG) printMessage("I am in funcs");
    if (strcmp(yytext, "") != 0) {
        output::errorSyn(yylineno);
        exit(0);
    }
}

Statement *mergeIfElseLists(Statement *ifStatement, Statement *elseStatement) {
    ifStatement->break_list = buffer.merge(ifStatement->break_list, elseStatement->break_list);
    ifStatement->continue_list = buffer.merge(ifStatement->continue_list, elseStatement->continue_list);
    return ifStatement;
}

string DeclareCaseLabel() {
    buffer.emit("; generating new label");
    int loc = buffer.emit("br label @");
    string label = buffer.genLabel();
    buffer.bpatch(buffer.makelist({loc, FIRST}), label);
    return label;
}
