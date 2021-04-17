#include <iostream>
#include <cstdlib>
#include <stack>
#include <string>

#include "tokens.hpp"

using std::stack;
using std::string;
using std::cout;
using std::endl;

void print_bad_expression();
void print_bad_token(int token);
void print_bad_char(char invalid_char);

int main() {
    stack<int> open_paren_stack;
    size_t current_indent_count;
    string current_indentation;
    int token;

    while((token = yylex())) {
        if (token == -1) {
            char *undefined = yytext;

            char invalid_char = undefined[0];

            print_bad_char(invalid_char);
        }
        else if (token == LPAREN || token == LBRACE) {
            current_indent_count = open_paren_stack.size();
            open_paren_stack.push(token);

            current_indentation = string(current_indent_count, '\t');

            char open;

            if (token == LPAREN) {
                open = '(';
            }
            else { // token == LBRACE
                open = '{';
            }

            cout << current_indentation << open << endl;
        }
        else if (token == RPAREN || token == RBRACE) {
            if (open_paren_stack.empty()) {
                print_bad_expression();
            }

            int last_open = open_paren_stack.top();

            open_paren_stack.pop();

            // assuming the only tokens in `open_param_stack` are RPAREN or RBRACE
            if (token - last_open == 1) {
                current_indent_count = open_paren_stack.size();
                current_indentation = string(current_indent_count, '\t');

                char close;

                if (token == RPAREN) {
                    close = ')';
                }
                else { // token == RBRACE
                    close = '}';
                }

                cout << current_indentation << close << endl;
            }
            else {
                print_bad_expression();
            }
        }
        else {
            print_bad_token(token);
        }
    }

    if (!open_paren_stack.empty()) {
        // means there are still open parenthesis waiting ti be closed
        print_bad_expression();
    }

	return 0;
}

void print_bad_expression() {
    cout << "Error: Bad Expression" << endl;
    exit(-1);
}

void print_bad_token(int token) {
    cout << "Error: " << token_to_string(token) << endl;
    exit(-1);
}

void print_bad_char(char invalid_char) {
    cout << "Error: " << invalid_char << endl;
    // todo: maybe assert yylength == 1 ?

    exit(-1);
}