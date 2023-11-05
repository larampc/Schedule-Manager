#include "menu.h"
#include <cctype>
#include <limits>
#include "color_print.h"
using namespace std;
extern bool color_mode;

void Menu::invalid(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Color_Print("red", "Invalid Input, please try again", true);
}

bool is_number(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

Menu::Menu(bool save_file) : data(LEIC("../classes.csv", save_file ? "../students_classes_save.csv" : "../students_classes.csv" , save_file)){}

void Menu::quit(){
    data.save_to_files();
    exit(0);
}

void Menu::run() {
    cout << "\n\n";
    Color_Print("blue", "-----------------------------------", true);
    Color_Print("blue", "\tSchedule Management System", true);
    Color_Print("blue", "-----------------------------------", true);
    Color_Print("blue", "Select option:", true);
    Color_Print("white", "Get Information");
    Color_Print("cyan", " - press 1", true);
    Color_Print("white", "Make requests");
    Color_Print("cyan", " - press 2", true);
    Color_Print("white", "Settings");
    Color_Print("cyan", " - press 3", true);
    Color_Print("red", "Quit Manager");
    Color_Print("cyan", " - press 4", true);

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
            settings();
            break;
        }
        case '4': {
            quit();
            break;
        }
    }
    Color_Print("blue", "\nMake another operation? ");
    Color_Print("cyan", "[Y/N]", true);
    cin >> option;
    while(option != "Y" && option != "N") {
        invalid();
        cin >> option;
    }
    option == "Y" ? run() : quit();
}

void Menu::settings(){
    string option;
    Color_Print("cyan", "1- ");
    Color_Print("white", "Change Class CAP (current is " + to_string(data.get_cap()) + ")\t");
    Color_Print("cyan", "2- ");
    if (color_mode) Color_Print("yellow", "Disable");
    else Color_Print("yellow", "Enable");
    Color_Print("white", " Color Mode\t");
    Color_Print("cyan", "3- ");
    Color_Print("red", "Cancel\n");

    cin >> option;
    while(option != "1" && option != "2" && option != "3") {
        invalid();
        cin >> option;
    }
    if(option == "1"){
        Color_Print("blue", "New CAP:", true);
        string cap;
        cin >> cap;
        while (!is_number(cap) || stoi(cap) > 50) {
            invalid();
            cin >> cap;
        }
        data.set_cap(stoi(cap));
        return;
    }
    if(option == "2"){
        color_mode = !color_mode;
        if (color_mode) Color_Print("cyan", "Color mode enabled", true);
        else Color_Print("cyan", "Color mode disabled", true);
        return;
    }
    run();
}

void Menu::handle_requests() {
    Color_Print("blue", "Select option:", true);
    Color_Print("white", "Add or remove registration");
    Color_Print("cyan", " - press 1", true);
    Color_Print("white", "Update registration");
    Color_Print("cyan", " - press 2", true);
    Color_Print("white", "Upload file with changes");
    Color_Print("cyan", " - press 3", true);
    Color_Print("white", "Undo Previous request");
    Color_Print("cyan", " - press 4", true);
    Color_Print("white", "Check pending requests");
    Color_Print("cyan", " - press 5", true);
    Color_Print("white", "Process pending requests");
    Color_Print("cyan", " - press 6", true);
    Color_Print("white", "Erase pending requests");
    Color_Print("cyan", " - press 7", true);
    Color_Print("red", "Cancel");
    Color_Print("cyan", " - press 8", true);

    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4" && option != "5" && option != "6" && option != "7" && option != "8") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            handle_registration();
            break;
        }
        case '2': {
            update_registration();
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
            data.print_pending_requests();
            break;
        }
        case '6': {
            string answer;
            Color_Print("blue", "Process: ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "Next request ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "All requests", true);
            cin >> answer;
            while(answer != "1" && answer != "2") {
                invalid();
                cin >> answer;
            }
            if (answer == "1") {
                do {
                    data.process_next_request();
                    if(data.pending_request_is_empty()) {
                        Color_Print("blue", "Processed all requests", true);
                        return;
                    }
                    Color_Print("blue", "Process next request? ");
                    Color_Print("cyan", "[Y/N]", true);
                    cin >> answer;
                    while (answer != "Y" && answer != "N") {
                        invalid();
                        cin >> answer;
                    }
                }
                while(answer == "Y");
            }
            else data.process_requests();
            break;
        }
        case '7':{
            data.empty_pending_requests();
            Color_Print("blue", "Emptied all pending requests",true);
            break;
        }
        case '8': {
            run();
        }
    }
}

