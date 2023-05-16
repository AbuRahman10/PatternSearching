//
// Created by Abobaker Rahman on 15/05/2023.
//
#include <fstream>
#include "DFA.h"
#include "RE.h"
#include "ENFA.h"
#include "iostream"
#include "Pattern.h"

using namespace std;


vector<string> Pattern::split(string &input)
{
    vector<string> woorden;
    string woord;
    for (int i = 0; i <= input.size(); i++)
    {
        if (i == input.size() - 1 and input[i] != ' ')
        {
            woord += input[i];
            woorden.push_back(woord);
        }
        else if (input[i] == '\n')
        {
            woorden.push_back(woord);
            woorden.push_back("\n");
            woord = "";
        }
        else if (input[i] != ' ')
        {
            woord += input[i];
        }
        else if (input[i] == ' ')
        {
            woorden.push_back(woord);
            woord = "";
        }
    }
    return woorden;
}

Pattern::Pattern(string inputtext) : inputtext(inputtext)
{
    woorden = split(inputtext);
}

void Pattern::searchPattern(string expression, Ui_MainWindow *ui)
{
    RE re(expression,'e');
    ENFA enfa = re.toENFA();
    DFA dfa = enfa.toDFA();
    DFA mindfa = dfa.minimize();
    bool foundOne = false;
    int line = 0;
    int i = 0;
    for (string woord : woorden)
    {
        if (woord == "\n")
        {
            line++;
            i = 0;
            continue;
        }
        bool accept = dfa.accepts(woord);
        if (accept)
        {
            string output_display = "Pattern found at line: " + to_string(line) + " and indexword: " + to_string(i) + "\n";
            ui->textBrowser->insertPlainText(QString::fromStdString(output_display));
            foundOne = true;
        }
        i++;
    }
    if (!foundOne)
    {
        ui->textBrowser->insertPlainText(QString::fromStdString("Pattern not found!\n"));
    }
}

void Pattern::searchPattern(string expression1, string expression2, bool constructie, Ui_MainWindow *ui)
{
    RE re1(expression1,'e');
    RE re2(expression2,'e');

    ENFA enfa1 = re1.toENFA();
    ENFA enfa2 = re2.toENFA();

    DFA dfa1 = enfa1.toDFA();
    DFA dfa2 = enfa2.toDFA();

    DFA dfa(dfa1,dfa2,constructie);
    DFA mindfa = dfa.minimize();
    bool foundOne = false;
    int line = 0;
    int i = 0;
    for (string woord : woorden)
    {
        if (woord == "\n")
        {
            line++;
            i = 0;
            continue;
        }
        bool accept = dfa.accepts(woord);
        if (accept)
        {
            string output_display = "Pattern found at line: " + to_string(line) + " and indexword: " + to_string(i) + "\n";
            ui->textBrowser->insertPlainText(QString::fromStdString(output_display));
            foundOne = true;
        }
        i++;
    }
    if (!foundOne)
    {
        ui->textBrowser->insertPlainText(QString::fromStdString("Pattern not found!\n"));
    }
}
