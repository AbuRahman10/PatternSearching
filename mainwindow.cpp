#include "mainwindow.h"
#include "../ui_mainwindow.h"
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
    // checking the haakjes
    // Need to check the brackets if they are equal
    auto open_haakje_R1 = std::count(regex_one.begin(),regex_one.end(),'(');
    auto sluitend_haakje_R1 = std::count(regex_one.begin(),regex_one.end(),')');
    auto open_haakje_R2 = std::count(regex_two.begin(),regex_two.end(),'(');
    auto sluitend_haakje_R2 = std::count(regex_two.begin(),regex_two.end(),')');
    if (open_haakje_R1 != sluitend_haakje_R1){
        QMessageBox::warning(this,"ERROR","For every opening bracket, there is no closing bracket in the first regexbox !");
        ui->Regexone_edit->clear();
    }
    if (open_haakje_R2 != sluitend_haakje_R2){
        QMessageBox::warning(this,"ERROR","For every opening bracket, there is no closing bracket in the second regexbox !");
        ui->Regextwo_edit->clear();
    }
    // ONDERSCHEID DE GEVALLEN
    //GEVAL 1: WE GAAN PATTERNSEARCHING DOEN ADHV EERSTE BOX EN TEXT DIE WE HEBBEN
    if(!ui->Regexone_edit->text().isEmpty() and !checking_text.empty() and ui->Regextwo_edit->text().isEmpty())
    {
        // detection for space chars !
        auto it = std::find(regex_one.begin(),regex_one.end(),' ');
        if (it != regex_one.end()){
            QMessageBox::warning(this,"SPACE CHAR DETECTION","There cannot be any space characters in the regexboxes!");
            //clearing that typed regex
            ui->Regexone_edit->clear();
        }else{
            // We can do the pattern search
            Pattern pattern(checking_text);
            ui->textBrowser->clear();
            pattern.searchPattern(regex_one,ui);
        }
    }
        // GEVAL 2: WE GAAN PATTERNSEARCHING DOEN ADHV AANGEDUIDE OPERATIE EN 2 INVOER REGEXES
    else if (!ui->Regexone_edit->text().isEmpty() and !checking_text.empty() and !ui->Regextwo_edit->text().isEmpty() and (ui->intersection_radio->isChecked() or ui->union_radio->isChecked()))
    {
        //detection for space char
        auto it_one = std::find(regex_one.begin(),regex_one.end(),' ');
        auto it_two = std::find(regex_two.begin(),regex_two.end(),' ');
        if (it_one != regex_one.end() or it_two != regex_two.end()){
            QMessageBox::warning(this,"SPACE CHAR DETECTION","There cannot be any space characters in the regexboxes!");
            // box 1 contains the space
            if (it_one != regex_one.end()){
                ui->Regexone_edit->clear();
            }
            // box 2 contains the space
            if (it_two != regex_two.end()){
                ui->Regextwo_edit->clear();
            }
        }
            // We can do the pattern search
        else
        {
            Pattern pattern(checking_text);
            bool constructie = checked_intersection;
            ui->textBrowser->clear();
            pattern.searchPattern(regex_one,regex_two,constructie,ui);
        }
    }
    else
    {
        QMessageBox::warning(this,"ERROR","Couldn't do the searching !");
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
        ui->union_radio->setChecked(false);
        ui->intersection_radio->setChecked(false);
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








