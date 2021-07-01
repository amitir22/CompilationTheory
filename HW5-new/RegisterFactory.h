#ifndef COMPILATIONTHEORY_REGISTERFACTORY_H
#define COMPILATIONTHEORY_REGISTERFACTORY_H

#include <string>

using std::string;

class RegisterFactory {
private:
    unsigned int counter;

public:
    RegisterFactory() : counter(0) { }

    string makeNewRegister();
};


#endif //COMPILATIONTHEORY_REGISTERFACTORY_H
