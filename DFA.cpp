//
// Created by Abobaker Rahman on 4/03/2023.
//

#include "DFA.h"
#include "fstream"
#include <iomanip>

bool DFA::accepts(string data)
{
    for (int i = 0; i < data.size(); ++i)
    {
        string str;
        str += data[i];
        if (!inAlphabet(str))
        {
            current = start;
            return false;
        }
        for (int k = 0; k < dfa["transitions"].size(); ++k)
        {
            string from = dfa["transitions"][k]["from"];
            string cr = current;
            string input = dfa["transitions"][k]["input"];
            if (dfa["transitions"][k]["from"] == current and dfa["transitions"][k]["input"] == str)
            {
                current = dfa["transitions"][k]["to"];
                break;
            }
        }
    }
    for (string state: finalStates)
    {
        if (current == state) //als je bij jouw final state bent
        {
            current = start;
            return true;
        }
    }
    current = start;
    return false;
}

DFA::DFA(const string &input) : input(input)
{
    ifstream fout(input);
    fout >> dfa;
    for (int i = 0; i < dfa["states"].size(); ++i)
    {
        if (dfa["states"][i]["starting"] == true)
        {
            start = dfa["states"][i]["name"];
            current = start;
        }
        if (dfa["states"][i]["accepting"] == true)
        {
            finalStates.push_back(dfa["states"][i]["name"]);
        }
    }
}

DFA::DFA(DFA &dfa1, DFA &dfa2, bool constructie)
{
    input1 = dfa1.input;
    input2 = dfa2.input;
    prod = constructie;
    productautomaat(input1,input2);
}

void DFA::productautomaat(const string &file1, const string &file2)
{
    // dfa1 en dfa2 worden aangevuld met de twee gegeven json (DFA) files
    ifstream inp1(file1);
    inp1 >> dfa1;
    ifstream inp2(file2);
    inp2 >> dfa2;
    j["type"] = "DFA";
    set<string> alphabets;
    for (string letter : dfa1["alphabet"])
    {
        alphabets.insert(letter);
    }
    for (string letter : dfa2["alphabet"])
    {
        alphabets.insert(letter);
    }
    j["alphabet"] = alphabets;
    j["states"] = {};
    j["transitions"] = {};
    vector<string> startstate;
    startstate.push_back(startState(dfa1));
    startstate.push_back(startState(dfa2));
    transitionTable(startstate);
    vector<bool> construction;
    for (vector<string> st : states)
    {
        construction = start_accept(st);
        bool starting = construction[0];
        bool accepting = construction[1];
        j["states"].push_back({{"name", stateString(st)},{"starting", starting},{"accepting", accepting}});
    }
    ofstream file("output/product.json");
    file << j;
    file.close();
    input = "output/product.json"; //json dfa gaat dit bestand openen
}

void DFA::print()
{
    cout << setw(4) << dfa << endl;
}

string DFA::startState(json automata)
{
    string startstate = "Phi";
    for (int i = 0; i < automata["states"].size(); i++)
    {
        if (automata["states"][i]["starting"] == true)
        {
            startstate = automata["states"][i]["name"];
        }
    }
    return startstate;
}

void DFA::transitionTable(vector<string> state)
{
    vector<vector<string>> inpStates;

    if (states.find(state) != states.end() or state.empty())
    {
        return;
    }
    states.insert(state);

    for (string const &inp : dfa["alphabet"])
    {
        inpStates.push_back(transitionState(state,inp));
    }

    for (vector<string> st : inpStates)
    {
        transitionTable(st);
    }
}

vector<string> DFA::transitionState(vector<string> state, string inp)
{
    vector<string> inpState;
    for (int i = 0;i < dfa1["transitions"].size();i++)
    {
        bool toegevoegd = false;
        if (dfa1["transitions"][i]["from"] == state[0] and dfa1["transitions"][i]["input"] == inp)
        {
            inpState.push_back(dfa1["transitions"][i]["to"]);
            toegevoegd = true;
        }
    }
    for (int i = 0;i < dfa2["transitions"].size();i++)
    {
        bool toegevoegd = false;
        if (dfa2["transitions"][i]["from"] == state[1] and dfa2["transitions"][i]["input"] == inp)
        {
            inpState.push_back(dfa2["transitions"][i]["to"]);
            toegevoegd = true;
        }
    }


    string from = stateString(state);
    string to = stateString(inpState);
    string input = inp;
    dfa["transitions"].push_back({{"from", stateString(state)}, {"to",stateString(inpState)}, {"input", inp}});
    return inpState;
}

string DFA::stateString(vector<string> state)
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

