#include "parser.h"
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

int countWords(ifstream &inputFile) {

    int wordCount = 0;

    if (inputFile.is_open()) {
        string line;

        while (getline(inputFile, line)) {
            stringstream ss(line);
            string word;

            while (ss >> word) {
                wordCount++;
            }
        }

    }

    return wordCount;
}

vector <string> getWords(ifstream &inputFile) {
    vector <string> words = {};

    int lenght = countWords(inputFile); //counts the words, and with that gets to the eof

    //Alternative to closing and opening the file again. It goes back to the first position to get words
    //after already getting the specified length
    inputFile.clear();
    inputFile.seekg(0);

    for (int i = 0; i < lenght; i++) {
        string buff;

        inputFile >> buff;
        words.push_back(buff);
    }

    return words;
}

//Checks if theres a semicolon at the end of each word in the words vector, returns the index
//of the word the semicolon is at the end of, in the vector "words".

vector <int> checkEOS(vector <string> &words) {
    vector <int> eosIndices = {};
    for (int i = 0; i < words.size(); i++) {
        string checkWord = words[i];

        // char lastChar = checkWord.back();
        // if (lastChar == ';') {
        //     eosIndices.push_back(i);
        // }

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