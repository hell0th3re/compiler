#include <iostream>
#include <vector>
#include <variant>
#include <sstream>
#include <map>
#include "codeGen.h"

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

// this doesnt check for a question mark or a equals sign, but it has no point in doing so yet,
// so when it does i'll rewrite it.
map<string, int> varMap;
map<string, int> varMapRev;

int getIndexOf(vector <string> vec, string s) {
    int index = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == s) {
            index = i;
        }
    }
    return index;
}

bool isChar(string s) {
    if (s.empty()) {
        cerr << "empty" << endl;
        return false;
    }

    if (s[0] == '\'' && s[2] == '\'') {
        return true;
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

//calls syntax: call call_name value
string handleCalls(vector <string> statement) {
    stringstream code;

    string value = "";
    string name = statement[1];

    if (statement.size() > 2) {
        value = statement[2];
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

        code << callCode(name, value);
    }
    else if (name == "write") {
        code << callCode(name, value);
    }
    //handle exit
    else if (name == "exit") {
        if (value.empty()) {
            cerr << "invalid syntax" << endl;
            return "";
        }

        code << callCode(name, value);
    }

    return code.str();
}

string callCode(string name, string value) {
    stringstream code;

    if (name == "getval") {
        //gets the desired variable to the top of the stack

        for (size_t i = 0; i < value.size(); i++) {
            if (!isalpha(value[i])) {
                cerr << "need a variable to get" << endl;
                return "";
            }
        }

        varMapRev = varMap;

        for (auto &[name, value] : varMapRev) {
            value = varMapRev.size() - value + 1;
        }

        int posRev = varMapRev[value]; //should return reverse position varCount (last ones first)

        //(position on the stack starting from 1 for the first var)
        int offset = (posRev-1)*8; //calculates the bytes for the needed rsp offset

        code << "    mov rax, [rsp + " << offset << "]\n";
        code << "    push rax\n"; //this might just be copying the value idk. possible stack overflow (W &)
    }

    else if (name == "exit") {
        //exits
        //add the getval a; exit; thing like with write;
        string errNr = value;

        bool isVar = false;
        bool isDigit = true;

        if (varMap[errNr]) { //if its a variable name
            isVar = true;
        }

        if (!isVar && !isDigit) {
            cerr << "unknown identifier" << endl;
            return "";
        }

        if (isVar) {
            code << "    pop rdi\n";
        }
        else if (isDigit) {
            code << "    mov rdi, " << errNr << "\n";
        }

        code << "    mov rax, 60\n";
        code << "    syscall\n";
    }

    else if (name == "write") {

        if (varMap[value] || value.empty()) {
            code << "    mov rsi, rsp\n";
        }
        else if (isNumber(value)) {
            int asciVal = stoi(value);

            code << "    push " << asciVal << "\n";
            code << "    mov rsi, rsp\n";
            code << "    pop rax\n";
        }
        else if (isChar(value)) {
            char chContents = value[1];
            int asciConv = chContents;

            code << "    push " << asciConv << "\n";
            code << "    mov rsi, rsp\n";
            code << "    pop rax\n";
            //code << "    mov rsi, [rax]\n";
        }

        else {
            cerr << "number or var name required" << endl;
            return "";
        }


        code << "    mov rax, 1\n";
        code << "    mov rdi, 1\n";
        code << "    mov rdx, 1\n"; //lenght;
        code << "    syscall\n";

        //putting a newline
        code << "    push 10\n";
        code << "    mov rsi, rsp\n";
        code << "    syscall\n";
    }

    return code.str();
}