vector<bool> DFA::start_accept(vector<string> state)
{
    vector<bool> state1;
    vector<bool> state2;
    vector<bool> st_fin;

    for (int i = 0; i < dfa1["states"].size(); i++)
    {
        if (dfa1["states"][i]["name"] == state[0])
        {
            state1.push_back(dfa1["states"][i]["starting"]);
            state1.push_back(dfa1["states"][i]["accepting"]);
        }
    }
    for (int i = 0; i < dfa2["states"].size(); i++)
    {
        if (dfa2["states"][i]["name"] == state[1])
        {
            state2.push_back(dfa2["states"][i]["starting"]);
            state2.push_back(dfa2["states"][i]["accepting"]);
        }
    }
    if (prod == true)
    {
        if (state1[0] == true and state2[0] == true)
        {
            st_fin.push_back(true);
        }
        else
        {
            st_fin.push_back(false);
        }
        if (state1[1] == true and state2[1] == true)
        {
            st_fin.push_back(true);
        }
        else
        {
            st_fin.push_back(false);
        }
    }
    else
    {
        if (state1[0] == true and state2[0] == true)
        {
            st_fin.push_back(true);
        }
        else
        {
            st_fin.push_back(false);
        }
        if (state1[1] == true or state2[1] == true)
        {
            st_fin.push_back(true);
        }
        else
        {
            st_fin.push_back(false);
        }
    }
    return st_fin;
}

