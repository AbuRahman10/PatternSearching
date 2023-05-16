//
// Created by Abobaker Rahman on 4/03/2023.
//

#ifndef TA_DFA_H
#define TA_DFA_H

#include "iostream"
#include "json.hpp"
#include "set"

using json = nlohmann::json;

using namespace std;

class DFA
{
private:
    set<vector<string>> states;
    string input;
    json dfa;
    string start;
    string current;
    string end;
    vector<string> finalStates;

    json dfa1;
    json dfa2;
    bool prod;

    string input1;
    string input2;

    json min_dfa;
    vector<string> stateNames;
    map<string, bool> starting;
    map<string, bool> accepting;
    vector<int> s;
    map<vector<string>, bool> table;
public:
    DFA(DFA &dfa1, DFA &dfa2, bool constructie);
    DFA(const string &input);
    bool accepts(string input);
    bool inAlphabet(string letter);
    void productautomaat(const string &file1,const string &file2);
    string startState(json automata);
    string stateString(vector<string> state);
    vector<bool> start_accept(vector<string> state);
    void transitionTable(vector<string> state);
    vector<string> transitionState(vector<string> state, string inp);
    DFA minimize();
    void makeStates(vector<vector<string>> const &vec);
    bool acceptCheck(string name1, string name2);
    void print();
    void printTable();
};

#endif //TA_DFA_H
