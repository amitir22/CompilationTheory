#include "tokens.hpp"
#include <stack>

// so every indentation will be translated to 4 spaces (0x20 ascii)
#define INDENT_SPACE_COUNT 4

using std::stack;

int main() {
    stack<enum tokentype> open_paren_stack;
    int current_indent = INDENT_SPACE_COUNT * open_paren_stack.size();

    int token = yylex();

    while(token) {
        if (token == LPAREN || token == LBRACE) {
            open_paren_stack.push(token);
        }

        token = yylex();
        // Your code here
    }

	return 0;
}