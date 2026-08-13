#include <iostream>
#include <vector>
#include <variant>
#include <sstream>
#include <map>
#include "codeGen.h"

#include "parser.h"

using namespace std;

/*
 *Stack tutorial so i dont forget:
 *  can only the the top element
 *  push rax = push the value from rax onto the top of the stack
 *  pop rax = get the value from the top of the stack and save it in rax
 *
 *  using 64bit registers the value of the previous stack element will be 64bit ABOVE the current memmory adr
 *
 *  so to get the PREVIOUS element of the stack you go
 *  push 5
 *  push 3
 *  mov rax, [rsp+8] (rsp is the stack pointer (that points to the top of the stack, which is the lowest mem adr???))
 */

//vars

//structs, enums in header

int varCount = 0; //will be useful to determine the declared vars position in the stack

map<string, int> varMap;

int getIndexOf(vector <string> vec, string s) {
    int index = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == s) {
            index = i;
        }
    }
    return index;
}

// vector <string> getSlice(vector <string> vec, int start, int end) {
//     vector <string> sliced = {};
//     for (int i = start; i < end; i++) {
//         sliced.push_back(vec[i]);
//     }
//     return sliced;
// }



bool isChar(string s) {

    if (s[0] == '\'' && s[2] == '\'') {
        return true;
    }

    if (s.empty()) {
        cerr << "empty" << endl;
        return false;
    }

    return false;
}

bool isNumber(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}



string handleVars(vector <string> statement) {
    //syntax: let name = value : type;
    string code;
    varCount++;

    string name = statement[1]; //name needs to come right after "let"
    string value = statement[ getIndexOf(statement,"=") + 1 ]; //element after '='
    string typeStr = statement[statement.size() - 1]; //type need to be the last element

    typeStr.pop_back(); //remove semi

    if (name.empty() || value.empty() || typeStr.empty()) {
        cerr << "invalid syntax" << endl;
        return "";
    }

    for (size_t i = 0; i < name.size(); i++) {
        if (!isalpha(name[i])) {
            cerr << "need a variable name" << endl;
            return "";
        }
    }


    if (typeStr == "int") {
        int valueConv = stoi(value); //string to int (value)
        ints userVar(name, valueConv);
        varCode(userVar);
        code += varCode(userVar);

        return code;
    }

    if (typeStr == "char") {
        //char valueConv = value[0]; //implicit conversion from string to char (in my lang not c++)
        if (!isChar(value)) {
            cerr << "invalid syntax" << endl;
            return "";
        }

        char valueConv = value[1];
        chars userVar(name, valueConv);
        varCode(userVar);
        code += varCode(userVar);

        return code;
    }

    cerr << "undefined type" << endl;
    return "";
}

string varCode(variant <ints, chars> userVar) {
    stringstream codeOut;

    if (holds_alternative<ints>(userVar)) {
        ints getStruct = get<ints>(userVar);
        varMap.insert({getStruct.name, varCount});

        codeOut << "    push " << getStruct.value << "\n";
    }
    else if (holds_alternative<chars>(userVar)) {
        chars getStruct = get<chars>(userVar);
        varMap.insert({getStruct.name, varCount});

        int asciConv = getStruct.value;
        codeOut << "    push " << asciConv << "\n";
    }

    return codeOut.str();
}

stringstream declCode;

vector <string> handleComplex(vector <string> statement) {
    //syntax: letC name = [element1, element2] :type_in_structure ^ type_of_structure;
    //remember to add "^" and [] to the parser
    varCount++;

    //make it clear the address is a variable on the stack
    string name = statement[1];
    varMap.insert({name, varCount});

    string structType = statement[statement.size() - 1];
    structType.pop_back();

    vector <string> codes = {}; //[0] = declCode; [1] = textCode;

    if (structType == "array") {

        stringstream codeOut;

        string varType = statement[statement.size() - 3];


        int start = getIndexOf(statement, "[");
        int end = getIndexOf(statement, "]");

        vector <string> userArr = getSlice(statement, start+1, end);
        //saved the values from the user array in a vector

        //iterate over the user array and remove the commas
        for (int i = 0; i < userArr.size(); i++) {
            if (userArr[i] == ",") {
                userArr.erase(userArr.begin() + i);
            }
        }

        if (varType == "int") {
            //syntax: letC name = [1,23,4] :int ^ array;
            for (int i = 0; i < userArr.size(); i++) {
                if (!isNumber(userArr[i])) {
                    cerr << "invalid type" << endl;
                    return {};
                }
            }

            declCode << "    " << name << ": resq " << userArr.size() << "\n"; //reserves space
            codes.push_back(declCode.str());

            //put the values in the .bss
            for (int i = 0; i < userArr.size(); i++) {
                codeOut << "    mov qword [" << name << " + " << 8*i << "], " << userArr[i] << "\n";
            }

            //loads the address to the array onto the main stack
            codeOut << "    mov rax, " << name << "\n";
            codeOut << "    push rax\n";

            codes.push_back(codeOut.str());
        }

        // for (int i = 0; i < userArr.size(); i++) {
        //     cout << userArr[i] << endl;
        // }
    }

    return codes;
}


