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

string handleVars(vector <string> statement) {
    //syntax: let name = value : type;
    string code;
    varCount++;

    string name = statement[1];
    string value = statement[3];
    string typeStr = statement[5];

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
        char valueConv = value[0]; //implicit conversion from string to char (in my lang not c++)
        chars userVar(name, valueConv);
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

    string name = statement[1];
    string value = statement[2];
    value.pop_back();

    if (value.empty()) {
        cerr << "invalid syntax" << endl;
        return "";
    }

    //handle getval
    if (name == "getval") {
        code << callCode(name, value);
    }
    //handle exit
    else if (name == "exit") {
        code << callCode(name, value);
    }

    return code.str();
}

string callCode(string name, string value) {
    stringstream code;

    if (name == "getval") {
        //check if its a number (we want variables/strings)
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
    }

    else if (name == "exit") {
        string errNr = value;

        for (size_t i = 0; i < errNr.size(); i++) {
            if (!isdigit(errNr[i])) {
                // cerr << "need a number" << endl;
                // return "";
                if (varMap[errNr]) { //if its a variable name
                    //load from rax (assume the user called "getval")
                }
            }
        }

        code << "    mov rax, 60\n";
        code << "    mov rdi, " << errNr << "\n";
        code << "    syscall";
    }

    return code.str();
}
