//
// Created by abobaker on 26/03/2021.
//

#ifndef SSC_ENFA_H
#define SSC_ENFA_H
#include <string>
#include "DFA.h"
#include "json.hpp"
#include <map>
#include <set>

using namespace std;
using json = nlohmann::json;

class DFA;

class ENFA {
public:
    string path;
    json dfa;
    json enfa;
    string eps;
    set<vector<string>> allStates;
    string output;

    ENFA();

    explicit ENFA(string p);
    DFA toDFA();
    void subsetConstruction(vector<string> const &state);
    void addState(string name, bool starting, bool accepting);
    void addTransition(string from, string to, string input);
    vector<string> tryEpsilon(vector<string> state1);
    vector<string> findTransition(vector<string> state, string input);
    static string vecToString(vector<string> new_state);
    bool accept(vector<string> new_state);
    bool accepts(string input);
    void nextNodes(vector<int>* node, string input);
    void tryEps(vector<int>* nodes);
    void print();

};


#endif //SSC_ENFA_H
