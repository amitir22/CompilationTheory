#include "Classes.h"
#include "RegisterFactory.h"
#include "iostream"
#include <memory>
#include <cstring>
#include <stack>

extern char *yytext;
int switchId = 0;
int maxSwitch = 0;
RegisterFactory registerFactory;
CodeBuffer &buffer = CodeBuffer::instance();
vector<shared_ptr<SymbolTable>> symbolTablesStack;
stack<int> offsetStack;

string currentFunctionId;
int argumentsCurrentFunction = 0;
int loopCounter = 0;
int switchCounter = 0;

SymbolTableRecord::SymbolTableRecord(string name, vector<string> type, int offset, bool isFunc) : name(std::move(name)), type(std::move(type)),
                                                                                                  offset(offset), isFunc(isFunc) {

}

string GetLLVMType(string type) {
    if (type == "VOID") {
        return "void";
    } else if (type == "BOOL") {
        return "i1";
    } else if (type == "BYTE") {
        return "i8";
    } else if (type == "STRING") {
        return "i8*";
    } else return "i32";
}

string zeroExtendType(string original_register, string llvmType) {
    string dest_register = registerFactory.makeNewRegister();

    buffer.emit("%" + dest_register + " = zext " + llvmType + " %" + original_register + " to i32");

    return dest_register;
}

void openScope() {
    symbolTablesStack.push_back(make_shared<SymbolTable>());
    offsetStack.push(offsetStack.top());
}

void closeScope() {
    shared_ptr<SymbolTable> currentSymbolTable = symbolTablesStack.back();

    for (auto &record : currentSymbolTable->records) {
        if (record->isFunc){
            string funcReturnType = record->type.back();
            // that's the return type from the vector
            record->type.pop_back();
        }
    }

    currentSymbolTable->records.clear();
    symbolTablesStack.pop_back();
    offsetStack.pop();
}

void enterSwitchState() {
    switchCounter++;
    maxSwitch++;
    switchId = maxSwitch;
}

void exitSwitchState() {
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

    buffer.bpatch(buffer.makelist({first->loc, FIRST}), first->instruction);
    buffer.bpatch(buffer.makelist({second->loc, FIRST}), second->instruction);
    buffer.bpatch(buffer.makelist({second->loc, SECOND}), label);
    buffer.bpatch(buffer.makelist({loc, FIRST}), first->instruction);

    if (!statement->continue_list.empty()) {
        buffer.bpatch(statement->continue_list, first->instruction);
    }
    if (!statement->break_list.empty()) {
        buffer.bpatch(statement->break_list, label);
    }
}

void exitProgramFuncs(RetType *ret) {
    if (ret->value == "VOID") {
        buffer.emit("ret void");
    } else {
        string funcReturnType = GetLLVMType(ret->value);
        buffer.emit("ret " + funcReturnType + " 0");
    }

    buffer.emit("}");
    currentFunctionId = "";
    argumentsCurrentFunction = 0;
}

void exitProgramRuntimeState() {
    shared_ptr<SymbolTable> globalScopeSymbolTable = symbolTablesStack.front();
    bool doesMainFuncExist = false;

    for (auto &record : globalScopeSymbolTable->records) {
        if (record->isFunc && record->name == "main" && record->type.back() == "VOID" &&
            record->type.size() == 1) {
            doesMainFuncExist = true;
        }
    }

    if (!doesMainFuncExist) {
        output::errorMainMissing();

        exit(0);
    }

    closeScope();
    buffer.printGlobalBuffer();
    buffer.printCodeBuffer();
}

bool isDeclared(const string &name) {
    // traversing the symbol tables of all the scopes to check if the given var/func name exists
    for (auto &symbolTable : symbolTablesStack) {
        for (auto &record : symbolTable->records) {
            if (record->name == name) {
                return true;
            }
        }
    }

    return false;
}

