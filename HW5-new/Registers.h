#ifndef HW5_REGISTERS_H
#define HW5_REGISTERS_H

#include <string>

using std::string;

class Registers {
public:
    unsigned int current;

    Registers();

    string GetNewRegister();
};


#endif //HW5_REGISTERS_H
