#include "script.h"
#include <limits>
using namespace std;

void invalid(){ cout << "Invalid Input, please try again\n"; }

Script::Script(): data(LEIC("../classes.csv", "../students_classes.csv")) {}

void Script::run() {
    system("Color 0A");
    cout << "\n\n-----------------------------------\n\tSchedule Management System\n------------------------------------\n";
    system("Color 0B");
    cout << "Select option:\n"
         << "Get Information - press 1\n"
         << "Make request - press 2\n"
         << "Settings - press 3\n"
         << "Quit Manager - press 4\n";
    system("Color 07");
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
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
        case '3': {
            cout << "1- Change Class CAP\t2- Cancel\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1"){
               cout << "New CAP: ";
               int cap;
               cin >> cap;
               while (cin.fail()) {
                   invalid();
                   cin.clear();
                   cin.ignore(numeric_limits<streamsize>::max(),'\n');
                   cin >> cap;
               }
               cout << cap;
               data.set_cap(cap);
            }
            else run();
            break;
        }
        case '4': {
            exit(0);
        }
    }
}

void Script::request(){
    string type, option, uc_or_class;
    cout << "Pick: 1- ADD 2- REMOVE 3- SWITCH 4- Cancel\n";
    cin >> option; while(option != "1" && option != "2" && option != "3" && option != "4") {invalid(); cin >> option; cout << '\n';}
    if(option == "4") {
        run();
        return;
    }
    string student_up, current_class, new_class, current_uc, new_uc;
    cout << "UP:\n";
    cin >> student_up;
    while(data.get_student_from_up(student_up) == nullptr){
        invalid();
        cin >> student_up;
    }
    switch (option[0]) {
        case '1': {
            type = "ADD";
            cout << "Pick: UC / CLASS\n";
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
            cout << "UC:\n";
            cin >> new_uc;
            if (uc_or_class == "CLASS") {
                cout << "CLASS:\n";
                cin >> new_class;
                while(!data.get_classcodes().count(new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            break;
        }
        case '2': {
            type = "REMOVE";
            cout << "UC:\n";
            cin >> current_uc;
            while(!data.get_ucs().count(current_uc) || !data.get_student_from_up(student_up)->has_uc(current_uc)) {
                invalid();
                cin >> current_uc;
            }
            break;
        }
        case '3': {
            type = "SWITCH";
            cout << "Pick: UC / CLASS\n";
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
            if (uc_or_class == "CLASS") {
                cout << "UC:\n";
                cin >> current_uc;
                while(!data.get_ucs().count(current_uc) && data.get_student_from_up(student_up)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                new_uc = current_uc;
                cout << "NEW CLASS:\n";
                cin >> new_class;
                while(!data.get_classcodes().count(new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            else {
                cout << "CURRENT UC:\n";
                cin >> current_uc;
                while(!data.get_ucs().count(current_uc) && data.get_student_from_up(student_up)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                cout << "NEW UC:\n";
                cin >> new_uc;
                while(!data.get_ucs().count(new_uc)) {
                    invalid();
                    cin >> new_uc;
                }
                cout << "Do you want to join a specific class: [Y/N]?\n";
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    invalid();
                    cin >> answer;
                }
                if (answer=="Y") {
                    cout << "CLASS:\n";
                    cin >> new_class;
                    while(!data.get_classcodes().count(new_class)) {
                        invalid();
                        cin >> new_class;
                    }
                }
            }
            break;
        }
    }
    Request request = Request(type, (uc_or_class == "CLASS"), student_up, current_class, new_class, current_uc, new_uc);
    if (!data.process_requests(request)) {
        system("Color 04");
        cout << "The request was rejected";
        system("Color 07");
    }
    run();
}

void Script::listings(){
    cout << "What would you like to check?\n"
    << "1- Schedules 2- Student Lists 3- Occupations 4- Cancel\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
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
        case '4': run();
    }
}

void Script::listSchedules(){
    cout << "Consult schedule of: 1- Student 2- Class 3- UC 4-Cancel\n";
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            cout << "Insert student up: ";
            string up;
            cin >> up;
            while(data.get_student_from_up(up) == nullptr){
                invalid();
                cin >> up;
            }
            data.get_student_from_up(up)->print_schedule();
            break;
        }
        case '2': {
            cout << "Enter Class: ";
            string class_;
            cin >> class_;
            while(!data.get_classcodes().count(class_)) {
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
                lesson.print_lesson();
            }
            break;
        }
        case '3': {
            cout << "Enter UC: ";
            string uc;
            cin >> uc;
            while(!data.get_ucs().count(uc)) {
                invalid();
                cin >> uc;
            }
            vector<Class*> classesUc = data.get_classes_from_uccode(uc);
            multiset<Lesson> total_lessons;
            for(Class* c : classesUc) {
                set<Lesson> lessons = c->get_lessons();
                total_lessons.insert(lessons.begin(), lessons.end());
            }
            string currentDay;
            for(Lesson lesson: total_lessons) {
                if (currentDay.empty()) {
                    currentDay = lesson.get_weekday();
                    cout << lesson.get_weekday() << endl;
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    cout << lesson.get_weekday() << endl;
                }
                lesson.print_lesson();
            }
            break;
        }
        case '4':{
            listings();
        }
    }
}

void Script::listStudents() {
    cout << "Consult list of students of: 1- Course 2- UC 3- Class 4- Cancel\n";
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
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
            break;
        }
        case '4': listings();
    }
}