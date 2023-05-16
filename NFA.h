//
// Created by abobaker on 24/04/2023.
//

#ifndef TA_NFA_H
#define TA_NFA_H
#include "iostream"
#include "json.hpp"
#include "set"
#include "vector"
#include "DFA.h"

using namespace std;
using json = nlohmann::json;

class NFA
{
private:
    string input;
    json dfa;
    json nfa;
    set<vector<string>> states;

public:
    NFA(string string1);
    void sscTransitionTable(vector<string> state);
    bool inStates(const vector<string>& state);
    bool isFinal(vector<string> state);
    string stateString(vector<string> state);
    vector<string> transitionState(vector<string> state, string inp);
    DFA toDFA();
};


#endif //TA_NFA_H
