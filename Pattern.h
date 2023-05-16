//
// Created by Abobaker Rahman on 15/05/2023.
//

#ifndef TA_PATTERN_H
#define TA_PATTERN_H

#include <fstream>
#include "DFA.h"
#include "RE.h"
#include "ENFA.h"
#include "iostream"
#include "./ui_mainwindow.h"

using namespace std;

class Pattern
{
private:
    string inputtext;
    vector<string> woorden;
public:
    vector<string> split(string &input);

    void searchPattern(string expression, Ui_MainWindow *ui);

    void searchPattern(string expression1, string expression2, bool constructie);

    explicit Pattern(string inputtext);
};


#endif //TA_PATTERN_H
