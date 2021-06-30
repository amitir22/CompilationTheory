#include "RegisterFactory.h"

string RegisterFactory::makeNewRegister() {
    string newRegister = "t" + to_string(counter);

    ++counter;

    return newRegister;
}