DFA DFA::minimize()
{
    //elke state naam wordt in deze vector gezet.
    vector <string> names;
    for (auto i: dfa["states"])
    {
        names.push_back(i["name"]);
    }
    sort(names.begin(), names.end());

    //elke mogelijke combinatie van 2 states wordt hier gemaakt en gezet in een vector.
    vector <vector<string>> arr;
    vector <string> arrrr;
    for (auto i: names)
    {
        for (auto k: names)
        {
            if (i != k) {
                vector <string> tt;
                tt.push_back(i);
                tt.push_back(k);

                string comb;
                comb += i;
                comb += k;
                sort(comb.begin(), comb.end());

                int dupe = 0;
                for (auto l: arrrr)
                {
                    if (l == comb)
                    {
                        dupe = 1;
                    }
                }
                if (dupe == 0)
                {
                    arrrr.push_back(comb);
                    sort(tt.begin(), tt.end());
                    arr.push_back(tt);
                }
            }
        }
    }


    //Hier maken wij de tabel aan zonder het te visualizeren.
    //In arr1 zijn alle combinaties waar nog geen kruisje door getrokken zijn.
    //En arr2 zijn alle combinaties waar een kruisje door getrokke is.
    vector <vector<string>> arr1;
    vector <vector<string>> arr2;

    bool acc1;
    bool acc2;

    //checken of dat een comb in arr1 moet of arr2.
    for (auto i: arr)
    {
        int counter = 0;
        vector <string> rr;
        string cop = "";
        for (auto k: i)
        {
            string temp = "";
            temp += k;
            rr.push_back(temp);
            cop += k;

            for (auto l: dfa["states"])
            {
                if (temp == l["name"] and counter == 0)
                {
                    acc1 = l["accepting"];
                    counter += 1;
                } else if (temp == l["name"] and counter == 1)
                {
                    acc2 = l["accepting"];
                }
            }
        }
        if (acc1 == acc2)
        {
            arr1.push_back(rr);
        } else
        {
            arr2.push_back(rr);
        }
    }


    //states uit arr1 halen(dus een kruisje geven) en in arr2 zetten a.d.h.v. de transitiefunctie.
    bool rec = true;
    while(rec)
    {
        int index = 0;
        for (auto i: arr1)
        {
            //cout << "eerste loop: " << i << endl;
            int counter2 = 0;
            for (auto a: dfa["alphabet"])
            {
                string newComb = "";
                vector <string> zz;
                vector <string> ff;
                string hala = "";
                for (auto k: i)
                {
                    newComb = "";
                    string temp = "";
                    temp += k;
                    zz.push_back(temp);
                    hala += k;
                    for (auto l: dfa["transitions"])
                    {
                        if (temp == l["from"] and counter2 == 0 and l["input"] == a)
                        {
                            newComb += l["to"];
                            ff.push_back(newComb);
                            counter2 += 1;
                        } else if (temp == l["from"] and counter2 == 1 and l["input"] == a)
                        {
                            newComb += l["to"];
                            ff.push_back(newComb);

                        }
                    }
                }
                //hier halen wij de states uit arr1 en zetten wij ze in arr2.
                for (auto k: arr2)
                {
                    int counter3 = 0;
                    for (auto l: k)
                    {
                        string vartje = "";
                        vartje += l;
                        for (auto op: ff)
                        {
                            string vartje1 = "";
                            vartje1 += op;
                            if (vartje == vartje1)
                            {
                                counter3 += 1;
                                break;
                            }
                        }
                    }
                    if (counter3 == 2)
                    {
                        arr2.push_back(zz);
                        arr1[index].clear();
                        break;
                    }
                }
            }
            //een variable die ik gebruik.
            index += 1;
        }
        vector<vector<string>> temporor;
        for (auto i : arr1)
        {
            if (i.size() > 0)
            {
                temporor.push_back(i);
            }
        }
        if(arr1.size() == temporor.size())
        {
            rec = false;
        }
        else
        {
            arr1.clear();
            for (auto i : temporor)
            {
                arr1.push_back(i);
            }
        }
    }

    //alle states die nog niet gekruist zijn worden in een nieuwe vector gezet.
    vector<vector<string>> finalVector;
    for (auto i : arr1)
    {
        finalVector.push_back(i);
    }

    //alle states combineren en de single states toevoegen.

    bool stop = false;
    vector<vector<string>> aangepast;


    while(!stop and finalVector.size() > 0)
    {
        string temp = "";
        vector<string> tempo;
        int index2 = 0;
        int erin = 1;
        for (auto i: finalVector)
        {
            vector <vector<string>> pp;
            if(index2 == 0)
            {
                pp.push_back(i);
            }
            if (index2 > 0)
            {
                vector<string> hah;
                for (auto k : tempo)
                {
                    string var1 = "";
                    var1 += k;
                    for (auto l : i)
                    {
                        string var2 = "";
                        var2 += l;
                        if (var1 == var2)
                        {
                            for(auto s : tempo)
                            {
                                string var3 = "";
                                var3 += s;
                                hah.push_back(var3);
                            }
                            for(auto d : i)
                            {
                                string var4 = "";
                                var4 += d;
                                hah.push_back(var4);
                            }
                            pp.push_back(hah);
                            erin = 1;
                        }
                    }
                }

            }
            if (erin == 1)
            {
                tempo = pp[0];
                erin = 0;
            }
            index2 += 1;
        }

        //Verwijderen van duplicates.
        auto end = tempo.end();
        for (auto it = tempo.begin(); it != end; ++it)
        {
            end = std::remove(it + 1, end, *it);
        }

        tempo.erase(end, tempo.end());



        //states uit finalVector halen
        int index3 = 0;
        for(auto i : finalVector)
        {
            for(auto k : i)
            {
                string var = "";
                var += k;
                for(auto l : tempo)
                {
                    string var0 = "";
                    var0 += l;
                    if(var == var0)
                    {
                        finalVector[index3].clear();
                    }
                }
            }
            index3 += 1;
        }
        vector<vector<string>> tempor;
        for(auto i : finalVector)
        {
            if(i.size() > 0)
            {
                tempor.push_back(i);
            }
        }
        finalVector.clear();
        for(auto i : tempor)
        {
            finalVector.push_back(i);
        }
        finalVector.push_back(tempo);


        if(finalVector.size() == aangepast.size())
        {
            stop = true;
        }
        aangepast.clear();
        for(auto i : finalVector)
        {
            aangepast.push_back(i);
        }
    }

    //single states toevoegen.
    int ctr = 0;
    for(auto i : names)
    {
        vector<string> bb;
        bb.push_back(i);
        string t = "";
        t += i;
        for(auto k : finalVector)
        {
            for(auto l : k)
            {
                string y = "";
                y += l;
                if(t == y)
                {
                    ctr += 1;
                }
            }
        }
        if(ctr == 0)
        {
            finalVector.push_back(bb);
        }
        ctr = 0;
    }

    vector<json> states;
    vector<json> transitions;

    //dfa states
    bool accept;
    bool start;
    for(auto i : finalVector)
    {
        for (auto k: dfa["states"])
        {
            json state;
            string ter = "";
            ter += i[0];

            if (i.size() == 1 and k["name"] == ter)
            {
                accept = k["accepting"];
                start = k["starting"];

                string name = "{";
                name += ter + "}";

                state["name"] = name;
                state["accepting"] = accept;
                state["starting"] = start;
                states.push_back(state);
                break;
            }
            else if (i.size() > 1 and k["name"] == ter)
            {
                accept = k["accepting"];
                start = k["starting"];

                int countertje2 = 0;
                string name2 = "{";
                for (auto m: i) {
                    name2 += m;
                    countertje2 += 1;
                    if (countertje2 != i.size())
                    {
                        name2 += ", ";
                    }
                }
                name2 += "}";
                state["name"] = name2;
                state["accepting"] = accept;
                state["starting"] = start;
                states.push_back(state);
                break;
            }
        }
    }

    //dfa transitions
    for(auto i : finalVector)
    {
        for (auto l: dfa["alphabet"])
        {
            for (auto k: dfa["transitions"])
            {
                string ter = "";
                ter += i[0];

                //single states
                if (i.size() == 1 and k["from"] == ter and k["input"] == l)
                {
                    json trans;
                    string name3 = "{";
                    name3 += ter + "}";
                    trans["from"] = name3;

                    string po = "";
                    po += k["to"];
                    vector <string> aa;

                    for (auto u: finalVector)
                    {
                        for (auto p: u)
                        {
                            string yu = "";
                            yu += p;
                            if (yu == po)
                            {
                                aa = u;
                            }
                        }
                    }

                    int countertje4 = 0;
                    string name4 = "{";
                    for (auto m: aa) {
                        name4 += m;
                        countertje4 += 1;
                        if (countertje4 != aa.size())
                        {
                            name4 += ", ";
                        }
                    }
                    name4 += "}";
                    trans["to"] = name4;
                    trans["input"] = l;
                    transitions.push_back(trans);
                }
                    //combined states
                else if (i.size() > 1 and k["from"] == ter and k["input"] == l)
                {
                    json trans;
                    int countertje5 = 0;
                    string name5 = "{";
                    for (auto m: i)
                    {
                        name5 += m;
                        countertje5 += 1;
                        if (countertje5 != i.size())
                        {
                            name5 += ", ";
                        }
                    }
                    name5 += "}";
                    trans["from"] = name5;
                    string top;
                    top += k["to"];


                    vector <string> ww;
                    for (auto h: finalVector)
                    {
                        for (auto m: h)
                        {
                            string tp = "";
                            tp += m;
                            if (top == tp)
                            {
                                ww = h;
                            }
                        }
                    }

                    int countertje6 = 0;
                    string name6 = "{";
                    for (auto m: ww)
                    {
                        name6 += m;
                        countertje6 += 1;
                        if (countertje6 != ww.size())
                        {
                            name6 += ", ";
                        }
                    }
                    name6 += "}";
                    trans["to"] = name6;
                    trans["input"] = l;
                    transitions.push_back(trans);

                }
            }
        }
    }

    //de dfa aan maken en meegeven als een json file.
    min_dfa["type"] = "DFA";
    min_dfa["alphabet"] = dfa["alphabet"];
    min_dfa["states"] = states;
    min_dfa["transitions"] = transitions;


    ofstream out(output);
    out << min_dfa;
    out.close();
    return DFA(output);
}

