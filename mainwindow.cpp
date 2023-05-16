#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPalette>
#include <iostream>
#include <QMessageBox>
#include "fstream"
#include "Pattern.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Pattern Searching");
    QPalette my_color;
    my_color.setColor(QPalette::WindowText,Qt::white);
    // set my backgroundcolor of the window
    my_color.setColor(QPalette::Window,Qt::darkCyan);
    // all buttontextcolor will change to gray
    my_color.setBrush(QPalette::ButtonText,Qt::gray);
    // all written text will be of this color !
    my_color.setBrush(QPalette::Text,Qt::magenta);
    // set this palette as default
    this->setPalette(my_color);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // First fetch all the information out of your inputs !
    string checking_text = ui->givingtext_edit->toPlainText().toStdString();
    // get the 2 regexes
    string regex_one = ui->Regexone_edit->text().toStdString();
    string regex_two = ui->Regextwo_edit->text().toStdString();
    // checking for union or intersection
    bool checked_union = ui->union_radio->isChecked();
    bool checked_intersection = ui->intersection_radio->isChecked();
    // check for ERROR CASES
    exceptions(regex_one,regex_two,checking_text,checked_union,checked_intersection);
    // ONDERSCHEID DE GEVALLEN

    //GEVAL 1: WE GAAN PATTERNSEARCHING DOEN ADHV EERSTE BOX EN TEXT DIE WE HEBBEN
    if(!regex_one.empty() and !checking_text.empty() and regex_two.empty())
    {
        Pattern pattern(checking_text);
        ui->textBrowser->clear();
        pattern.searchPattern(regex_one,ui);
    }
    else if (!regex_one.empty() and !checking_text.empty() and !regex_two.empty() and (checked_intersection or checked_union))
    {
        Pattern pattern(checking_text);
        bool constructie = checked_intersection;
        ui->textBrowser->clear();
        pattern.searchPattern(regex_one,regex_two,constructie,ui);
    }
}

void MainWindow::exceptions(std::string& state_R1,std::string& state_R2, std::string& input_text,bool is_union,bool is_intersection){
    // CASE 1: IF FIRST REGEXBOX IS EMPTY THEN WE CAN'T DO ANYTHING !
    if(state_R1.empty()){
        QMessageBox::warning(this,"ERROR","No operations can be executed as long as the first regexbox isn't filled");
    }
    // CASE 2: IF FIRST REGEXBOX IS NOT EMPTY BUT THE SEARCHING TEXT IS EMPTY
    else if((!state_R1.empty() && input_text.empty())){
        QMessageBox::warning(this,"ERROR","There is no checking text to do the searching !");
    }

    // FROM HERE THIS MEANS THERE IS SOMETHING ELSE WRONG WHEN WE ARE DOING OPERATIONS WITH 2 REGEXES

    // CASE 3: UNION AND INTERSECTION OPTIONS ARE BOTH CROSSED
    else if(is_union and is_intersection){
        QMessageBox::warning(this,"ERROR","We cannot do 2 operations on the same time!");
    }
    // CASE 4: REGEX2BOX IS EMPTY WHEN SELECTING UNION OR INTERSECTION OPERATION
    else if(!state_R1.empty() and state_R2.empty() and !input_text.empty() and (is_union or is_intersection)){
        QMessageBox::warning(this,"ERROR","You have to fill up the second regexbox to do union or intersection");
    }
    // CASE 5: IF EVERYTHING IS NOT EMPTY BUT UNION AND INTERSECTION ARE NOT CHECKED
    else if(!input_text.empty() and !state_R1.empty() and !state_R2.empty() and (!is_union and !is_intersection)){
        QMessageBox::warning(this,"ERROR","You need to check the Union or Intersection option if you want to use 2 Regex boxes!");

    }
}







