#include "script.h"
#include <cctype>
using namespace std;

void invalid(){ cout << "Invalid Input, please try again\n"; }

bool is_number(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

Script::Script(bool save_file) : data(LEIC("../classes.csv", save_file ? "../students_classes_save.csv" : "../students_classes.csv" , save_file)){}

void Script::quit(){
    data.save_to_files();
    exit(0);
}

void Script::run() {
    cout << "\n\n-----------------------------------\n\tSchedule Management System\n------------------------------------\n";
    cout << "Select option:\n"
         << "Get Information - press 1\n"
         << "Make requests - press 2\n"
         << "Settings - press 3\n"
         << "Quit Manager - press 4\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            listings();
            break;
        }
        case '2': {
            handle_requests();
            break;
        }
        case '3': {
            cout << "1- Change Class CAP (current is " << data.get_cap() << ")\t2- Cancel\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1"){
               cout << "New CAP:";
               string cap;
               cin >> cap;
               while (!is_number(cap) || stoi(cap) > 50) {
                   invalid();
                   cin >> cap;
               }
               data.set_cap(stoi(cap));
            }
            else run();
            break;
        }
        case '4': {
            quit();
            break;
        }
    }
    cout << "Make another operation? [Y/N]\n";
    cin >> option;
    while(option != "Y" && option != "N") {
        invalid();
        cin >> option;
    }
    option == "Y" ? run() : quit();
}

void Script::handle_requests() {
    cout << "Select option:\n"
         << "New registration - press 1\n"
         << "Update registration - press 2\n"
         << "Upload file with changes - press 3\n"
         << "Undo Previous Operation - press 4\n"
         << "Cancel - press 5\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4" && option != "5") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            new_registration();
            break;
        }
        case '2': {
            request();
            break;
        }
        case '3': {
            request_file();
            break;
        }
        case '4': {
            data.undo_request();
            break;
        }
        case '5': {
            run();
        }
    }
}

void Script::request(){
    string option, student_up;
    cout << "Pick: 1- ADD 2- REMOVE 3- SWITCH 4- Cancel\n";
    cin >> option; while(option != "1" && option != "2" && option != "3" && option != "4") {invalid(); cin >> option; cout << '\n';}
    if(option == "4") {
        handle_requests();
        return;
    }
    cout << "Enter StudentCode (e.g. 202020047):\n";
    cin >> student_up;
    while(data.get_student_from_studentCode(student_up) == nullptr){
        invalid();
        cin >> student_up;
    }
    get_request(student_up, option);
    cout << "Would you like to make another request? [Y/N]\n";
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") request();
    data.process_requests();
}

