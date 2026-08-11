#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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

/*
for each statement:

    keyword = statement[0]

    if keyword == "var":
        handleVars(statement)

    else if keyword == "call":
        handleCalls(statement)
 */

//to pass by reference, or to not pass by reference? that is thy question.
void handleVars(vector <string> statement) {
    //assembly generation for vars here
}

void getKeyword(vector <string> statement) {

    //im not sure if that would ever be the case, but wtv
    if (statement.empty()) {
        return; //aparently you can do that
    }

    if (statement[0] == "let") {
        handleVars(statement);
    }
    //other keywords here
}


void callHandlers(vector <vector<string>> &statements) {
    //thy question remains
    for (vector statement: statements) {
        getKeyword(statement);
    }
}

//callHandlers -> calls getKeyword with arg of statement -> getKeyword calls the proper handler for the keyword


string checkForExit(vector <vector<string>> &statements) {

    stringstream codeOut;
    for (int i = 0; i < statements.size(); i++) {
        if (statements[i][0] == "exit") {
            string errNr = statements[i][1];
            errNr.pop_back(); //removes the semicolon

            codeOut << "    mov rax, 60\n";
            codeOut << "    mov rdi, " << errNr << "\n";
            codeOut << "    syscall\n";
        }
    }
    return codeOut.str(); //why .str()?
}

string codeGen(vector <vector<string>> &statements) {
    stringstream codeOut;
    codeOut << "global _start\n";
    codeOut << "_start:\n";
    //things here
    codeOut << checkForExit(statements);

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

// To recap: so far this does about fuckall. I'll put the code in different files
// after i sleep to not make an even bigger mess.