//calls syntax: call call_name value
string handleCalls(vector <string> statement) {
    stringstream code;
    string value = "";
    int index = 0;
    string name = statement[1];

    if (!empty(statement[2])) {
        value = statement[2];
    }


    if (statement.size() > 3 && statement[3] == "[") {
        index = stoi(statement[4]);
    }
    else if (statement.size() == 2) {
        value.pop_back();
    }
    else {
        name.pop_back();
    }

    //handle getval
    if (name == "getval") {

        if (value.empty()) {
            cerr << "invalid syntax" << endl;
            return "";
        }
        code << callCode(name, value, index);
    }

    //handle write
    else if (name == "write") {
        code << callCode(name, value, index);
    }

    //handle exit
    else if (name == "exit") {
        code << callCode(name, value, index);
    }

    //hadle int -> char conversion
    else if (name == "toChar") {
        code << callCode(name, value, index);
    }

    return code.str();
}


string callCode(string name, string value, int index) {
    stringstream code;

    //pushes the variable to the top of the stack. Arg(var_name)
    if (name == "getval") {
        code << getVal(value, index); //"value" here cooresponds to the variable name
        cout << "Fwrge";
    }

    //writes to the console. Arg(<var_name, char>)
    else if (name == "write") {
        cout << "hi" << endl;
        code << write(value);
    }

    //converts the variable from the top of the stack to type : char
    else if (name == "toChar") {
        code << toChar(); //args to do later
    }

    else if (name == "exit") {
        code << exit(value);
    }

    return code.str();
}

//puts the variable value at the top of the stack
string getVal(string value, int index) {
    stringstream code;

    int pos = varMap[value];
    int offset = (varCount - pos) * 8; //calculates the needed rsp offset

    if (index != 0) {
        code << "    mov rax, [" << value << " + " << index*8 << "]\n";
        code << "    push rax\n";
    }
    else {
        code << "    mov rax, [rsp + " << offset << "]\n";
        code << "    push rax\n"; //this might just be copying the value idk. possible stack overflow (W &)
    }

    return code.str();
}

string write(string value) {
    //maybe an optional "nl" parameter at the end?
    //it would make even more sense for the syntax to be smth like:
    //"call write 'c', nl" because then things like call "write 'c', 'd';" would make sense as well
    //but that introduces the issue of an undefined number of parameters
    //so ill probably do that when im implementing functions and "write" will just be a built in function

    stringstream code;
    cout << "Ge" << endl;
    if (value.empty()){
        code << "    mov rsi, rsp\n";
    }
    else if (isChar(value)) {
        char chContents = value[1];
        int asciConv = chContents;

        code << "    push " << asciConv << "\n";
        code << "    mov rsi, rsp\n";
    }
    else if (value == "nl") {
        code << "    push 10\n";
        code << "    mov rsi, rsp\n";
    }
    else if (varMap.contains(value)) {
        code << getVal(value, 0);
        code << "    mov rsi, rsp\n";

    }
    else {
        cerr << "number or var name required" << endl;
        return "";
    }

    code << "    mov rax, 1\n";
    code << "    mov rdi, 1\n";
    code << "    mov rdx, 1\n"; //lenght;
    code << "    syscall\n";
    code << "    pop rax\n";

    return code.str();
}

//explicit conversion from int to char
//takes from the stack, adds 48 and puts it on the stack
string toChar() {
    stringstream code;
    code << "    pop rax\n";
    code << "    add rax, 48\n";
    code << "    push rax\n";

    return code.str();
}

string exit(string value) {
    stringstream code;

    if (value.empty()){
        code << "    pop rdi\n";
    }
    else if (varMap.contains(value)) {
        code << getVal(value, 0);
        code << "    pop rdi\n";
    }
    else if (isNumber(value)) {
        code << "    push " << value << "\n";
        code << "    pop rdi\n";
    }
    else {
        cerr << "number or var name required" << endl;
        return "";
    }

    code << "    mov rax, 60\n";
    code << "    syscall\n";

    return code.str();
}