void Script::get_request(string studentCode, string option) {
    string type, uc_or_class, current_class, new_class, current_uc, new_uc;
    switch (option[0]) {
        case '1': {
            type = "ADD";
            cout << "Enter UCcode (e.g. L.EIC001):\n";
            cin >> new_uc;
            while(!data.exists_Uc(new_uc) || data.get_student_from_studentCode(studentCode)->has_uc(new_uc)) {
                invalid();
                cin >> new_uc;
            }
            cout << "Do you want to join a specific class? [Y/N]\n";
            string answer;
            cin >> answer;
            while(answer != "Y" && answer != "N") {
                invalid();
                cin >> answer;
            }
            if (answer=="Y") {
                uc_or_class = "CLASS";
                cout << "Enter Class (e.g. 1LEIC01):\n";
                cin >> new_class;
                while(!data.exists_class(new_uc,new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            break;
        }
        case '2': {
            type = "REMOVE";
            cout << "Enter UCcode (e.g. L.EIC001):\n";
            cin >> current_uc;
            while(!data.exists_Uc(current_uc) || !data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
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
                cout << "Enter UCcode (e.g. L.EIC001):\n";
                cin >> current_uc;
                while(!data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                new_uc = current_uc;
                cout << "Enter new ClassCode (e.g. 1LEIC01):\n";
                cin >> new_class;
                while(!data.exists_class(current_uc,new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            else {
                cout << "Enter current UCcode (e.g. L.EIC001):\n";
                cin >> current_uc;
                while(!data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                cout << "Enter new UCcode (e.g. L.EIC001):\n";
                cin >> new_uc;
                while(!data.exists_Uc(new_uc)) {
                    invalid();
                    cin >> new_uc;
                }
                cout << "Do you want to join a specific class? [Y/N]\n";
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    invalid();
                    cin >> answer;
                }
                if (answer=="Y") {
                    uc_or_class = "CLASS";
                    cout << "Enter ClassCode (e.g. 1LEIC01):\n";
                    cin >> new_class;
                    while(!data.exists_class(new_uc,new_class)) {
                        invalid();
                        cin >> new_class;
                    }
                }
            }
            break;
        }
    }
    data.add_request_to_process(Request(type, (uc_or_class == "CLASS"), studentCode, current_class, new_class, current_uc, new_uc));
}

void Script::new_registration() {
    string new_up, name, number_ucs, new_uc, new_class;
    cout << "New StudentCode (e.g. 202020047):";
    cin >> new_up;
    while (!is_number(new_up) || new_up.length() != 9 || data.get_student_from_studentCode(new_up) != nullptr) {
        invalid();
        cin >> new_up;
    }
    cout << "Name of new student:";
    cin.ignore();
    getline(cin, name);
    data.add_student(Student(name, new_up));
    data.add_processed_request(Request("NEW","", new_up));
    cout << "Would you now like to add UC's to this student? [Y/N]\n";
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer=="Y") {
        cout << "How many UC's do you want to join?\n";
        cin >> number_ucs;
        while (!is_number(number_ucs) || stoi(number_ucs) > 7 ) {
            invalid();
            cin >> number_ucs;
        }
        for (int i = 0; i < stoi(number_ucs); i++) {
            get_request(new_up, "1");
        }
    }
    data.process_requests();
}

void Script::request_file() {
    cout << "Save the file to this directory named requests.csv with the following structure:\n"
         << "StudentCode, Type, oldUcCode, newUcCode, oldClassCode, newClassCode\n"
         << "Do you want to process the requests? [Y/N]\n";
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") data.upload_requests();
}

void Script::listings(){
    cout << "What would you like to check?\n"
    << "1- Schedules 2- Student Lists 3- Occupations 4- Students with at least n UC's 5- Cancel\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4" && option != "5") {
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
            occupations();
            data.list_number_students_class();
            break;
        }
        case '4': {
            cout << "N:";
            string n;
            cin >> n;
            while (!is_number(n) || stoi(n) > 7) {
                invalid();
                cin >> n;
            }
            cout << data.students_in_n_Ucs(stoi(n)) << endl;
            break;
        }
        case '5': run();
    }
}

void Script::occupations(){
    cout << "What would you like to check?\n"
         << "1- Year 2- UC 3- Class 4- Cancel\n";
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4" && option != "5") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            list_year_occupations();
            break;
        }
        case '2': {
            break;
        }
        case '3': {
            break;
        }
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
            cout << "Insert StudentCode (e.g. 202020047): ";
            string up;
            cin >> up;
            while(data.get_student_from_studentCode(up) == nullptr){
                invalid();
                cin >> up;
            }
            data.get_student_from_studentCode(up)->print_schedule();
            break;
        }
        case '2': {
            cout << "Enter ClassCode (e.g. 1LEIC01): ";
            string class_;
            cin >> class_;
            while(!data.get_classCodes().count(class_)) {    //mudar para exists classcode???????
                invalid();
                cin >> class_;
            }

            vector<Class> classCodeClasses;
            vector <Class> classes = data.get_classes();
            auto first_itr = lower_bound(classes.begin(),classes.end(),Class("",class_));
            while(first_itr->get_classCode() == class_) classCodeClasses.push_back(*(first_itr++));
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
            cout << "Enter UCcode (e.g. L.EIC001): ";
            string uc;
            cin >> uc;
            while(!data.get_UcCodes().count(uc)) {
                invalid();
                cin >> uc;
            }
            vector<Class*> classesUc = data.get_classes_from_UcCode(uc);
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
            cout << "Sort student by? 1- StudentCode 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_students_by_studentCode();
            else data.list_students_by_name();
            break;
        }
        case '2': {
            cout << "Enter UCcode (e.g. L.EIC001): ";
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            cout << "Sort student by? 1- StudentCode 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_UC_students_by_studentCode(UC);
            else data.list_UC_students_by_name(UC);
            break;
        }
        case '3': {
            cout << "Enter UC of Class to check (e.g. L.EIC001):";
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            cout << "Enter Class to check (e.g. 1LEIC01):";
            string class_; cin >> class_;
            while(!data.exists_class(class_,UC)) {
                invalid();
                cin >> class_;
            }
            cout << "Sort student by? 1- StudentCode 2- Student Name\n";
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_class_students_by_studentCode(data.get_class_from_classCode_and_UcCode(class_, UC));
            else data.list_class_students_by_name(data.get_class_from_classCode_and_UcCode(class_, UC));
            break;
        }
        case '4': listings();
    }
}

void Script::list_year_occupations() {

}



