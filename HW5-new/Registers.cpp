#include "Registers.h"

using std::to_string;

Registers::Registers() : current(0) {

}

string Registers::GetNewRegister() {
    return "t" + to_string(current++);
}
