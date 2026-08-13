#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "parser.h"
#include "codeGen.h"

using namespace std;

/*
for each statement:

    keyword = statement[0]

    if keyword == "var":
        handleVars(statement)

    else if keyword == "call":
        handleCalls(statement)
 */


vector <string> getCode(vector <vector<string>> &statements) {
    //call handlers
    stringstream code;
    stringstream declCode;
    for (vector <string> &statement: statements) {
        //getKeyword(statement);

        //im not sure if that would ever be the case, but wtv
        if (statement.empty()) {
            cerr << "statement is empty\n";
            return {};
        }

        if (statement[0] == "let") {
            code << handleVars(statement);
        }
        else if (statement[0] == "call") {
            code << handleCalls(statement);
        }
        else if (statement[0] == "letC") {
            vector <string> codes = handleComplex(statement);
            code << codes[1];
            declCode << codes[0];
        }
        //other keywords here

    }

    return {declCode.str(), code.str()};
}

//callHandlers -> calls getKeyword with arg of statement -> getKeyword calls the proper handler for the keyword

string codeGen(vector <vector<string>> &statements) {
    stringstream codeOut;
    vector <string> codes = getCode(statements);

    //complex datatypes
    codeOut << "section .bss\n";
    codeOut << codes[0];

    codeOut << "section .text\n";
    codeOut << "global _start\n";
    codeOut << "_start:\n";
    codeOut << codes[1];

    return codeOut.str();
}

int main() {
    ifstream inputFile("../file.txt");

    vector <string> words = getWords(inputFile);
    vector <vector<string>> statements = getStatements(words);

    inputFile.close();

    ofstream("../asem.asm") << codeGen(statements);

    return 0;
}

//