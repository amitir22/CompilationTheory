#include "tokens.hpp"


int main() {
	int token = yylex();

    while(token) {
        token = yylex();
        // Your code here
    }

	return 0;
}