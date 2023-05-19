//
// Created by Abobaker Rahman on 15/05/2023.
//
#include <fstream>
#include "DFA.h"
#include "RE.h"
#include "ENFA.h"
#include "iostream"
#include "Pattern.h"
#include <qstring.h>
#include <qtextdocument.h>
#include <qtextformat.h>
#include <qtextcursor.h>
#include <qtextobject.h>

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
    re.output = "output/enfa.json";
    ENFA enfa = re.toENFA();
    enfa.output = "output/mssc.json";
    DFA dfa = enfa.toDFA();
    dfa.output = "output/dfa.json";
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
        bool accept = mindfa.accepts(woord);
        QString highlight_word = QString::fromStdString(woord);
        QTextDocument* document = ui->givingtext_edit->document();
        QTextCursor cursor(document);
        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(Qt::green);

        if (accept)
        {
            string output_display = "Pattern found at line: " + to_string(line) + " and indexword: " + to_string(i) + "\n";
            ui->textBrowser->insertPlainText(QString::fromStdString(output_display));
/*            // Remove the previous highlighting by resetting the format for the entire document
            QTextCursor clearCursor(document);
            clearCursor.select(QTextCursor::Document);
            clearCursor.setCharFormat(QTextCharFormat());*/

            cursor.movePosition(QTextCursor::StartOfLine);
            int targetline = line;
            QTextBlock chosenBlock;

            if (targetline < document->blockCount()){
                chosenBlock = document->findBlockByLineNumber(targetline);
            }else{
                // TARGET LINE  IS OUT OF RANGE
                break;
            }
            // This will be the block where we will search our word
            QTextCursor lineCursor(chosenBlock);
            // keeping the counter of x_coordinate
            int teller_x = 0;
            // Iterate through each word in the line
            while (!lineCursor.atEnd()) {
                lineCursor.select(QTextCursor::WordUnderCursor);
                QString highlightedWord = lineCursor.selectedText();
                std::string checkWord = highlightedWord.toStdString();

                if (checkWord != woord or teller_x != i){
                    lineCursor.movePosition(QTextCursor::NextWord);
                    teller_x += 1;
                    continue;
                }
                teller_x = 0;
                // Erase the inside spaces
                checkWord.erase(std::remove_if(checkWord.begin(), checkWord.end(), [](char c) { return std::isspace(c); }), checkWord.end());
                if (checkWord == woord) {
                    lineCursor.mergeCharFormat(highlightFormat);
                    break;
                }
            }
            foundOne = true;
        }

        i++;
    }
    if (!foundOne)
    {
        ui->textBrowser->insertPlainText(QString::fromStdString("Pattern not found!\n"));
        // Remove the previous highlighting by resetting the format for the entire document
        QTextDocument* document = ui->givingtext_edit->document();
        QTextCursor clearCursor(document);
        clearCursor.select(QTextCursor::Document);
        clearCursor.setCharFormat(QTextCharFormat());
    }
}

void Pattern::searchPattern(string expression1, string expression2, bool constructie, Ui_MainWindow *ui)
{
    RE re1(expression1,'e');
    RE re2(expression2,'e');

    re1.output = "output/enfa1.json";
    re2.output = "output/enfa2.json";

    ENFA enfa1 = re1.toENFA();
    ENFA enfa2 = re2.toENFA();

    enfa1.output = "output/mssc1.json";
    enfa2.output = "output/mssc2.json";

    DFA dfa1 = enfa1.toDFA();
    DFA dfa2 = enfa2.toDFA();

    dfa1.output = "output/dfa1.json";
    dfa2.output = "output/dfa2.json";

    //DFA dfa(dfa1,dfa2,constructie);
    DFA mindfa1 = dfa1.minimize();
    DFA mindfa2 = dfa2.minimize();

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
        bool accept1 = mindfa1.accepts(woord);
        bool accept2 = mindfa2.accepts(woord);
        if (!constructie)
        {
            if (accept1 or accept2)
            {
                string output_display = "Pattern found at line: " + to_string(line) + " and indexword: " + to_string(i) + "\n";
                ui->textBrowser->insertPlainText(QString::fromStdString(output_display));
                foundOne = true;
            }
        }
        else if (constructie)
        {
            if (accept1 and accept2)
            {
                string output_display = "Pattern found at line: " + to_string(line) + " and indexword: " + to_string(i) + "\n";
                ui->textBrowser->insertPlainText(QString::fromStdString(output_display));
                foundOne = true;
            }
        }
        i++;
    }
    if (!foundOne)
    {
        ui->textBrowser->insertPlainText(QString::fromStdString("Pattern not found!\n"));
        // Remove the previous highlighting by resetting the format for the entire document
        QTextDocument* document = ui->givingtext_edit->document();
        QTextCursor clearCursor(document);
        clearCursor.select(QTextCursor::Document);
        clearCursor.setCharFormat(QTextCharFormat());
    }
}
