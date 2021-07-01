#include "RegisterFactory.h"

using std::to_string;

string RegisterFactory::makeNewRegister() {
    string newRegister = "t" + to_string(counter);

    ++counter;

    return newRegister;
}