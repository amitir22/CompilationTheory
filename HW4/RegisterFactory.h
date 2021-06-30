#ifndef COMPILATIONTHEORY_REGISTERFACTORY_H
#define COMPILATIONTHEORY_REGISTERFACTORY_H

#include <string>

using std::to_string;

class RegisterFactory {
    unsigned int counter;

    RegisterFactory() : counter(0) { }

    string makeNewRegister();
};


#endif //COMPILATIONTHEORY_REGISTERFACTORY_H
