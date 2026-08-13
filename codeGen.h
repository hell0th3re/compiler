#ifndef UNTITLED_CODEGEN_H
#define UNTITLED_CODEGEN_H

#include <variant>
using namespace std;

enum types {
    _int,
    _char
};

struct ints {
    string name;
    int value;
};

struct chars {
    string name;
    char value;
};

int getIndexOf(vector <string> vec, string s);
bool isChar(string s);
bool isNumber(string s);

string handleVars(vector <string> statement);
string varCode(variant<ints, chars> userVar);

string handleCalls(vector <string> statement);
string callCode(string name, string value);

string getVal(string name);
string write(string value);
string toChar();
string exit(string value);

#endif //UNTITLED_CODEGEN_H