bool DFA::acceptCheck(string name1, string name2)
{
    vector<vector<string>> possibles;
    for(auto const &alph : dfa["alphabet"])
    {
        vector<string> posib(2);
        for(auto tran : dfa["transitions"])
        {
            if(tran["input"] == alph && tran["from"] == name1)
            {
                posib[0] = tran["to"];
            }
            else if (tran["input"] == alph && tran["from"] == name2)
            {
                posib[1] = tran["to"];
            }
        }
        possibles.push_back(posib);
    }

    for(auto const &p : possibles)
    {
        if(table[p])
        {
            return true;
        }
    }
    return false;
}

void DFA::makeStates(const vector<vector<string>> &vec)
{
    for(auto const &v : vec)
    {
        bool start = false;
        bool accept = false;
        string name = "{";
        for(auto const &elem : v)
        {
            if(starting[elem])
                start = true;

            if(accepting[elem])
                accept = true;

            name += elem + ",";
        }
        name = name.substr(0, name.size()-1);
        name += "}";
        min_dfa["states"].push_back({{"name", name},{"starting", start},{"accepting", accept}});
    }
}

bool DFA::inAlphabet(string letter)
{
    if (letter == "e")
    {
        return true;
    }
    vector<string> al = dfa["alphabet"];
    for (string lett : dfa["alphabet"])
    {
        if (letter == lett)
        {
            return true;
        }
    }
    return false;
}

void DFA::printTable()
{
    for(int x = 1; x < stateNames.size(); x++)
    {
        cout << stateNames[x];
        for(int y = 0; y < x; y++)
        {
            string first = stateNames[x];
            string second = stateNames[y];
            if(table[{first, second}])
            {
                cout << "\t" << "X";
            }
            else
            {
                cout << "\t" << "-";
            }
        }
        cout << endl;
    }
    cout << "\t";
    for(int i = 0; i < stateNames.size()-1; i++){cout << stateNames[i] << "\t";}
    cout << endl;
}
