//
// Created by abobaker on 26/03/2021.
//

#include "ENFA.h"
#include "json.hpp"
#include <fstream>
#include <map>
#include <iomanip>


using namespace std;
using json = nlohmann::json;


ENFA::ENFA(string p)
{
    path = p;
    ifstream input(path);
    input >> enfa;
    eps = enfa["eps"];
}


void ENFA::nextNodes(vector<int> *nodes, string input)
{
    vector<int> new_states;
    for (auto node : *nodes)
    {
        for (auto transition : enfa["transitions"])
        {
            if (transition["from"] == node && transition["input"] == input && count(new_states.begin(), new_states.end(), transition["to"]) == 0)
            {
                new_states.push_back(transition["to"]);
            }
        }
    }
    *nodes = new_states;
}


void ENFA::tryEps(vector<int> *nodes)
{
    for (auto node : *nodes)
    {
        for (auto transition : enfa["transitions"])
        {
            if (transition["from"] == node && transition["input"] == eps && count(nodes->begin(), nodes->end(), transition["to"]) == 0)
            {
                nodes->push_back(transition["to"]);
                return tryEps(nodes);
            }
        }
    }
}


bool ENFA::accepts(string input)
{
    vector<int> states = {0};
    tryEps(&states);
    for (auto c : input)
    {
        string character(1, c);
        nextNodes(&states, character);
        tryEps(&states);
    }
    return (count(states.begin(), states.end(), enfa["states"].size() - 1) == 1);
}

bool ENFA::accept(vector<string> new_state)
{
    bool accepting = false;
    for (int i = 0; i < enfa["states"].size(); i++)
    {
        if (enfa["states"][i]["accepting"] == true)
        {
            if (count(new_state.begin(), new_state.end(), enfa["states"][i]["name"]))
            {
                accepting = true;
            }
        }
    }
    return accepting;
}


string ENFA::vecToString(vector<string> new_state)
{
    if (new_state.empty())
    {
        return "{}";
    }
    string name = "{" + new_state[0];
    for (int i = 1; i < new_state.size(); i++)
    {
        name += ",";
        name += new_state[i];
    }
    name += "}";
    return name;
}


void ENFA::addState(string name, bool starting, bool accepting)
{
    dfa["states"].push_back({{"name", name},{"starting", starting},{"accepting", accepting}});
}

void ENFA::addTransition(string from, string to, string input)
{
    dfa["transitions"].push_back({{"from", from},{"to", to},{"input", input}});
}

vector<string> ENFA::findTransition(vector<string> state, string input)
{
    vector<string> new_state;
    for (int i = 0; i < enfa["transitions"].size(); i++)
    {
        if (count(state.begin(), state.end(), enfa["transitions"][i]["from"]) && enfa["transitions"][i]["input"] == input)
        {
            new_state.push_back(enfa["transitions"][i]["to"]);
        }
    }
    return new_state;
}

vector<string> ENFA::tryEpsilon(vector<string> state1)
{
    vector<string> new_state = state1;
    for (auto transition : enfa["transitions"])
    {
        if (count(state1.begin(), state1.end(), transition["from"]) && transition["input"] == eps)
        {
            new_state.push_back(transition["to"]);
        }
    }
    sort(new_state.begin(), new_state.end());
    new_state.erase(unique(new_state.begin(), new_state.end()), new_state.end());
    if (new_state == state1)
    {
        return new_state;
    }
    else
    {
        return tryEpsilon(new_state);
    }
}

void ENFA::subsetConstruction(vector<string> const &state)
{
    vector<vector<string>> states;
    if (allStates.find(state) != allStates.end())
        return;
    allStates.insert(state);

    for (auto const &alph : dfa["alphabet"])
    {
        vector<string> new_state = tryEpsilon(findTransition(state, alph));
        states.push_back(new_state);
        addTransition(vecToString(state), vecToString(new_state), alph);
    }
    for (auto const &s : states)
    {
        subsetConstruction(s);
    }
}

DFA ENFA::toDFA()
{
    vector<string> startState;
    dfa =
    {
        {"type",     "DFA"},
        {"alphabet", enfa["alphabet"]}
    };

    for (int i = 0; i < enfa["states"].size(); i++)
    {
        if (enfa["states"][i]["starting"] == true)
        {
            startState = {enfa["states"][i]["name"]};
            startState = tryEpsilon(startState);
            dfa["states"] = {};
        }
    }
    dfa["transitions"] = {};
    subsetConstruction(startState);
    for (auto const &elem : allStates)
    {
        if (elem == startState)
        {
            addState(vecToString(elem), true, accept(elem));
        }
        else
        {
            addState(vecToString(elem), false, accept(elem));
        }
    }
    ofstream file(output);
    file << dfa;
    file.close();
    return DFA(output);
}

void ENFA::print()
{
    cout << setw(4) << enfa << endl;
}

ENFA::ENFA() {}
