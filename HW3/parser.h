// todo: remove all those signatures:
// Created by Michael on 02-Jun-20.
//

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

using namespace std;

struct param
{
	string name;
	string type;
};

struct parser
{
	string var_name;
	string type;
	vector<struct param>* listParams;
};

#endif //PARSER_H