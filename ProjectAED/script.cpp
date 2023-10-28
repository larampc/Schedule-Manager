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
    string type, uc_or_class;
    cout << "Pick: ADD / REMOVE / SWITCH\n";
    cin >> type; while(type != "ADD" && type != "REMOVE" && type != "SWITCH") {invalid(); cin >> type; cout << '\n';}
    string student_up, current_class, new_class, current_uc, new_uc;
    cout << "UP:\n";
    cin >> student_up;
    switch (type[0]) {
        case 'A': {
            cout << "Pick: UC / CLASS\n";
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
            cout << "UC:\n";
            cin >> new_uc;
            if (uc_or_class == "CLASS") {
                cout << "CLASS:\n";
                cin >> new_class;
            }
            break;
        }
        case 'R': {
            cout << "UC:\n";
            cin >> current_uc;
            break;
        }
        case 'S': {
            cout << "Pick: UC / CLASS\n";
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
            if (uc_or_class == "CLASS") {
                cout << "UC:\n";
                cin >> current_uc;
                new_uc = current_uc;
                cout << "NEW CLASS:\n";
                cin >> new_class;
            }
            else {
                cout << "CURRENT UC:\n";
                cin >> current_uc;
                cout << "NEW UC:\n";
                cin >> new_uc;
                cout << "Do you want to join a specific class: [Y/N]?\n";
                string answer;
                cin >> answer;
                if (answer=="Y") {
                    cout << "CLASS:\n";
                    cin >> new_class;
                }
            }
            break;
        }
    }
    Request request = Request(type, (uc_or_class == "CLASS"), student_up, current_class, new_class, current_uc, new_uc);
    if (!data.process_requests(request)) {
        cout << "The request was rejected";
    }
    run();
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
            while(cin.fail() || data.get_student_from_up(to_string(up))->get_name() == ""){
                invalid();
                cin.clear();
                cin.ignore( numeric_limits<streamsize>::max(),'\n');
                cin >> up;
            }
            data.get_student_from_up(to_string(up))->print_schedule();
            break;
        }
        case '2': {
            set<string> classcodes;
            for(Class c: data.get_classes()) classcodes.insert(c.get_classCode());
            cout << "Enter Class: ";
            string class_;
            cin >> class_;
            while(!classcodes.count(class_)) {
                invalid();
                cin >> class_;
            }
            vector<Class> classCodeClasses;
            for(Class c : data.get_classes()){
                if(c.get_classCode() == class_){
                    classCodeClasses.push_back(c);
                }
            }
            set<Lesson> classLessons;
            for(Class c : classCodeClasses){
                set<Lesson> lessons = c.get_lessons();
                classLessons.insert(lessons.begin(),lessons.end());
            }
            string currentDay;
            for(Lesson lesson: classLessons) {
                if (currentDay.empty()) {
                    currentDay = lesson.get_weekday();
                    cout << lesson.get_weekday() << endl;
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    cout << lesson.get_weekday() << endl;
                }
                cout << setw(2) << setfill('0') << lesson.get_starttime().get_hour() << ":"
                     << setw(2) << setfill('0') << lesson.get_starttime().get_minute()
                     << " - " << setw(2) << setfill('0') << lesson.get_endtime().get_hour() << ":"
                     << setw(2) << setfill('0') << lesson.get_endtime().get_minute()
                     << "\t" << setw(2) << setfill(' ') << lesson.get_type() << " " << lesson.get_thisclass() << endl;
            }

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
            break;
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
            break;
        }
        case '3': {
            cout << "Enter UC of Class to check: ";
            string UC; cin >> UC;
            while(!data.get_ucs().count(UC)) {
                invalid();
                cin >> UC;
            }
            cout << "Enter Class to check: ";
            string class_; cin >> class_;
            while(data.get_class_from_classcode_and_uccode(class_,UC) == nullptr) {
                invalid();
                cin >> class_;
            }
            cout << "Sort student by? 1- upNumber 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_class_students_by_UP(data.get_class_from_classcode_and_uccode(class_,UC));
            else data.list_class_students_by_Name(data.get_class_from_classcode_and_uccode(class_,UC));
        }
    }
}