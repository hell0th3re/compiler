#ifndef UNTITLED_PARSER_H
#define UNTITLED_PARSER_H

#include <iosfwd>
#include <vector>

using namespace std;

vector <string> getWords(ifstream &inputFile);
vector <int> checkEOS(vector <string> &words);
vector <string> getSlice(vector <string> vec, int start, int end);
vector<vector<string>> getStatements(vector<string> &words);

#endif //UNTITLED_PARSER_H