void Menu::update_registration(){
    string type, option, uc_or_class, studentCode, current_class, new_class, current_uc, new_uc;
    Color_Print("blue", "Pick: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "ADD ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "REMOVE ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "SWITCH ");
    Color_Print("cyan", "4- ");
    Color_Print("red", "Cancel", true);

    cin >> option; while(option != "1" && option != "2" && option != "3" && option != "4") { invalid(); cin >> option; cout << '\n';}
    if(option == "4") {
        //data.process_requests(); ?
        handle_requests();
        return;
    }
    if (!data.get_studentCode_last_request().empty()) {
        Color_Print("blue", "Is the request for the same previous student? ");
        Color_Print("cyan", "[Y/N]", true);
        string answer;
        cin >> answer;
        while(answer != "Y" && answer != "N") {
            invalid();
            cin >> answer;
        }
        if (answer == "Y") studentCode = data.get_studentCode_last_request();
        else {
            Color_Print("blue", "Enter StudentCode ");
            Color_Print("green", "(e.g. 202020047)");
            Color_Print("blue", ":", true);
            cin >> studentCode;
            while(data.get_student_from_studentCode(studentCode) == nullptr){
                invalid();
                cin >> studentCode;
            }
        }
    }
    else {
        Color_Print("blue", "Enter StudentCode ");
        Color_Print("green", "(e.g. 202020047)");
        Color_Print("blue", ":", true);
        cin >> studentCode;
        while(!is_number(studentCode) || studentCode.length() != 9 ){
            invalid();
            cin >> studentCode;
        }
    }

    get_request(studentCode, option);

    Color_Print("blue", "Would you like to make another request? ");
    Color_Print("cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") {
        update_registration();
        return;
    }

    Color_Print("blue", "Would you like to process all requests now? ");
    Color_Print("cyan", "[Y/N]", true);
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if(answer == "Y") data.process_requests();
}

void Menu::get_request(string studentCode, string option) {
    string type, uc_or_class, current_class, new_class, current_uc, new_uc;
    switch (option[0]) {
        case '1': {
            type = "ADD";
            Color_Print("blue", "Enter UCcode ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            cin >> new_uc;
            while(!data.exists_Uc(new_uc)) {
                invalid();
                cin >> new_uc;
            }
            Color_Print("blue", "Do you want to join a specific class? ");
            Color_Print("cyan", "[Y/N]", true);
            string answer;
            cin >> answer;
            while(answer != "Y" && answer != "N") {
                invalid();
                cin >> answer;
            }
            if (answer=="Y") {
                uc_or_class = "CLASS";
                Color_Print("blue", "Enter Class ");
                Color_Print("green", "(e.g. 1LEIC01)");
                Color_Print("blue", ":", true);
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
            Color_Print("blue", "Enter UCcode ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            cin >> current_uc;
            while(!data.exists_Uc(current_uc)) {
                invalid();
                cin >> current_uc;
            }
            break;
        }
        case '3': {
            type = "SWITCH";
            Color_Print("blue", "Pick: ");
            Color_Print("cyan", "UC / CLASS", true);
            cin >> uc_or_class; while(uc_or_class != "UC" && uc_or_class != "CLASS") { invalid(); cin >> uc_or_class;}
            if (uc_or_class == "CLASS") {
                Color_Print("blue", "Enter UCcode ");
                Color_Print("green", "(e.g. L.EIC001)");
                Color_Print("blue", ":", true);
                cin >> current_uc;
                while(!data.exists_Uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                new_uc = current_uc;
                Color_Print("blue", "Enter new ClassCode ");
                Color_Print("green", "(e.g. 1LEIC01)");
                Color_Print("blue", ":", true);
                cin >> new_class;
                while(!data.exists_class(current_uc,new_class)) {
                    invalid();
                    cin >> new_class;
                }
            }
            else {
                Color_Print("blue", "Enter UCcode ");
                Color_Print("green", "(e.g. L.EIC001)");
                Color_Print("blue", ":", true);
                cin >> current_uc;
                while(!data.exists_Uc(current_uc)) {
                    invalid();
                    cin >> current_uc;
                }
                Color_Print("blue", "Enter new UCcode ");
                Color_Print("green", "(e.g. L.EIC001)");
                Color_Print("blue", ":", true);
                cin >> new_uc;
                while(!data.exists_Uc(new_uc)) {
                    invalid();
                    cin >> new_uc;
                }
                Color_Print("blue", "Do you want to join a specific class? ");
                Color_Print("cyan", "[Y/N]", true);
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    invalid();
                    cin >> answer;
                }
                if (answer=="Y") {
                    uc_or_class = "CLASS";
                    Color_Print("blue", "Enter ClassCode ");
                    Color_Print("green", "(e.g. 1LEIC01)");
                    Color_Print("blue", ":", true);
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
    data.add_request_to_process(Request(type, studentCode, "", current_uc, current_class, new_uc, new_class));
}

void Menu::handle_registration() {
    string option, type;
    Color_Print("blue", "Pick: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Add a new student ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Remove a student ");
    Color_Print("cyan", "3- ");
    Color_Print("red", "Cancel", true);
    cin >> option; while(option != "1" && option != "2" && option != "3") { invalid(); cin >> option; cout << '\n';}
    switch (option[0]) {
        case '1': {
            type = "NEW";
            string new_studentCode, name, number_ucs, new_uc, new_class;
            Color_Print("blue", "New StudentCode ");
            Color_Print("green", "(e.g. 202020047)");
            Color_Print("blue", ":", true);
            cin >> new_studentCode;
            while (!is_number(new_studentCode) || new_studentCode.length() != 9 || data.get_student_from_studentCode(new_studentCode) != nullptr) {
                invalid();
                cin >> new_studentCode;
            }
            Color_Print("blue", "Name of new student:", true);
            cin.ignore();
            getline(cin, name);
            data.add_request_to_process(Request(type, new_studentCode, name));
            Color_Print("blue", "Would you now like to add UC's to this student? ");
            Color_Print("cyan", "[Y/N]", true);
            string answer;
            cin >> answer;
            while(answer != "Y" && answer != "N") {
                invalid();
                cin >> answer;
            }
            if (answer=="Y") {
                Color_Print("blue", "How many UC's do you want to join?", true);
                cin >> number_ucs;
                while (!is_number(number_ucs) || stoi(number_ucs) > 7) {
                    invalid();
                    cin >> number_ucs;
                }
                for (int i = 0; i < stoi(number_ucs); i++) {
                    get_request(new_studentCode, "1");
                }
            }
            break;
        }
        case '2': {
            type = "DELETE";
            string studentCode;
            Color_Print("blue", "Student code of student to remove");
            Color_Print("green", "(e.g. 202020047)");
            Color_Print("blue", ":", true);
            cin >> studentCode;
            while (!is_number(studentCode) || studentCode.length() != 9) {
                invalid();
                cin >> studentCode;
            }
            data.add_request_to_process(Request(type, studentCode, data.get_student_from_studentCode(studentCode)->get_name()));
            break;
        }
        case '3': {
            handle_requests();
            return;
        }
    }
    Color_Print("blue", "Would you like to process all requests now? ");
    Color_Print("cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if(answer == "Y") data.process_requests();
}

void Menu::request_file() {
    Color_Print("white", "Save the file to this directory with the name ");
    Color_Print("green", "requests.csv ");
    Color_Print("white", "with the following structure:", true);
    Color_Print("white", "Type,StudentCode,oldUcCode,newUcCode,newClassCode - if Type in {");
    Color_Print("green", "ADD");
    Color_Print("white", ",");
    Color_Print("green", "REMOVE");
    Color_Print("white", ",");
    Color_Print("green", "SWITCH");
    Color_Print("white", "} leave fields empty if not applicable", true);
    Color_Print("white", "OR Type,StudentCode,StudentName - if Type is ");
    Color_Print("green", "NEW", true);
    Color_Print("white", "OR Type,StudentCode - if Type is ");
    Color_Print("green", "DELETE", true);
    Color_Print("blue", "Do you want to upload the requests? ");
    Color_Print("cyan", "[Y/N]", true);
    string answer;
    cin >> answer;
    while(answer != "Y" && answer != "N") {
        invalid();
        cin >> answer;
    }
    if (answer == "Y") {
        data.upload_requests();

        Color_Print("blue", "Would you like to process all requests now? ");
        Color_Print("cyan", "[Y/N]", true);
        cin >> answer;
        while (answer != "Y" && answer != "N") {
            invalid();
            cin >> answer;
        }
        if (answer == "Y") data.process_requests();
    }
}

void Menu::listings(){
    Color_Print("blue", "What would you like to check?", true);
    Color_Print("cyan", "1- ");
    Color_Print("white", "Schedules ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Student Lists ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "Occupations ");
    Color_Print("cyan", "4- ");
    Color_Print("white", "Students with at least n UC's ");
    Color_Print("cyan", "5- ");
    Color_Print("red", "Cancel", true);
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
            Color_Print("blue", "N:", true);
            string n;
            cin >> n;
            while (!is_number(n) || stoi(n) > 7) {
                invalid();
                cin >> n;
            }
            data.students_in_n_Ucs(stoi(n));
            break;
        }
        case '5': run();
    }
}

void Menu::occupations(){
    Color_Print("blue", "What would you like to check?", true);
    Color_Print("cyan", "1- ");
    Color_Print("white", "Year ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "UC ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "Class ");
    Color_Print("cyan", "4- ");
    Color_Print("red", "Cancel", true);
    string option; cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch (option[0]) {
        case '1': {
            Color_Print("blue", "Pick: ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "1st Year ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "2nd Year ");
            Color_Print("cyan", "3- ");
            Color_Print("white", "3rd Year ");
            Color_Print("cyan", "4- ");
            Color_Print("red", "Cancel", true);
            string year;
            cin >> year;
            while(year != "1" && year != "2" && year != "3" && year != "4") {
                invalid();
                cin >> option;
            }
            if(year == "4") {
                occupations();
                break;
            }
            year_occupations(year);
            break;
        }
        case '2': {
            Color_Print("blue", "Enter UCcode ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            Uc_occupations(UC);
            break;
        }
        case '3': {
            Color_Print("blue", "Enter Class to check ");
            Color_Print("green", "(e.g. 1LEIC01)");
            Color_Print("blue", ":", true);
            string classCode; cin >> classCode;
            while(!data.get_classCodes().count(classCode)) {
                invalid();
                cin >> classCode;
            }
            class_occupations(classCode);
            break;
        }
        case '4': {
            listings();
            break;
        }
    }
}

void Menu::listSchedules(){
    Color_Print("blue", "Consult schedule of: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Student ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Class ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "UC ");
    Color_Print("cyan", "4- ");
    Color_Print("red", "Cancel", true);
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            Color_Print("blue", "Insert StudentCode ");
            Color_Print("green", "(e.g. 202020047)");
            Color_Print("blue", ":", true);
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
            Color_Print("blue", "Enter ClassCode ");
            Color_Print("green", "(e.g. 1LEIC01)");
            Color_Print("blue", ":", true);
            string class_;
            cin >> class_;
            while(!data.get_classCodes().count(class_)) {    //mudar para exists classcode???????
                invalid();
                cin >> class_;
            }
            Color_Print("blue", "Schedule of Class ");
            Color_Print("yellow", class_, true);

            set<Lesson> classLessons;
            vector<Class> classes = data.get_classes();
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
                    Color_Print("blue", lesson.get_weekday(), true);
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    Color_Print("blue", lesson.get_weekday(), true);
                }
                lesson.print_lesson();
            }
            break;
        }
        case '3': {
            Color_Print("blue", "Enter UCcode ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            string uc;
            cin >> uc;
            while(!data.get_UcCodes().count(uc)) {
                invalid();
                cin >> uc;
            }
            Color_Print("blue", "Schedule of UC ");
            Color_Print("yellow", uc, true);

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
                    Color_Print("blue", lesson.get_weekday(), true);
                }
                if (lesson.get_weekday() != currentDay) {
                    currentDay = lesson.get_weekday();
                    Color_Print("blue", lesson.get_weekday(), true);
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

void Menu::listStudents() {
    Color_Print("blue", "Consult list of students of: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Course ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "UC ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "Class ");
    Color_Print("cyan", "4- ");
    Color_Print("red", "Cancel", true);
    string option;
    cin >> option;
    while(option != "1" && option != "2" && option != "3" && option != "4") {
        invalid();
        cin >> option;
    }
    switch(option[0]){
        case '1': {
            Color_Print("blue", "Sort student by? ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "StudentCode ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2" && option != "3") {
                invalid();
                cin >> option;
            }
            Color_Print("blue", "Pick: ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "Ascending Order ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Descending Order", true);
            string order;
            cin >> order;
            while(order != "1" && order != "2"){
                invalid();
                cin >> order;
            }
            if(option == "1") data.list_students_by_studentCode(order == "1");
            else data.list_students_by_name(order == "1");
            break;
        }
        case '2': {
            Color_Print("blue", "Enter UCcode ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            Color_Print("blue", "Sort student by? ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "StudentCode ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            string order;
            Color_Print("blue", "Pick: ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "Ascending Order ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Descending Order", true);
            cin >> order;
            while(order != "1" && order != "2"){
                invalid();
                cin >> order;
            }
            if(option == "1") data.list_UC_students_by_studentCode(UC, order == "1");
            else data.list_UC_students_by_name(UC, order == "1");
            break;
        }
        case '3': {
            Color_Print("blue", "Enter UC of Class to check ");
            Color_Print("green", "(e.g. L.EIC001)");
            Color_Print("blue", ":", true);
            string UC; cin >> UC;
            while(!data.exists_Uc(UC)) {
                invalid();
                cin >> UC;
            }
            Color_Print("blue", "Enter Class to check ");
            Color_Print("green", "(e.g. 1LEIC01)");
            Color_Print("blue", ":", true);
            string class_; cin >> class_;
            while(!data.exists_class(UC,class_)) {
                invalid();
                cin >> class_;
            }
            cout << endl;
            Color_Print("blue", "Sort student by? ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "StudentCode ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Student Name", true);
            cin >> option;
            while(option != "1" && option != "2") {
                invalid();
                cin >> option;
            }
            string order;
            Color_Print("blue", "Pick: ");
            Color_Print("cyan", "1- ");
            Color_Print("white", "Ascending Order ");
            Color_Print("cyan", "2- ");
            Color_Print("white", "Descending Order", true);
            cin >> order;
            while(order != "1" && order != "2"){
                invalid();
                cin >> order;
            }
            if(option == "1") data.list_class_students_by_studentCode(data.get_class_from_classCode_and_UcCode(class_, UC), order == "1");
            else data.list_class_students_by_name(data.get_class_from_classCode_and_UcCode(class_, UC), order == "1");
            break;
        }
        case '4': listings();
    }
}

void Menu::year_occupations(string year) {
    string option, order;
    Color_Print("blue", "Sort by? ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "UC ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "ClassCode ");
    Color_Print("cyan", "3- ");
    Color_Print("white", "Occupation", true);
    cin >> option;
    while(option != "1" && option != "2" && option != "3"){
        invalid();
        cin >> option;
    }
    Color_Print("blue", "Pick: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Ascending Order ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Descending Order", true);
    cin >> order;
    while(order != "1" && order != "2"){
        invalid();
        cin >> order;
    }
    if(option == "1") {
        data.list_year_occupations_by_UC(year, order == "1"); return;
    }
    if(option == "2") {
        data.list_year_occupations_by_classCode(year, order == "1"); return;
    }
    data.list_year_occupations_by_occupation(year, order == "1");
}



void Menu::Uc_occupations(std::string UcCode) {
    string option, order;
    Color_Print("blue", "Sort by? ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "ClassCode ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Occupation", true);
    cin >> option;
    while (option != "1" && option != "2") {
        invalid();
        cin >> option;
    }
    Color_Print("blue", "Pick: ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Ascending Order ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Descending Order", true);
    cin >> order;
    while (order != "1" && order != "2") {
        invalid();
        cin >> order;
    }
    if (option == "1") {
        data.list_Uc_occupations_by_classCode(UcCode, order == "1");
        return;
    }
    data.list_Uc_occupations_by_occupation(UcCode, order == "1");
}


void Menu::class_occupations(std::string classCode) {
    string option, order;
    Color_Print("blue", "Sort by? ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "UcCode ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Occupation", true);
    cin >> option;
    while(option != "1" && option != "2"){
        invalid();
        cin >> option;
    }
    Color_Print("blue", "Sort by? ");
    Color_Print("cyan", "1- ");
    Color_Print("white", "Ascending Order ");
    Color_Print("cyan", "2- ");
    Color_Print("white", "Descending Order", true);
    cin >> order;
    while(order != "1" && order != "2"){
        invalid();
        cin >> order;
    }
    if (option == "1") {
        data.list_class_occupations_by_UC(classCode, order == "1");
        return;
    }
    data.list_class_occupations_by_occupation(classCode, order == "1");
}