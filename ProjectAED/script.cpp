#include "script.h"
#include <cctype>
#include "color_print.h"
using namespace std;

void Script::invalid(){ Color_Print(color_mode, "red", "Invalid Input, please try again", true); }

bool is_number(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

Script::Script(bool save_file) : data(LEIC("../classes.csv", save_file ? "../students_classes_save.csv" : "../students_classes.csv" , save_file)){}

void Script::quit(){
    data.save_to_files();
    exit(0);
}

void Script::run() {
    cout << "\n\n";
    Color_Print(color_mode, "blue", "-----------------------------------", true);
    Color_Print(color_mode, "blue", "\tSchedule Management System", true);
    Color_Print(color_mode, "blue", "-----------------------------------", true);
    Color_Print(color_mode, "green", "Select option:", true);
    Color_Print(color_mode, "green", "Get Information");
    Color_Print(color_mode, "cyan", " - press 1", true);
    Color_Print(color_mode, "green", "Make requests");
    Color_Print(color_mode, "cyan", " - press 2", true);
    Color_Print(color_mode, "green", "Settings");
    Color_Print(color_mode, "cyan", " - press 3", true);
    Color_Print(color_mode, "red", "Quit Manager");
    Color_Print(color_mode, "cyan", " - press 4", true);

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
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "green", "Change Class CAP (current is " + to_string(data.get_cap()) + ")\t");
            Color_Print(color_mode, "cyan", "2- ");
            if (color_mode) Color_Print(color_mode, "yellow", "Disable");
            else Color_Print(color_mode, "yellow", "Enable");
            Color_Print(color_mode, "green", " Color Mode\t");
            Color_Print(color_mode, "cyan", "3- ");
            Color_Print(color_mode, "red", "Cancel\n");

            cin >> option;
            while(option != "1" && option != "2" && option != "3") {
                invalid();
                cin >> option;
            }
            if(option == "1"){
                Color_Print(color_mode, "cyan", "New CAP: ");
                string cap;
                cin >> cap;
                while (!is_number(cap) || stoi(cap) > 50) {
                   invalid();
                   cin >> cap;
                }
                data.set_cap(stoi(cap));
            }
            else if(option == "2"){
                color_mode = !color_mode;
                if (color_mode) Color_Print(color_mode, "cyan", "Color mode enabled", true);
                else Color_Print(color_mode, "cyan", "Color mode disabled", true);
            }
            else run();
            break;
        }
        case '4': {
            quit();
            break;
        }
    }
    Color_Print(color_mode, "green", "Make another operation? ");
    Color_Print(color_mode, "cyan", "[Y/N]", true);
    cin >> option;
    while(option != "Y" && option != "N") {
        invalid();
        cin >> option;
    }
    option == "Y" ? run() : quit();
}

void Script::handle_requests() {
    Color_Print(color_mode, "green", "Select option:", true);
    Color_Print(color_mode, "green", "New registration");
    Color_Print(color_mode, "cyan", " - press 1", true);
    Color_Print(color_mode, "green", "Update registration");
    Color_Print(color_mode, "cyan", " - press 2", true);
    Color_Print(color_mode, "green", "Upload file with changes");
    Color_Print(color_mode, "cyan", " - press 3", true);
    Color_Print(color_mode, "green", "Undo Previous Operation");
    Color_Print(color_mode, "cyan", " - press 4", true);
    Color_Print(color_mode, "red", "Cancel");
    Color_Print(color_mode, "cyan", " - press 5", true);

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
            data.undo_request(color_mode);
            break;
        }
        case '5': {
            run();
        }
    }
}

