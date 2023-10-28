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
    string type, uc_or_class, uccode, classcode, up;
    cout << "Pick: ADD / REMOVE / SWITCH\n";
    cin >> type; while(type != "ADD" && type != "REMOVE" && type != "SWITCH") {invalid(); cin >> type; cout << '\n';}
    cout << "Pick: UC / CLASS\n";
    cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}

    // Temporary just for testing class balance and schedule compatibility
    cout << "UC:\n";
    cin >> uccode;
    cout << "CLASS:\n";
    cin >> classcode;
    cout << "UP:\n";
    cin >> up;
    Class* c = data.get_class_from_classcode_and_uccode(classcode,uccode);
    Student s = data.get_student_from_up(up);
    cout << data.class_balance_valid(*c) << endl;
    string classcode_to, uccode_to;
//    cout << "UC TO:\n";
//    cin >> uccode_to;
//    cout << "CLASS TO:\n";
//    cin >> classcode_to;
    //Class* c_to = data.get_class_from_classcode_and_uccode(classcode_to,uccode_to);
    cout << data.compatible_schedules(s, c) << endl;
 //   data.processRequest(type, uc_or_class == "UC");
}

void Script::listings(){
    cout << "What would you like to check?\n"
    << "1- Schedules 2- Student Lists 3- Occupations\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3") {
        invalid();
        cin >> option;
    }
    switch (option[0]){
        case '1': {
            listSchedules();
            break;
        }
        case '2': {
            listStudents();
            break;
        }
        case '3': {
            data.numberstudents_class();
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
            while(cin.fail() || data.get_student_from_up(to_string(up)).get_name() == ""){
                invalid();
                cin.clear();
                cin.ignore( numeric_limits<streamsize>::max(),'\n');
                cin >> up;
            }
            data.get_student_from_up(to_string(up)).print_schedule();
            break;
        }
        case '2': {

        }
    }
}

void Script::listStudents() {
    cout << "Consult list of students of: 1- Course 2- UC 3- Class\n";
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            cout << "Sort student by? 1- upNumber 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.listStudentsByUP();
            else data.listStudentsByName();
        }
        case '2': {
            cout << "Enter UC to check: ";
            string UC; cin >> UC;
            while(!data.get_ucs().count(UC)) {
                invalid();
                cin >> UC;
            }
            cout << "Sort student by? 1- upNumber 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.listUCStudentsByUP(UC);
            else data.listUCStudentsByName(UC);
        }
    }
}