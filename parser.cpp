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
        else if ((c == '=' || c == ':') && !inChar) {
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

//Checks if theres a semicolon at the end of each word in the words vector, returns the index
//of the word the semicolon is at the end of, in the vector "words".
vector <int> checkEOS(vector <string> &words) {
    vector <int> eosIndices = {};
    for (int i = 0; i < words.size(); i++) {
        string checkWord = words[i];

        if (!checkWord.empty() && checkWord.back() == ';') {
            eosIndices.push_back(i);
        }
    }
    return eosIndices;
}


vector <string> getSlice(vector <string> vec, int start, int end) {
    vector <string> sliced = {};
    for (int i = start; i < end; i++) {
        sliced.push_back(vec[i]);
    }
    return sliced;
}

//splits the words into statements and returns a vector of them
vector<vector<string>> getStatements(vector<string> &words) {
    vector<vector<string>> statements;
    vector<int> eosIndices = checkEOS(words);

    int start = 0;

    for (int eosIndex : eosIndices) {
        vector<string> statement = getSlice(words, start, eosIndex + 1);
        statements.push_back(statement);

        start = eosIndex + 1;
    }

    return statements;
}

//Parser: splits the program into statements