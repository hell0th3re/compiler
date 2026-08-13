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
//vector <string> getSlice(vector <string> vec, int start, int end);
bool isChar(string s);
bool isNumber(string s);

string handleVars(vector <string> statement);
vector <string> getSlice(vector <string> vec, int start, int end);
string varCode(variant<ints, chars> userVar);

vector <string> handleComplex(vector <string> statement);

string handleCalls(vector <string> statement);
string callCode(string name, string value, int index);

string getVal(string value, int index);
string write(string value);
string toChar();
string exit(string value);


#endif //UNTITLED_CODEGEN_H

