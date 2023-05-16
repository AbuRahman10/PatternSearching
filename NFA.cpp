//
// Created by aboba on 24/04/2023.
//

#include "NFA.h"
#include "fstream"

NFA::NFA(string string1)
{
    input = string1;
    ifstream in(input);
    in >> nfa;
}

DFA NFA::toDFA()
{
    dfa["type"] = "DFA";
    dfa["alphabet"] = nfa["alphabet"];

    vector<string> startstate;
    for (int i = 0; i < nfa["states"].size();i++)
    {
        if (nfa["states"][i]["starting"] == true)
        {
            startstate = {nfa["states"][i]["name"]};
            dfa["states"] = {"",""};
        }
    }
    dfa["transitions"] = {"",""};
    sscTransitionTable(startstate); // alle staten gevormd voor DFA en transitie ook uitgevoerd
    for (vector<string>const &st : states)
    {
        if (st == startstate)
        {
            dfa["states"].push_back({{"name", stateString(st)},{"starting", true},{"accepting", isFinal(st)}});
        }
        else
        {
            dfa["states"].push_back({{"name", stateString(st)},{"starting", false},{"accepting", isFinal(st)}});
        }
    }
    string clean_input;
    for (char i : input)
    {
        if (i == '.')
        {
            break;
        }
        clean_input += i;
    }
    dfa["states"].erase(dfa["states"].begin());
    dfa["states"].erase(dfa["states"].begin());
    dfa["transitions"].erase(dfa["transitions"].begin());
    dfa["transitions"].erase(dfa["transitions"].begin());
    string inputfile = clean_input + "_to_DFA.json";
    ofstream file(inputfile);
    file << dfa;
    file.close();
    return DFA(inputfile);
}

void NFA::sscTransitionTable(vector<string> state)
{
    vector<vector<string>> statesByInputs; //de staten gekregen door de inputs te voeren op een bepaalde staat (zoals de transitietabel)
    if (inStates(state)) // checken of een bepaalde staat al gecreëerd is of niet voor de nieuwe DFA
    {
        return;
    }
    states.insert(state);

    for (string const &inp : dfa["alphabet"]) // de transitie uitvoeren op een bepaalde staat
    {
        statesByInputs.push_back(transitionState(state,inp));
    }
    for (vector<string>const &i : statesByInputs) // recursief op elke staat dat gevormd wordt door verdere transitie
    {
        sscTransitionTable(i);
    }
}

bool NFA::inStates(const vector<string> &state)
{
    if (states.find(state) != states.end())
    {
        return true;
    }
    return false;
}

string NFA::stateString(vector<string> state)
{
    string staat = "{";
    for (int i = 0; i < state.size(); i++)
    {
        if (i == state.size()-1)
        {
            staat += state[i];
        }
        else
        {
            staat += state[i] + ",";
        }
    }
    staat += "}";
    return staat;
}

bool NFA::isFinal(vector<string> state)
{
    bool final = false;
    for (int i = 0; i < nfa["states"].size(); i++)
    {
        if (nfa["states"][i]["accepting"] == true)
        {
            if (count(state.begin(), state.end(), nfa["states"][i]["name"]))
            {
                final = true;
            }
        }
    }
    return final;
}

vector<string> NFA::transitionState(vector<string> state, string inp)
{
    vector<string> inpState;
    for (int i = 0; i < nfa["transitions"].size(); i++)
    {
        if (count(state.begin(), state.end(), nfa["transitions"][i]["from"]))
        {
            if (nfa["transitions"][i]["input"] == inp)
            {
                bool dublicate = false;
                for (string s : inpState)
                {
                    if (s == nfa["transitions"][i]["to"])
                    {
                        dublicate = true;
                    }
                }
                if (dublicate == false)
                {
                    inpState.push_back(nfa["transitions"][i]["to"]);
                }
            }
        }
    }
    sort(inpState.begin(), inpState.end());
    dfa["transitions"].push_back({{"from", stateString(state)},{"to", stateString(inpState)},{"input",inp}});
    return inpState;
}