bool isDeclaredVariable(const string &name) {
    // traversing the symbol tables of all the scopes to check if the given variable name exists
    for (auto &symbolTable : symbolTablesStack) {
        for (auto &record : symbolTable->records) {
            if (record->name == name && !record->isFunc) {
                return true;
            }
        }
    }
    return false;
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

string loadVariableFromSymTab(int offset, string type) {
    string reg = registerFactory.makeNewRegister();
    string ptrRegister = registerFactory.makeNewRegister();

    if (offset >= 0) {
        buffer.emit("%" + ptrRegister + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
    } else if (offset < 0 && argumentsCurrentFunction > 0) {
        buffer.emit("%" + ptrRegister + " = getelementptr [" + to_string(argumentsCurrentFunction) + " x i32], [" +
                    to_string(argumentsCurrentFunction) + " x i32]* %args, i32 0, i32 " +
                    to_string(argumentsCurrentFunction + offset));
    }

    buffer.emit("%" + reg + " = load i32, i32* %" + ptrRegister);

    string varType = GetLLVMType(type);
    string register_name = reg;

    if (varType != "i32") {
        register_name = registerFactory.makeNewRegister();
        buffer.emit("%" + register_name + " = trunc i32 %" + reg + " to " + varType);
    }

    return register_name;
}

string saveNewDataOnStackVariable(string sourceReg, string sourceType, int offset) {
    string destReg = registerFactory.makeNewRegister();
    string dataRegister = sourceReg;
    string regType = GetLLVMType(sourceType);

    if (regType != "i32") {
        dataRegister = zeroExtendType(sourceReg, regType);
    }

    buffer.emit("%" + destReg + " = add i32 0,%" + dataRegister);

    string regPtr = registerFactory.makeNewRegister();

    if (offset >= 0) {
        buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));
    } else if (offset < 0 && argumentsCurrentFunction > 0) {
        buffer.emit("%" + regPtr + " = getelementptr [" + to_string(argumentsCurrentFunction) +
                    " x i32], [" + to_string(argumentsCurrentFunction) + " x i32]* %args, i32 0, i32 " +
                    to_string(argumentsCurrentFunction + offset));
    }

    buffer.emit("store i32 %" + destReg + ", i32* %" + regPtr);

    return destReg;
}

Variable *parseBooleanCondition(Exp *leftInstruction) {
    Variable *node = new P(leftInstruction);
    return node;
}

void backpatchIf(M *label, Exp *exp) {
    int loc = buffer.emit("br label @");
    string end = buffer.genLabel();

    buffer.bpatch(exp->true_list, label->instruction);
    buffer.bpatch(exp->false_list, end);
    buffer.bpatch(buffer.makelist({loc, FIRST}), end);
}

shared_ptr<SymbolTableRecord> makeParamSymbolRecord(string value, vector<string> currentParamType, int currentOffset) {
    return make_shared<SymbolTableRecord>(value, currentParamType, currentOffset, false);
}

void insertFunctionParametersToSymbolTable(Formals *formals) {
    int currentOffset;
    vector<string> currentParamType;
    shared_ptr<SymbolTableRecord> currentParamRecord;

    // iterating over all the function parameters and adding each one as a record in the symbol table
    for (unsigned int i = 0; i < formals->formals.size(); ++i) {
        currentOffset =  -i - 1;
        currentParamType = {formals->formals[i]->paramType};

        currentParamRecord = makeParamSymbolRecord(formals->formals[i]->value, currentParamType, currentOffset);

        symbolTablesStack.back()->records.push_back(currentParamRecord);
    }
}

void backpatchIfElse(M *label1, N *label2, Exp *exp) {
    int loc = buffer.emit("br label @");
    string end = buffer.genLabel();

    buffer.bpatch(exp->true_list, label1->instruction);
    buffer.bpatch(exp->false_list, label2->instruction);
    buffer.bpatch(buffer.makelist({label2->loc, FIRST}), end);
    buffer.bpatch(buffer.makelist({loc, FIRST}), end);
}