void Script::request(){
    string type, option, uc_or_class, student_up, current_class, new_class, current_uc, new_uc;
    Color_Print(color_mode, "green", "Pick: ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "ADD ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "REMOVE ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "SWITCH ");
    Color_Print(color_mode, "cyan", "4- ");
    Color_Print(color_mode, "red", "Cancel", true);

    cin >> option; while(option != "1" && option != "2" && option != "3" && option != "4") {invalid(); cin >> option; cout << '\n';}
    if(option == "4") {
        handle_requests();
        return;
    }
    Color_Print(color_mode, "cyan", "Enter StudentCode ");
    Color_Print(color_mode, "yellow", "(e.g. 202020047)");
    Color_Print(color_mode, "cyan", ":", true);
    cin >> student_up;
    while(data.get_student_from_studentCode(student_up) == nullptr){
        invalid();
        cin >> student_up;
    }
    get_request(student_up, option);
    Color_Print(color_mode, "green", "Would you like to make another request? ");
    Color_Print(color_mode, "cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") request();
    data.process_requests(color_mode);
}

void Script::get_request(string studentCode, string option) {
    string type, uc_or_class, current_class, new_class, current_uc, new_uc;
    switch (option[0]) {
        case '1': {
            type = "ADD";
            Color_Print(color_mode, "cyan", "Enter UCcode ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ":", true);
            cin >> new_uc;
            while(!data.exists_Uc(new_uc) || data.get_student_from_studentCode(studentCode)->has_uc(new_uc)) {
                invalid();
                cin >> new_uc;
            }
            Color_Print(color_mode, "green", "Do you want to join a specific class? ");
            Color_Print(color_mode, "cyan", "[Y/N]", true);
            string answer;
            cin >> answer;
            while(answer != "Y" && answer != "N") {
                invalid();
                cin >> answer;
            }
            if (answer=="Y") {
                uc_or_class = "CLASS";
                Color_Print(color_mode, "cyan", "Enter Class ");
                Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
                Color_Print(color_mode, "cyan", ":", true);
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
            Color_Print(color_mode, "cyan", "Enter UCcode ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ":", true);
            cin >> current_uc;
            while(!data.exists_Uc(current_uc) || !data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
                invalid();
                cin >> current_uc;
            }
            break;
        }
        case '3': {
            type = "SWITCH";
            Color_Print(color_mode, "green", "Pick: ");
            Color_Print(color_mode, "cyan", "UC / CLASS", true);
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") {invalid(); cin >> uc_or_class;}
            if (uc_or_class == "CLASS") {
                Color_Print(color_mode, "cyan", "Enter UCcode ");
                Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
                Color_Print(color_mode, "cyan", ":", true);
                cin >> current_uc;
                while(!data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                new_uc = current_uc;
                Color_Print(color_mode, "cyan", "Enter new ClassCode ");
                Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
                Color_Print(color_mode, "cyan", ":", true);
                cin >> new_class;
                while(!data.exists_class(current_uc,new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            else {
                Color_Print(color_mode, "cyan", "Enter UCcode ");
                Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
                Color_Print(color_mode, "cyan", ":", true);
                cin >> current_uc;
                while(!data.get_student_from_studentCode(studentCode)->has_uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                Color_Print(color_mode, "cyan", "Enter new UCcode ");
                Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
                Color_Print(color_mode, "cyan", ":", true);
                cin >> new_uc;
                while(!data.exists_Uc(new_uc)) {
                    invalid();
                    cin >> new_uc;
                }
                Color_Print(color_mode, "green", "Do you want to join a specific class? ");
                Color_Print(color_mode, "cyan", "[Y/N]", true);
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    invalid();
                    cin >> answer;
                }
                if (answer=="Y") {
                    uc_or_class = "CLASS";
                    Color_Print(color_mode, "cyan", "Enter ClassCode ");
                    Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
                    Color_Print(color_mode, "cyan", ":", true);
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
    Color_Print(color_mode, "cyan", "New StudentCode ");
    Color_Print(color_mode, "yellow", "(e.g. 202020047)");
    Color_Print(color_mode, "cyan", ":", true);
    cin >> new_up;
    while (!is_number(new_up) || new_up.length() != 9 || data.get_student_from_studentCode(new_up) != nullptr) {
        invalid();
        cin >> new_up;
    }
    Color_Print(color_mode, "cyan", "Name of new student: ");
    cin.ignore();
    getline(cin, name);
    data.add_student(Student(name, new_up));
    data.add_processed_request(Request("NEW","", new_up));
    Color_Print(color_mode, "green", "Would you now like to add UC's to this student? ");
    Color_Print(color_mode, "cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer=="Y") {
        Color_Print(color_mode, "cyan", "How many UC's do you want to join?", true);
        cin >> number_ucs;
        while (!is_number(number_ucs) || stoi(number_ucs) > 7 ) {
            invalid();
            cin >> number_ucs;
        }
        for (int i = 0; i < stoi(number_ucs); i++) {
            get_request(new_up, "1");
        }
    }
    data.process_requests(color_mode);
}

void Script::request_file() {
    Color_Print(color_mode, "green", "Save the file to this directory named ");
    Color_Print(color_mode, "cyan", "requests.csv ");
    Color_Print(color_mode, "green", "with the following structure:", true);
    Color_Print(color_mode, "green", "StudentCode, Type, oldUcCode, newUcCode, oldClassCode, newClassCode", true);
    Color_Print(color_mode, "green", "Do you want to process the requests? ");
    Color_Print(color_mode, "cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") data.upload_requests(color_mode);
}

void Script::listings(){
    Color_Print(color_mode, "green", "What would you like to check?", true);
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Schedules ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Student Lists ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "Occupations ");
    Color_Print(color_mode, "cyan", "4- ");
    Color_Print(color_mode, "green", "Students with at least n UC's ");
    Color_Print(color_mode, "cyan", "5- ");
    Color_Print(color_mode, "red", "Cancel", true);
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
            break;
        }
        case '4': {
            Color_Print(color_mode, "cyan", "N:");
            string n;
            cin >> n;
            while (!is_number(n) || stoi(n) > 7) {
                invalid();
                cin >> n;
            }
            string total =to_string(data.students_in_n_Ucs(stoi(n)));
            cout << "Total students with " << n << " UC's: ";
            Color_Print(color_mode, "blue", total, true);
            break;
        }
        case '5': run();
    }
}

void Script::occupations(){
    Color_Print(color_mode, "green", "What would you like to check?", true);
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Year ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "UC ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "Class ");
    Color_Print(color_mode, "cyan", "4- ");
    Color_Print(color_mode, "red", "Cancel", true);
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4" && option != "5") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            Color_Print(color_mode, "green", "Pick: ");
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "green", "1st Year ");
            Color_Print(color_mode, "cyan", "2- ");
            Color_Print(color_mode, "green", "2nd Year ");
            Color_Print(color_mode, "cyan", "3- ");
            Color_Print(color_mode, "green", "3rd Year ");
            Color_Print(color_mode, "cyan", "4- ");
            Color_Print(color_mode, "red", "Cancel", true);
            string year;
            cin >> year;
            while(year != "1" && year != "2" && year != "3" && year != "4") {
                invalid();
                cin >> option;
            }
            list_year_occupations(year);
            break;
        }
        case '2': {
            Color_Print(color_mode, "cyan", "Enter UCcode ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ": ");
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            list_Uc_occupations(UC);
            break;
        }
        case '3': {
            Color_Print(color_mode, "cyan", "Enter Class to check ");
            Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
            Color_Print(color_mode, "cyan", ": ");
            string classCode; cin >> classCode;
            while(!data.get_classCodes().count(classCode)) {
                invalid();
                cin >> classCode;
            }
            list_class_occupations(classCode);
            break;
        }
        case '4': {
            listings();
            break;
        }
    }
}

void Script::listSchedules(){
    Color_Print(color_mode, "green", "Consult schedule of: ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Student ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Class ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "UC ");
    Color_Print(color_mode, "cyan", "4- ");
    Color_Print(color_mode, "red", "Cancel", true);
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            Color_Print(color_mode, "cyan", "Insert StudentCode ");
            Color_Print(color_mode, "yellow", "(e.g. 202020047)");
            Color_Print(color_mode, "cyan", ":");
            string up;
            cin >> up;
            while(data.get_student_from_studentCode(up) == nullptr){
                invalid();
                cin >> up;
            }
            data.get_student_from_studentCode(up)->print_schedule(color_mode);
            break;
        }
        case '2': {
            Color_Print(color_mode, "cyan", "Enter ClassCode ");
            Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
            Color_Print(color_mode, "cyan", ":");
            string class_;
            cin >> class_;
            while(!data.get_classCodes().count(class_)) {    //mudar para exists classcode???????
                invalid();
                cin >> class_;
            }

            vector <Class> classes = data.get_classes();
            set<Lesson> classLessons;
            for(Class c : classes){
                if (c.get_classCode() == class_) {
                    set<Lesson> lessons = c.get_lessons();
                    classLessons.insert(lessons.begin(),lessons.end());
                }
            }
            string currentDay;
            for(Lesson lesson: classLessons) {
                if (currentDay.empty()) {
                    currentDay = lesson.get_weekday();
                    Color_Print(color_mode, "blue", lesson.get_weekday(), true);
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    Color_Print(color_mode, "blue", lesson.get_weekday(), true);
                }
                lesson.print_lesson(color_mode);
            }
            break;
        }
        case '3': {
            Color_Print(color_mode, "cyan", "Enter UCcode ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ": ");
            string uc;
            cin >> uc;
            while(!data.get_UcCodes().count(uc)) {
                invalid();
                cin >> uc;
            }
            vector<Class*> classesUc = data.get_classes_from_UcCode(uc);
            set<Lesson> total_lessons;
            for(Class* c : classesUc) {
                set<Lesson> lessons = c->get_lessons();
                total_lessons.insert(lessons.begin(), lessons.end());
            }
            string currentDay;
            for(Lesson lesson: total_lessons) {
                if (currentDay.empty()) {
                    currentDay = lesson.get_weekday();
                    Color_Print(color_mode, "blue", lesson.get_weekday(), true);
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    Color_Print(color_mode, "blue", lesson.get_weekday(), true);
                }
                lesson.print_lesson(color_mode);
            }
            break;
        }
        case '4':{
            listings();
        }
    }
}

void Script::listStudents() {
    Color_Print(color_mode, "green", "Consult list of students of: ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Course ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "UC ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "Class ");
    Color_Print(color_mode, "cyan", "4- ");
    Color_Print(color_mode, "red", "Cancel", true);
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            Color_Print(color_mode, "green", "Sort student by? ");
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "green", "StudentCode ");
            Color_Print(color_mode, "cyan", "2- ");
            Color_Print(color_mode, "green", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2" && option != "3") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_students_by_studentCode(color_mode);
            else data.list_students_by_name(color_mode);
            break;
        }
        case '2': {
            Color_Print(color_mode, "cyan", "Enter UCcode ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ": ");
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            Color_Print(color_mode, "green", "Sort student by? ");
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "green", "StudentCode ");
            Color_Print(color_mode, "cyan", "2- ");
            Color_Print(color_mode, "green", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_UC_students_by_studentCode(UC, color_mode);
            else data.list_UC_students_by_name(UC, color_mode);
            break;
        }
        case '3': {
            Color_Print(color_mode, "cyan", "Enter UC of Class to check ");
            Color_Print(color_mode, "yellow", "(e.g. L.EIC001)");
            Color_Print(color_mode, "cyan", ": ");
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            Color_Print(color_mode, "cyan", "Enter Class to check ");
            Color_Print(color_mode, "yellow", "(e.g. 1LEIC01)");
            Color_Print(color_mode, "cyan", ": ");
            string class_; cin >> class_;
            while(!data.exists_class(UC,class_)) {
                invalid();
                cin >> class_;
            }
            Color_Print(color_mode, "green", "Sort student by? ");
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "green", "StudentCode ");
            Color_Print(color_mode, "cyan", "2- ");
            Color_Print(color_mode, "green", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            if(option == "1") data.list_class_students_by_studentCode(data.get_class_from_classCode_and_UcCode(class_, UC), color_mode);
            else data.list_class_students_by_name(data.get_class_from_classCode_and_UcCode(class_, UC), color_mode);
            break;
        }
        case '4': listings();
    }
}

void Script::list_year_occupations(string year) {
    vector<Class> classes = data.get_classes();
    vector<Class> yearClasses;
    for(Class c: classes){
        if (c.get_classCode()[0] == year[0]) yearClasses.push_back(c);
    }

    string option, order;
    Color_Print(color_mode, "green", "Sort by? ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "UC ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "ClassCode ");
    Color_Print(color_mode, "cyan", "3- ");
    Color_Print(color_mode, "green", "Occupation", true);
    cin >> option;
    while(option != "1" && option != "2" && option != "3"){
        invalid();
        cin >> option;
    }
    Color_Print(color_mode, "green", "Pick: ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Ascending Order ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Descending Order", true);
    cin >> order;
    while(order != "1" && order != "2"){
        invalid();
        cin >> order;
    }
    if(option == "3"){
        (order == "1") ? sort(yearClasses.begin(),yearClasses.end(), [] (Class c1,Class c2) -> bool {return (c1.get_students().size() < c2.get_students().size())
                                    || (c1.get_students().size() == c2.get_students().size() && (c1.get_ucCode() < c2.get_ucCode()) || (c1.get_ucCode() == c2.get_ucCode() && c1.get_classCode() < c2.get_classCode()))  ;})
        : sort(yearClasses.rbegin(),yearClasses.rend(), [] (Class c1,Class c2) -> bool { return (c1.get_students().size() < c2.get_students().size())
                                    || (c1.get_students().size() == c2.get_students().size() && (c1.get_ucCode() > c2.get_ucCode()) || (c1.get_ucCode() == c2.get_ucCode() && c1.get_classCode() > c2.get_classCode()));});
    }
    else if (option == "2"){
        (order == "1") ? sort(yearClasses.begin(),yearClasses.end(), [] (Class c1,Class c2) -> bool {return c1.get_classCode() < c2.get_classCode()
                                                                                                                    || (c1.get_classCode() == c2.get_classCode() && c1.get_ucCode() < c2.get_ucCode()) ;})
        : sort(yearClasses.rbegin(),yearClasses.rend(), [] (Class c1,Class c2) -> bool {return c1.get_classCode() < c2.get_classCode()
                                                                                || (c1.get_classCode() == c2.get_classCode() && c1.get_ucCode() > c2.get_ucCode());});
    }
    if(option == "1" && order == "2") {
        auto itr = yearClasses.end();
        while(itr-- != yearClasses.begin()){
            Color_Print(color_mode, "blue", itr->get_classCode() + " " + itr->get_ucCode() + " " + to_string(itr->get_students().size()), true);
        }
        return;
    }
    for (Class c: yearClasses) {
        Color_Print(color_mode, "blue", c.get_classCode() + " " + c.get_ucCode() + " " + to_string(c.get_students().size()), true);
    }
}

void Script::list_Uc_occupations(std::string UcCode) {
    string option, order;
    Color_Print(color_mode, "green", "Sort by? ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "ClassCode ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Occupation",true);
    cin >> option;
    while(option != "1" && option != "2"){
        invalid();
        cin >> option;
    }
    Color_Print(color_mode, "green", "Pick: ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Ascending Order ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Descending Order", true);
    cin >> order;
    while(order != "1" && order != "2"){
        invalid();
        cin >> order;
    }

    vector<Class> classes = data.get_classes();
    vector<Class> UcClasses;
    for(Class c: classes){
        if (c.get_ucCode() == UcCode) UcClasses.push_back(c);
    }
    if(option == "2"){
        (order == "1") ? sort(UcClasses.begin(),UcClasses.end(), [] (Class c1,Class c2) -> bool {return (c1.get_students().size() < c2.get_students().size())
                                                                                                            || (c1.get_students().size() == c2.get_students().size() && c1.get_classCode() < c2.get_classCode())  ;})
                       : sort(UcClasses.rbegin(),UcClasses.rend(), [] (Class c1,Class c2) -> bool { return (c1.get_students().size() < c2.get_students().size())
                                                                                                               || (c1.get_students().size() == c2.get_students().size() && c1.get_classCode() > c2.get_classCode());});
    }
    else if (option == "1" && order == "2"){
        auto itr = UcClasses.end();
        while(itr-- != UcClasses.begin()){
            Color_Print(color_mode, "blue", itr->get_classCode() + " " + itr->get_ucCode() + " " + to_string(itr->get_students().size()), true);
        }
        return;
    }
    for (Class c: UcClasses) {
        Color_Print(color_mode, "blue", c.get_classCode() + " " + to_string(c.get_students().size()), true);
    }
}


void Script::list_class_occupations(std::string classCode) {
    string option, order;
    Color_Print(color_mode, "green", "Sort by? ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "UcCode ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Occupation",true);
    cin >> option;
    while(option != "1" && option != "2"){
        invalid();
        cin >> option;
    }
    Color_Print(color_mode, "green", "Sort by? ");
    Color_Print(color_mode, "cyan", "1- ");
    Color_Print(color_mode, "green", "Ascending Order ");
    Color_Print(color_mode, "cyan", "2- ");
    Color_Print(color_mode, "green", "Descending Order", true);
    cin >> order;
    while(order != "1" && order != "2"){
        invalid();
        cin >> order;
    }

    vector<Class> classes = data.get_classes();
    vector<Class> classClasses;
    for(Class c: classes){
        if (c.get_classCode() == classCode) classClasses.push_back(c);
    }
    if(option == "2"){
        (order == "1") ? sort(classClasses.begin(),classClasses.end(), [] (Class c1,Class c2) -> bool {return (c1.get_students().size() < c2.get_students().size())
                                                                                                        || (c1.get_students().size() == c2.get_students().size() && c1.get_ucCode() < c2.get_ucCode())  ;})
                       : sort(classClasses.rbegin(),classClasses.rend(), [] (Class c1,Class c2) -> bool { return (c1.get_students().size() < c2.get_students().size())
                                                                                                           || (c1.get_students().size() == c2.get_students().size() && c1.get_ucCode() > c2.get_ucCode());});
    }
    else if (option == "1" && order == "2"){
        auto itr = classClasses.end();
        while(itr-- != classClasses.begin()){
            Color_Print(color_mode, "blue", itr->get_ucCode() + " " + to_string(itr->get_students().size()), true);
        }
        return;
    }
    for (Class c: classClasses) {
        Color_Print(color_mode, "blue", c.get_ucCode() + " " + to_string(c.get_students().size()), true);
    }
}



