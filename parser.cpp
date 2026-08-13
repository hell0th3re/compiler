#include "parser.h"
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

//switched the tokeniser to use next chars not next whitespace so you can actually write a space in a char
vector<string> getWords(ifstream &inputFile) {
    vector<string> words;
    string current;
    char c;
    bool inChar = false;

    while (inputFile.get(c)) {
        if (c == '\'') {
            current += c;
            inChar = !inChar;
        }

        //operators
        else if ((c == '=' || c == ':' || c == '[' || c == ']' ||
            c == '^' || c == ',' || c == ';' || c == ')' || c == '(' ||
            c == '+') && !inChar) {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }

            words.push_back(string(1, c));
        }
        else if (isspace(static_cast<unsigned char>(c)) && !inChar) {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    if (!current.empty()) {
        words.push_back(current);
    }

    return words;
}

//splits the words into statements and returns a vector of them
vector<vector<string>> getStatements(vector<string>& words) {
    vector<vector<string>> statements;
    vector<string> current;

    for (const string& word : words) {
        current.push_back(word);

        if (word == ";") {
            statements.push_back(current);
            current.clear();
        }
    }

    return statements;
}
//Parser: splits the program into statements