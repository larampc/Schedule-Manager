#include "script.h"
#include <limits>
using namespace std;

void invalid(){ cout << "Invalid Input, please try again\n"; }

Script::Script(): data(LEIC("../classes.csv", "../students_classes.csv")) {}

void Script::run() {
    cout << "------------------------------------\n\tSchedule Management System\n------------------------------------\nSelect option:\n"
         << "Get Information - press 1\n"
         << "Make request - press 2\n";
    string option; cin >> option;
    while(option != "1" && option != "2") {
        invalid();
        cin >> option;
    }
    switch (option.at(0)) {
        case '1': {
            listings();
            break;
        }
        case '2': {
            request();
            break;
        }
    }
}

void Script::request(){
    std::string type, uc_or_class;
    cout << "Pick: ADD / REMOVE / SWITCH\n";
    cin >> type; while(type != "ADD" && type != "REMOVE" && type != "SWITCH") {invalid(); cin >> type; cout << '\n';}
    cout << "Pick: UC / CLASS\n";
    cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
 //   data.processRequest();
}

void Script::listings(){
    cout << "What would you like to check?\n"
    << "1- Schedules 2- Student Lists 3- ...\n";
    string option; cin >> option;
    while(option != "1" && option != "2") {
        invalid();
        cin >> option;
    }
    switch (option[0]){
        case '1': {
            listSchedules();
            break;
        }
    }
}

void Script::listSchedules(){
    cout << "Consult schedule of: 1- Student 2- Class 3- UC\n";
    string option;
    cin >> option;
    while(option != "1" && option != "2") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            cout << "Insert student up: ";
            long up;
            cin >> up;
            while(cin.fail() || up < 200000000 || up > 202300000){
                invalid();
                cin.clear();
                cin.ignore( numeric_limits<streamsize>::max(),'\n');
                cin >> up;
            }
            //getstudentschedule();
            break;
        }
        case '2': {

        }
    }

}