Funcs::Funcs() {
    if (strcmp(yytext, "") != 0) {
        output::errorSyn(yylineno);
        exit(0);
    }
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

    // putting the print and printi functions declarations in the symbolTable
    symTab->records.push_back(printFunc);
    symTab->records.push_back(printiFunc);

    // initializing the symbol-table scope stack with global scope symbol-table
    symbolTablesStack.push_back(symTab);

    // initializing the offset scope stack with the global scope offset
    offsetStack.push(0);

    // emitting print/printi functions declarations
    buffer.emitGlobal("declare i32 @printf(i8*, ...)");
    buffer.emitGlobal("declare void @exit(i32)");

    // emitting int specifier declaration
    buffer.emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");


    // emitting string specifier declaration
    buffer.emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    buffer.emitGlobal("@ThrowZeroException = constant [23 x i8] c\"Error division by zero\\00\"");

    // emitting print/printi definitions
    buffer.emitGlobal("define void @printi(i32) {");
    buffer.emitGlobal("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
    buffer.emitGlobal("ret void");
    buffer.emitGlobal("}");
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
    if (isDeclared(id->value)) {
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    for (unsigned int i = 0; i < funcParams->formals.size(); ++i) {
        if (isDeclared(funcParams->formals[i]->value) || funcParams->formals[i]->value == id->value) {
            output::errorDef(yylineno, funcParams->formals[i]->value);
            exit(0);
        }

        for (unsigned int j = i + 1; j < funcParams->formals.size(); ++j) {
            if (funcParams->formals[i]->value == funcParams->formals[j]->value) {
                output::errorDef(yylineno, funcParams->formals[i]->value);
                exit(0);
            }
        }
    }

    value = id->value;

    if (funcParams->formals.size() != 0) {
        for (auto &formal : funcParams->formals) {
            functionParamsTypes.push_back(formal->paramType);
        }
    }

    functionParamsTypes.push_back(rType->value);

    shared_ptr<SymbolTableRecord> nFunc = std::make_shared<SymbolTableRecord>(value, functionParamsTypes, 0, true);
    symbolTablesStack.back()->records.push_back(nFunc);
    currentFunctionId = value;
    argumentsCurrentFunction = funcParams->formals.size();

    string funcArgumentString = "(";

    for (int i = 0; i < funcParams->formals.size(); ++i) {
        funcArgumentString += GetLLVMType(funcParams->formals[i]->paramType);
        if (i != funcParams->formals.size() - 1) {
            funcArgumentString += ',';
        }
    }

    funcArgumentString.push_back(')');

    string funcReturnType = GetLLVMType(rType->value);

    buffer.emit("define " + funcReturnType + " @" + value + funcArgumentString + " {");
    buffer.emit("%stack = alloca [50 x i32]");
    buffer.emit("%args = alloca [" + to_string(funcParams->formals.size()) + " x i32]");

    for (int i = 0; i < funcParams->formals.size(); ++i) {
        string ptrRegister = registerFactory.makeNewRegister();

        buffer.emit("%" + ptrRegister + " = getelementptr [" + to_string(funcParams->formals.size()) + " x i32], [" +
                    to_string(funcParams->formals.size()) + " x i32]* %args, i32 0, i32 " + to_string(argumentsCurrentFunction - i - 1));

        string dataRegister = to_string(i);
        string funcArgumentType = GetLLVMType(funcParams->formals[i]->paramType);

        if (funcArgumentType != "i32") {
            dataRegister = registerFactory.makeNewRegister();
            buffer.emit("%" + dataRegister + " = zext " + funcArgumentType + " %" + to_string(i) + " to i32");
        }

        buffer.emit("store i32 %" + dataRegister + ", i32* %" + ptrRegister);
    }
}

Call::Call(Variable *id) {
    for (auto &symTab : symbolTablesStack) {
        for (auto &row : symTab->records) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == 1) {
                    value = row->type.back();
                    string funcName = id->value;
                    string returnTypeFromFunc = GetLLVMType(value);
                    register_name = registerFactory.makeNewRegister();
                    if (returnTypeFromFunc == "void") {
                        buffer.emit("call " + returnTypeFromFunc + " @" + funcName + " ()");
                    } else {
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

    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}

Call::Call(Variable *id, ExpList *list) {
    for (auto &symTab : symbolTablesStack) {
        for (auto &row : symTab->records) {
            if (row->name == id->value) {
                if (!row->isFunc) {
                    output::errorUndefFunc(yylineno, id->value);
                    exit(0);
                } else if (row->isFunc && row->type.size() == list->expressionsList.size() + 1) {
                    string funcArgs = "(";

                    for (unsigned int i = 0; i < list->expressionsList.size(); ++i) {
                        if (list->expressionsList[i].type == row->type[i]) {
                            funcArgs += GetLLVMType(list->expressionsList[i].type) + " %" + list->expressionsList[i].register_name + ",";
                            continue;
                        } else if (list->expressionsList[i].type == "BYTE" && row->type[i] == "INT") {
                            string dataRegister = registerFactory.makeNewRegister();
                            buffer.emit("%" + dataRegister + " = zext i8 %" + list->expressionsList[i].register_name + " to i32");
                            funcArgs += GetLLVMType("INT") + " %" + dataRegister + ",";
                            continue;
                        }

                        row->type.pop_back();
                        output::errorPrototypeMismatch(yylineno, id->value, row->type);
                        exit(0);
                    }

                    funcArgs.back() = ')';
                    value = row->type.back();

                    string funcName = id->value;
                    string returnTypeFromFunc = GetLLVMType(value);
                    register_name = registerFactory.makeNewRegister();

                    if (returnTypeFromFunc == "void") {
                        buffer.emit("call " + returnTypeFromFunc + " @" + funcName + " " + funcArgs);
                    } else {
                        buffer.emit("%" + register_name + " = call " + returnTypeFromFunc + " @" + funcName + " " + funcArgs);
                    }

                    int loc = buffer.emit("br label @");
                    instruction = buffer.genLabel();
                    buffer.bpatch(buffer.makelist({loc, FIRST}), instruction);
                    return;
                } else {
                    row->type.pop_back();
                    output::errorPrototypeMismatch(yylineno, id->value, row->type);
                    exit(0);
                }
            }
        }
    }

    output::errorUndefFunc(yylineno, id->value);
    exit(0);
}

Exp::Exp(Call *call) {
    register_name = call->register_name;
    value = call->value;
    type = call->value;
    instruction = call->instruction;
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
}

Exp::Exp(Variable *id) {
    if (!isDeclaredVariable(id->value)) {
        output::errorUndef(yylineno, id->value);
        exit(0);
    }

    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();

    for (auto symbolTableIter = symbolTablesStack.rbegin();
         symbolTableIter != symbolTablesStack.rend();
         ++symbolTableIter) {
        for (auto &record : (*symbolTableIter)->records) {
            if (record->name == id->value) {
                value = id->value;
                type = record->type.back();
                register_name = loadVariableFromSymTab(record->offset, type);
                return;
            }
        }
    }
}

Exp::Exp(Variable *variable, Exp *exp) {
    if (exp->type != "BOOL") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    type = "BOOL";
    register_name = registerFactory.makeNewRegister();

    buffer.emit("%" + register_name + " = add i1 1, %" + exp->register_name);

    actualBoolean = !actualBoolean;
    false_list = exp->true_list;
    true_list = exp->false_list;
}

Exp::Exp(Variable *variable, string taggedType) : Variable(variable->value) {
    type = taggedType;
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
    if (taggedType == "CASE_NUM") {
        type = "INT";
    }
    if (taggedType == "NUM") {
        type = "INT";
        register_name = registerFactory.makeNewRegister();

        buffer.emit("%" + register_name + " = add i32 0," + variable->value);
    }
    if (type == "BYTE") {
        if (stoi(variable->value) > 255) {
            output::errorByteTooLarge(yylineno, variable->value);
            exit(0);
        }

        register_name = registerFactory.makeNewRegister();

        buffer.emit("%" + register_name + " = add i8 0," + variable->value);
    }
    if (type == "BOOL") {
        register_name = registerFactory.makeNewRegister();

        if (variable->value == "true") {
            actualBoolean = true;
            buffer.emit("%" + register_name + " = add i1 0,1");
        } else {
            actualBoolean = false;
            buffer.emit("%" + register_name + " = add i1 0,0");
        }
    }

    if (type == "STRING") {
        register_name = registerFactory.makeNewRegister();

        int size = variable->value.size();
        string sub = variable->value.substr(1, variable->value.length() - 2);
        string num_size = to_string(variable->value.size() - 1);

        buffer.emitGlobal("@" + register_name + " = constant [" + num_size + " x i8] c\"" +
                          variable->value.substr(1, variable->value.length() - 2) + "\\00\"");
        buffer.emit("%" + register_name + " = getelementptr [" + num_size + " x i8], [" + num_size + " x i8]* @" + register_name + ", i8 0, i8 0");
    }
}

Exp::Exp(Exp *ex) {
    value = ex->value;
    type = ex->type;
    actualBoolean = ex->actualBoolean;
    register_name = ex->register_name;
    instruction = ex->instruction;
    true_list = ex->true_list;
    false_list = ex->false_list;
}

Exp::Exp(Exp *e1, Variable *op, Exp *e2, const string &taggedType, P *leftInstruction) {
    register_name = registerFactory.makeNewRegister();
    true_list = vector<pair<int, BranchLabelIndex>>();
    false_list = vector<pair<int, BranchLabelIndex>>();
    string end = "";
    bool bothByte = false;

    if ((e1->type == "INT" || e1->type == "BYTE") && (e2->type == "INT" || e2->type == "BYTE")) {
        if (taggedType == "EQ_NEQ_RELOP" || taggedType == "REL_RELOP") {
            type = "BOOL";
            string llvmSize = "i8";
            string relop;

            if (e1->type == "INT" || e2->type == "INT") {
                llvmSize = "i32";
            }

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

            if (llvmSize == "i32") {
                if (e1->type == "BYTE") {
                    leftRegister = registerFactory.makeNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                }
                if (e2->type == "BYTE") {
                    rightRegister = registerFactory.makeNewRegister();
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

            string operation;
            string leftRegister = e1->register_name;
            string rightRegister = e2->register_name;
            string llvmSize = "i8";
            register_name = registerFactory.makeNewRegister();
            type = "BYTE";

            if (e1->type == "INT" || e2->type == "INT") {
                type = "INT";
                llvmSize = "i32";
            }
            if (e1->type != e2->type) {
                if (e1->type == "BYTE") {
                    leftRegister = registerFactory.makeNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                }
                if (e2->type == "BYTE") {
                    rightRegister = registerFactory.makeNewRegister();
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
                string condition = registerFactory.makeNewRegister();

                if (bothByte) {
                    leftRegister = registerFactory.makeNewRegister();
                    buffer.emit("%" + leftRegister + " = zext i8 %" + e1->register_name + " to i32");
                    rightRegister = registerFactory.makeNewRegister();
                    buffer.emit("%" + rightRegister + " = zext i8 %" + e2->register_name + " to i32");
                }

                buffer.emit("%" + condition + " = icmp eq i32 %" + rightRegister + ", 0");

                int zeroDivisionBranchCheck = buffer.emit("br i1 %" + condition + ", label @, label @");
                string zeroDivisionCaseLabel = buffer.genLabel();
                string zeroDivisionExceptionReg = registerFactory.makeNewRegister();

                buffer.emit("%" + zeroDivisionExceptionReg + " = getelementptr [23 x i8], [23 x i8]* @ThrowZeroException, i32 0, i32 0");
                buffer.emit("call void @print(i8* %" + zeroDivisionExceptionReg + ")");
                buffer.emit("call void @exit(i32 0)");

                int normalDivisionCaseLabelLoc = buffer.emit("br label @");
                string normalDivisionBranchLabel = buffer.genLabel();

                buffer.bpatch(buffer.makelist({zeroDivisionBranchCheck, FIRST}), zeroDivisionCaseLabel);
                buffer.bpatch(buffer.makelist({zeroDivisionBranchCheck, SECOND}), normalDivisionBranchLabel);
                buffer.bpatch(buffer.makelist({normalDivisionCaseLabelLoc, FIRST}), normalDivisionBranchLabel);

                llvmSize = "i32";
                operation = "sdiv";
                end = normalDivisionBranchLabel;
            }

            buffer.emit("%" + register_name + " = " + operation + " " + llvmSize + " %" + leftRegister + ", %" + rightRegister);

            if (operation == "sdiv" && bothByte) {
                string tempReg = registerFactory.makeNewRegister();
                buffer.emit("%" + tempReg + " = trunc i32 %" + register_name + " to i8");
                register_name = tempReg;
            }
        }
    } else if (e1->type == "BOOL" && e2->type == "BOOL") {
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
                int secondCheckIsFalseLabel = buffer.emit("br label @");
                end = buffer.genLabel();

                buffer.emit("%" + register_name + " = phi i1 [%" + e2->register_name + ", %" + instruction + "],[0, %" + leftIsFalseLabel + "]");
                buffer.bpatch(buffer.makelist({leftInstruction->loc, FIRST}), leftInstruction->instruction);
                buffer.bpatch(buffer.makelist({leftInstruction->loc, SECOND}), leftIsFalseLabel);
                buffer.bpatch(buffer.makelist({firstCheckIsFalseBranchJump, FIRST}), end);
                buffer.bpatch(buffer.makelist({secondCheckIsFalseLabel, FIRST}), end);

                if (e1->actualBoolean && e2->actualBoolean) {
                    actualBoolean = true;
                } else {
                    actualBoolean = false;
                }
            } else if (op->value == "or") {
                int firstCheckIsTrueBranchJump = buffer.emit("br label @");
                string leftIsTrueLabel = buffer.genLabel();
                int secondCheckIsTrueLabel = buffer.emit("br label @");
                end = buffer.genLabel();

                buffer.emit("%" + register_name + " = phi i1 [%" + e2->register_name + ", %" + instruction + "],[1, %" + leftIsTrueLabel + "]");
                buffer.bpatch(buffer.makelist({leftInstruction->loc, FIRST}), leftIsTrueLabel);
                buffer.bpatch(buffer.makelist({leftInstruction->loc, SECOND}), leftInstruction->instruction);
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

ExpList::ExpList(Exp *exp) {
    expressionsList.insert(expressionsList.begin(), exp);
}

ExpList::ExpList(Exp *exp, ExpList *expList) {
    expressionsList = vector<Exp>(expList->expressionsList);
    expressionsList.insert(expressionsList.begin(), exp);
}

Statement::Statement(Variable *type) {
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();

    if (loopCounter == 0 && switchCounter == 0) {
        if (type->value == "break") {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        } else if (type->value == "continue") {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    } else if (loopCounter != 0) {
        int loc = buffer.emit("br label @");

        if (type->value == "break") {
            break_list = buffer.makelist({loc, FIRST});
        } else {
            continue_list = buffer.makelist({loc, FIRST});
        }
    } else if (switchCounter != 0) {
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

Statement::Statement(const string &funcReturnType) {
    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();

    for (int i = symbolTablesStack.size() - 1; i >= 0; i--) {
        for (auto &row : symbolTablesStack[i]->records) {
            if (row->isFunc && row->name == currentFunctionId) {
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
    if (exp->type == "VOID") {
        output::errorMismatch(yylineno);
        exit(0);
    }

    for (int i = symbolTablesStack.size() - 1; i >= 0; i--) {
        for (auto &row : symbolTablesStack[i]->records) {
            if (row->isFunc && row->name == currentFunctionId) {
                if (row->type.back() == exp->type) {
                    dataTag = exp->value;
                    string realLLVMType = GetLLVMType(exp->type);

                    buffer.emit("ret " + realLLVMType + " %" + exp->register_name);
                } else if (row->type.back() == "INT" && exp->type == "BYTE") {
                    dataTag = row->type.back();
                    string dataRegister = registerFactory.makeNewRegister();

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

    for (int i = symbolTablesStack.size() - 1; i >= 0; i--) {
        for (auto &row : symbolTablesStack[i]->records) {
            if (!row->isFunc && row->name == id->value) {
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
    if (isDeclared(id->value)) {
        output::errorDef(yylineno, id->value);
        exit(0);
    }
    if ((t->value == exp->type) || (t->value == "INT" && exp->type == "BYTE")) {
        break_list = vector<pair<int, BranchLabelIndex>>();
        continue_list = vector<pair<int, BranchLabelIndex>>();
        dataTag = t->value;

        int offset = offsetStack.top()++;

        vector<string> varType = {t->value};
        shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);

        symbolTablesStack.back()->records.push_back(nVar);

        register_name = registerFactory.makeNewRegister();
        string regType = GetLLVMType(t->value);
        string dataRegister = register_name;

        if (t->value == "INT" && exp->type == "BYTE") {
            dataRegister = registerFactory.makeNewRegister();
            buffer.emit("%" + dataRegister + " = zext i8 %" + exp->register_name + " to i32");
        } else {
            dataRegister = exp->register_name;
        }

        buffer.emit("%" + register_name + " = add " + regType + " 0,%" + dataRegister);

        string regPtr = registerFactory.makeNewRegister();

        buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));

        dataRegister = register_name;

        if (regType != "i32") {
            dataRegister = registerFactory.makeNewRegister();
            buffer.emit("%" + dataRegister + " = zext " + regType + " %" + register_name + " to i32");
        }

        buffer.emit("store i32 %" + dataRegister + ", i32* %" + regPtr);
    } else {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Statement::Statement(Type *t, Variable *id) {
    if (isDeclared(id->value)) {
        output::errorDef(yylineno, id->value);
        exit(0);
    }

    break_list = vector<pair<int, BranchLabelIndex>>();
    continue_list = vector<pair<int, BranchLabelIndex>>();

    int offset = offsetStack.top()++;
    vector<string> varType = {t->value};
    shared_ptr<SymbolTableRecord> nVar = std::make_shared<SymbolTableRecord>(id->value, varType, offset, false);

    symbolTablesStack.back()->records.push_back(nVar);
    dataTag = t->value;
    register_name = registerFactory.makeNewRegister();

    string regType = GetLLVMType(t->value);

    buffer.emit("%" + register_name + " = add " + regType + " 0,0");

    string regPtr = registerFactory.makeNewRegister();

    buffer.emit("%" + regPtr + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(offset));

    string dataRegister = register_name;

    if (regType != "i32") {
        dataRegister = registerFactory.makeNewRegister();

        buffer.emit("%" + dataRegister + " = zext " + regType + " %" + register_name + " to i32");
    }

    buffer.emit("store i32 %" + dataRegister + ", i32* %" + regPtr);
}

Statement::Statement(Statements *states) {
    break_list = states->break_list;
    continue_list = states->continue_list;
    dataTag = "statement block";
}

Statement::Statement(Exp *exp, CaseList *cList) {
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

    buffer.emit("label_switch_" + to_string(switchId) + ":");

    int defaultIndex = 0;
    string endLabel = "label_switch_" + to_string(switchId) + "_end";

    for (int i = cList->cases.size() - 1; i >= 0; --i) {
        if (cList->cases[i]->register_name == "default") {
            defaultIndex = i;
            continue;
        }

        string tempReg = registerFactory.makeNewRegister();
        string compReg = registerFactory.makeNewRegister();

        buffer.emit("%" + tempReg + " = add i32 0, " + to_string(cList->cases[i]->numToInt));
        buffer.emit("%" + compReg + " = icmp eq i32 %" + exp->register_name + ", %" + tempReg);

        string falseCaseLabel = "label_switch_" + to_string(switchId) + "_case_" + to_string(i);

        buffer.emit("br i1 %" + compReg + ", label %" + cList->cases[i]->instruction + ", label %" + falseCaseLabel);
        buffer.emit(falseCaseLabel + ":");

        if (!cList->cases[i]->break_list.empty()) {
            buffer.bpatch(cList->cases[i]->break_list, endLabel);
        }
        if (cList->cases.size() == 1) {
            buffer.emit("br label %" + endLabel);
        }
    }

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
    break_list = statement->break_list;
    continue_list = statement->continue_list;
}

Statements::Statements(Statements* statements, Statement* statement) {
    break_list = buffer.merge(statements->break_list, statement->break_list);
    continue_list = buffer.merge(statements->continue_list, statement->continue_list);
}

CaseDecl::CaseDecl(Exp *num, Statements *states, Variable *caseLabel) {
    instruction = caseLabel->instruction;

    if (num->type != "INT" && num->type != "BYTE") {
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
    CaseDecl *tempCase = new CaseDecl();

    tempCase->value = "INT";
    tempCase->register_name = "default";
    tempCase->instruction = label->instruction;

    buffer.bpatch(buffer.makelist({label->loc, FIRST}), label->instruction);

    this->cases.push_back(tempCase);

    int loc = buffer.emit("br label @");

    this->break_list = buffer.merge(statements->break_list, buffer.makelist({loc, FIRST}));
    this->continue_list = statements->continue_list;
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
