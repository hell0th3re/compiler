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


string getCode(vector <vector<string>> &statements) {
    //call handlers
    stringstream code;
    for (vector <string> &statement: statements) {
        //getKeyword(statement);

        //im not sure if that would ever be the case, but wtv
        if (statement.empty()) {
            cerr << "statement is empty\n";
            return "";
        }

        if (statement[0] == "let") {
            code << handleVars(statement);
        }
        else if (statement[0] == "call") {
            code << handleCalls(statement);
        }
        //other keywords here

    }

    return code.str();
}

//callHandlers -> calls getKeyword with arg of statement -> getKeyword calls the proper handler for the keyword

//temporary function


string codeGen(vector <vector<string>> &statements) {
    stringstream codeOut;
    codeOut << "global _start\n";
    codeOut << "_start:\n";

    codeOut << getCode(statements);

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