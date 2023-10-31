#include "leic.h"
#include "color_print.h"
using namespace std;
extern bool color_mode;

bool is_number2(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

LEIC::LEIC(std::string filenameclasses, std::string filenamestudents, bool save_file) {
    ifstream classesFile(filenameclasses);
    string line;
    getline(classesFile, line);
    string classcode, uccode, weekday, starthour, duration, type;
    while (getline(classesFile, line)) {     // read all lines from the given file
        istringstream iss(line);
        getline(iss, classcode, ',');
        getline(iss, uccode, ',');
        getline(iss, weekday, ',');
        getline(iss, starthour, ',');
        getline(iss, duration, ',');
        iss >> type; // to do: try with getline
        ucs.insert(uccode);
        Class c = Class(classcode, uccode);
        string thisclass = uccode + " " + classcode;
        Lesson lesson = Lesson(thisclass, weekday, Time(starthour), Time(stof(duration)+stof(starthour)), type); // create new lesson
        auto it = find(classes.begin(), classes.end(), c);
        if(it != classes.end()){
            it->add_lesson(lesson);
        }
        else {
            c.add_lesson(lesson);
            classes.push_back(c);
        }
    }
    classesFile.close();

    sort(classes.begin(),classes.end());

    ifstream studentsFile(filenamestudents);
    getline(studentsFile, line);
    string up, name;
    while (getline(studentsFile, line)) {     // read all lines from the given file
        istringstream iss(line);
        getline(iss, up, ',');
        getline(iss, name, ',');
        getline(iss, uccode, ',');
        iss >> classcode;

        Student s = Student(name, up);
        auto it = up_students.find(up);
        if(it == up_students.end()){
            up_students.insert({up,s});
        }
        if (!uccode.empty()) {
            Class c = Class(classcode,uccode);
            auto it2 = find(classes.begin(), classes.end(), c);
            up_students.at(up).add_class(&(*it2));
            it2->add_student(up);
        }
    }
    studentsFile.close();
    if(save_file){
        ifstream requestsFile("../accepted_requests.csv");
        getline(requestsFile, line);
        string StudentCode,Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
        stack<Request> reverseOrderRequest;
        while (getline(requestsFile, line)) {     // read all lines from the given file
            istringstream iss(line);
            getline(iss, Type, ',');
            getline(iss, StudentCode, ',');
            getline(iss, oldUcCode, ',');
            getline(iss, newUcCode, ',');
            getline(iss, oldClassCode, ',');
            iss >> newClassCode;
            reverseOrderRequest.emplace(Type,true,StudentCode,oldClassCode,newClassCode,oldUcCode,newUcCode);
        }
        while(!reverseOrderRequest.empty()){
            processed_requests.push(reverseOrderRequest.top());
            reverseOrderRequest.pop();
        }
        requestsFile.close();
    }
}

Student* LEIC::get_student_from_studentCode(std::string studentCode) {
    return (up_students.find(studentCode) == up_students.end()) ? nullptr : &up_students.at(studentCode);
}

vector<Class> LEIC::get_classes() const {
    return classes;
}

std::set<std::string> LEIC::get_UcCodes() const {
    return ucs;
}

std::set<std::string> LEIC::get_classCodes() const {
    set<string> classcodes;
    for(Class c: classes) classcodes.insert(c.get_classCode());
    return classcodes;
}

Class* LEIC::get_class_from_classCode_and_UcCode(std::string classcode, std::string uccode) {
    if(!exists_class(uccode,classcode)) return nullptr;
    auto first_itr = lower_bound(classes.begin(),classes.end(),Class(classcode,uccode));
    return first_itr != classes.end() ? &(*first_itr) : nullptr;
}

vector<Class*> LEIC::get_classes_from_UcCode(std::string ucCode) {
    vector<Class*> classes_ucCode;
    if(!exists_Uc(ucCode)) return classes_ucCode;
    auto first_itr = lower_bound(classes.begin(),classes.end(),Class("",ucCode));
    while(first_itr->get_ucCode() == ucCode) classes_ucCode.push_back(&(*(first_itr++)));
    return classes_ucCode;
}

int LEIC::get_cap() {
    return CAP;
}

void LEIC::set_cap(int cap) {
    CAP = cap;
}

bool LEIC::exists_class(string ucCode, string classCode){
    return binary_search(classes.begin(),classes.end(),Class(classCode,ucCode));
}

bool LEIC::exists_Uc(std::string ucCode) {
    return ucs.count(ucCode);
}

bool LEIC::Uc_has_vacancy(std::string uccode) {
    for (Class& c: classes) {
        if (c.get_ucCode() == uccode){
            if (c.get_students().size() < CAP) return true;
        }
    }
    return false;
}

void LEIC::list_students_by_studentCode() {
    Color_Print(color_mode, "blue", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "white", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    for(pair<string, Student> p : up_students){
        Color_Print(color_mode, "blue", p.first);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "white", p.second.get_name(), true);
    }
}

void LEIC::list_students_by_name(){
    Color_Print(color_mode, "white", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "blue", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    map<string, string> students_up;
    for(pair<string, Student> p : up_students){
        students_up[p.second.get_name()] = p.first;
    }
    for(pair<string, string> p : students_up){
        Color_Print(color_mode, "white", p.second);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "blue", p.first, true);
    }
}

void LEIC::list_UC_students_by_studentCode(std::string UcCode) {
    Color_Print(color_mode, "blue", "Students of UC ");
    Color_Print(color_mode, "yellow", UcCode, true);
    Color_Print(color_mode, "blue", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "white", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    set<string> studentinUC;
    for(Class c : classes){
        if(c.get_ucCode() == UcCode){
            set<string> studentUC = c.get_students();
            studentinUC.insert(studentUC.begin(), studentUC.end());
        }
    }
    for(string code : studentinUC){
        Color_Print(color_mode, "blue", code);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "white", up_students.at(code).get_name(), true);
    }
}

void LEIC::list_UC_students_by_name(std::string uc) {
    Color_Print(color_mode, "blue", "Students of UC ");
    Color_Print(color_mode, "yellow", uc, true);
    Color_Print(color_mode, "white", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "blue", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    map<string, string> UCstudents_up;
    for(Class c : classes){
        if(c.get_ucCode() == uc) {
            for (string up: c.get_students()) UCstudents_up[up_students.at(up).get_name()] = up;
        }
    }
    for (pair<string, string> p: UCstudents_up){
        Color_Print(color_mode, "white", p.second);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "blue", p.first, true);
    }
}

void LEIC::list_class_students_by_studentCode(Class *class_) const {
    Color_Print(color_mode, "blue", "Students of Class ");
    Color_Print(color_mode, "yellow", class_->get_classCode());
    Color_Print(color_mode, "blue", " of UC ");
    Color_Print(color_mode, "yellow", class_->get_ucCode(), true);
    Color_Print(color_mode, "blue", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "white", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    for(string up : class_->get_students()){
        Color_Print(color_mode, "blue", up);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "white", up_students.at(up).get_name(), true);
    }
}

void LEIC::list_class_students_by_name(Class *class_) const {
    Color_Print(color_mode, "blue", "Students of Class ");
    Color_Print(color_mode, "yellow", class_->get_classCode());
    Color_Print(color_mode, "blue", " of UC ");
    Color_Print(color_mode, "yellow", class_->get_ucCode(), true);
    Color_Print(color_mode, "white", "UPNUMBER");
    Color_Print(color_mode, "green", " | ");
    Color_Print(color_mode, "blue", "NAME", true);
    Color_Print(color_mode, "green", "------------------------------------", true);
    map<string, string> UCstudents_up;
    for (string up: class_->get_students()) UCstudents_up[up_students.at(up).get_name()] = up;
    for (pair<string, string> p: UCstudents_up){
        Color_Print(color_mode, "white", p.second);
        Color_Print(color_mode, "green", " | ");
        Color_Print(color_mode, "blue", p.first, true);
    }
}

void LEIC::list_number_students_class() {
    for (Class c: classes) {
        int currentSize = c.get_students().size();
        Color_Print(color_mode, "blue", c.get_classCode() + " " + c.get_ucCode() + " " + to_string(currentSize), true);
    }
}

int LEIC::students_in_n_Ucs(int n){
    int count = 0;
    for (pair<string, Student> p: up_students) {
        if(p.second.get_classes().size() >= n) {
            count++;
            Color_Print(color_mode, "white", p.first);
            Color_Print(color_mode, "green", " | ");
            Color_Print(color_mode, "white", p.second.get_name());
            Color_Print(color_mode, "green", " - ");
            Color_Print(color_mode, "blue", to_string(p.second.get_classes().size()), true);
        }
    }
    return count;
}

Class* LEIC::class_balance_valid(Class* newClass) {
    Class* max = newClass;
    Class* min = newClass;
    for (Class& c: classes) {
        if (c.get_ucCode() == newClass->get_ucCode()){
            if (c.get_students().size() < min->get_students().size()) min = &c;
        }
    }
    if (*max == *min || (max->get_students().size()+1)-min->get_students().size() <= 4) return nullptr;
    return min;
}

bool LEIC::compatible_schedules(Student student, Class* newclass, Class* oldclass) {
//    for (Lesson newlesson: newclass->get_lessons()) {
//        if (newlesson.get_type() != "T") {
//            for (Class* c: student.get_classes()) {
//                if (c != oldclass) {
//                    set<Lesson> currentLessons = c->get_lessons();
//                    auto itr = lower_bound(currentLessons.begin(),currentLessons.end(),newlesson,
//                                           [] (Lesson l1, Lesson l2) -> bool {return l1.get_weekday() < l2.get_weekday();});
//
//                    while(itr->get_weekday() == newlesson.get_weekday()){
//                        if(itr->get_type() != "T") {
//                            if (newlesson.overlap(*itr)) return false;
//                        }
//                        itr++;
//                    }
//                }
//            }
//        }
//    }
    for (Lesson newlesson: newclass->get_lessons()) {
        if (newlesson.get_type() == "PL" || newlesson.get_type() == "TP") {
            for (Class* c: student.get_classes()) {
                if (c == oldclass) {
                    continue;
                }
                for (Lesson currentlesson: c->get_lessons()) {
                    if (currentlesson.get_type() == "PL" || currentlesson.get_type() == "TP") {
                        if (newlesson.overlap(currentlesson)) return false;
                    }
                }

            }
        }
    }
    return true;
}

void LEIC::add_student(Student student) {
    up_students.insert({student.get_studentCode(), student});
}

void LEIC::add_student_to_class(Student* student, Class *newclass) {
    newclass->add_student(student->get_studentCode());
    student->add_class(newclass);
}

void LEIC::remove_student_from_class(Student* student, Class *newclass) {
    newclass->remove_student(student->get_studentCode());
    student->remove_class_from_uc(newclass->get_ucCode());
}

void LEIC::add_request_to_process(Request request) {
    requests.push(request);
}

void LEIC::add_processed_request(Request request) {
    processed_requests.push(request);
}

void LEIC::upload_requests() {
    string line;
    ifstream requestsFile("../requests.csv");
    string StudentCode,Type,oldUcCode,newUcCode,newClassCode, studentName;
    int countLines = 0;
    int countNews = 0;
    while (getline(requestsFile, line)) {     // read all lines from the given file
        istringstream iss(line);
        getline(iss, Type, ',');
        bool add = Type == "ADD";
        bool remove = Type == "REMOVE";
        bool Switch = Type == "SWITCH";
        if (Type == "NEW") {
            getline(iss, StudentCode, ',');
            if (!(is_number2(StudentCode) && StudentCode.size() == 9)) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            getline(iss, studentName);
            add_student(Student(studentName, StudentCode));
            requests.emplace("NEW", false, StudentCode);
            processed_requests.emplace("NEW", false, StudentCode);
            countNews++;
        }
        else if (add || remove || Switch) {
            getline(iss, StudentCode, ',');
            if (get_student_from_studentCode(StudentCode) == nullptr) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                for (int j = 0; j < countNews; j++) undo_request();
                return;
            }
            getline(iss, oldUcCode, ',');
            if ((add && !oldUcCode.empty())
                || ((!add) && (!get_student_from_studentCode(StudentCode)->has_uc(oldUcCode) || oldUcCode.empty()))) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                for (int j = 0; j < countNews; j++) undo_request();
                return;
            }
            getline(iss, newUcCode, ',');
            if ((!remove) && (!exists_Uc(newUcCode) || get_student_from_studentCode(StudentCode)->has_uc(newUcCode))
                || (remove && !newUcCode.empty())) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                for (int j = 0; j < countNews; j++) undo_request();
                return;
            }
            iss >> newClassCode;
            if (((!remove) && !exists_class(newUcCode,newClassCode))
                || (remove && !newClassCode.empty())) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                for (int j = 0; j < countNews; j++) undo_request();
                return;
            }
            requests.emplace(Type,!newClassCode.empty(),StudentCode,"",newClassCode,oldUcCode,newUcCode);
        }
        else {cout << "Invalid input in the given file. Line " << countLines << endl; empty_pending_requests(); for (int j = 0; j < countNews; j++) undo_request(); return;}
        countLines++;
    }
    requestsFile.close();
    process_requests();
}

std::string LEIC::request_add(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (student->has_uc(request.get_new_UcCode())) {
        cout << "Invalid request. Student is already in the UC " << request.get_new_UcCode() << "." << endl;
        return "Request Error";
    }
    if (request.get_Uc_class()) {
        Class* newclass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_new_UcCode());
        if (newclass->get_students().size() >= CAP){
            Color_Print(color_mode, "red", "Request rejected. The class ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "red", " from the UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " it's full.", true);
            return "Request error";
        }
        if (!compatible_schedules(*student, newclass)){
            Color_Print(color_mode, "red", "Request rejected. The schedule of the class ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "red", " from the UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " it's not compatible.", true);
            return "Request error";
        }
        Class* suggestion = class_balance_valid(newclass);
        if (suggestion != nullptr) {
            Color_Print(color_mode, "green", "Joining the class ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "green", " in the UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "green", " affects class balance. Do you want to:", true);
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "white", "Proceed with the operation \t");
            Color_Print(color_mode, "cyan", "2- ");
            Color_Print(color_mode, "white", "Change to class ");
            Color_Print(color_mode, "yellow", suggestion->get_classCode(), true);
            string option;
            cin >> option; while(option != "1" && option != "2") {Color_Print(color_mode, "red", "Invalid Input, please try again", true); cin >> option; cout << '\n';}
            if (option == "2") {
                add_student_to_class(student, suggestion);
                request.set_new_class(suggestion->get_classCode());
                processed_requests.emplace("ADD", true, request.get_studentCode(), "", suggestion->get_classCode(),"", suggestion->get_ucCode());
                return "";
            }
        }
        add_student_to_class(student, newclass);
        processed_requests.push(request);
        return "";
    }
    else {
        if (Uc_has_vacancy(request.get_new_UcCode()) && student->get_classes().size()<7) {
            vector<Class*> classes_uccode = get_classes_from_UcCode(request.get_new_UcCode());
            for (Class* c: classes_uccode) {
                if (compatible_schedules(*student, c) && class_balance_valid(c)) {
                    add_student_to_class(student, c);
                    request.set_new_class(c->get_classCode());
                    processed_requests.push(request);
                    return "";
                }
            }
        }
        Color_Print(color_mode, "red", "All classes from the UC ");
        Color_Print(color_mode, "yellow", request.get_new_UcCode());
        Color_Print(color_mode, "red", " are full.", true);
        return "Request error";
    }
}

bool LEIC::request_remove(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    Class* currentClass = student->get_class_from_uc(request.get_current_UcCode());
    if (currentClass == nullptr) return false;
    string currentclass = currentClass->get_classCode();
    string currentUc = request.get_current_UcCode();
    request.set_current_class(currentclass);
    remove_student_from_class(student, currentClass);
    processed_requests.push(request);
    return true;
}

std::string LEIC::request_switch(Request& request) {
    request.set_type("REMOVE");
    if (!request_remove(request)) {
        cout << "Invalid request. Student is not in the UC " << request.get_current_UcCode() << endl;
        return "Request Error";
    }
    request.set_type("ADD");
    Class* newclass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_current_UcCode());
    if (request.get_Uc_class() && (class_balance_valid(newclass) != nullptr)) {
        Request removerequest = Request("ADD", true, request.get_studentCode(), "", request.get_current_classCode(),  "", request.get_current_UcCode());
        request_add(removerequest);
        Color_Print(color_mode, "red", "Invalid request. Switching class ");
        Color_Print(color_mode, "yellow", request.get_current_classCode());
        Color_Print(color_mode, "red", " for ");
        Color_Print(color_mode, "yellow", request.get_new_classCode());
        Color_Print(color_mode, "red", " in the UC ");
        Color_Print(color_mode, "yellow", request.get_current_UcCode());
        Color_Print(color_mode, "red", " violates class balance.", true);
        return "Request error";
    }
    string message = request_add(request);
    if (message == "") {
        processed_requests.pop();
        processed_requests.pop();
        request.set_type("SWITCH");
        processed_requests.push(request);
        return "";
    }
    else {
        add_student_to_class(get_student_from_studentCode(request.get_studentCode()),
                             get_class_from_classCode_and_UcCode(request.get_current_classCode(), request.get_current_UcCode()));
    }
    return message;
}

bool LEIC::undo_request() {
    if(processed_requests.empty()) {
        Color_Print(color_mode, "cyan", "There are no requests to undo", true);
        return false;
    }
    Request thisrequest = processed_requests.top();
    processed_requests.pop();
    bool valid = false;
    switch (thisrequest.get_type()[0]) {
        case 'A': {
            Request newrequest = Request("REMOVE", true, thisrequest.get_studentCode(), "", "", thisrequest.get_new_UcCode(), "");
            valid = request_remove(newrequest);
            if (valid) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " was removed from class ");
                Color_Print(color_mode, "yellow", newrequest.get_current_classCode());
                Color_Print(color_mode, "cyan", " in the UC ");
                Color_Print(color_mode, "yellow", newrequest.get_current_UcCode(), true);
                processed_requests.pop();
            }
            return valid;
        }
        case 'R': {
            Request newrequest = Request("ADD", true, thisrequest.get_studentCode(), "", thisrequest.get_current_classCode(),  "", thisrequest.get_current_UcCode());
            valid = request_add(newrequest) == "";
            if (valid) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " is now in the class ");
                Color_Print(color_mode, "yellow", newrequest.get_new_classCode());
                Color_Print(color_mode, "cyan", " in the UC ");
                Color_Print(color_mode, "yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return valid;
        }
        case 'S': {
            Request newrequest = Request("SWITCH", true, thisrequest.get_studentCode(),thisrequest.get_new_classCode(), thisrequest.get_current_classCode(), thisrequest.get_new_UcCode(), thisrequest.get_current_UcCode());
            valid = request_switch(newrequest) == "";
            if (valid) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " was removed from class ");
                Color_Print(color_mode, "yellow", newrequest.get_current_classCode());
                Color_Print(color_mode, "cyan", " in the UC ");
                Color_Print(color_mode, "yellow", newrequest.get_current_UcCode());
                Color_Print(color_mode, "cyan", " and is now in the class ");
                Color_Print(color_mode, "yellow", newrequest.get_new_classCode());
                Color_Print(color_mode, "cyan", " in the UC ");
                Color_Print(color_mode, "yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return valid;
        }
        case 'N': {
            Color_Print(color_mode, "cyan", "Student with the student code ");
            Color_Print(color_mode, "yellow", thisrequest.get_studentCode());
            Color_Print(color_mode, "cyan", " was removed.", true);
            up_students.erase(thisrequest.get_studentCode());
            return true;
        }
    }
    return valid;
}

void LEIC::process_requests() {
    while (!requests.empty()) {
        Request request = requests.front();
        requests.pop();
        switch (request.get_type()[0]) {
            case 'A': {
                string message = request_add(request);
                if (message == "") {
                    Color_Print(color_mode, "cyan", "Student is now in the class ");
                    Color_Print(color_mode, "yellow", request.get_new_classCode());
                    Color_Print(color_mode, "cyan", " in the UC ");
                    Color_Print(color_mode, "yellow", request.get_new_UcCode(), true);
                }
                break;
            }
            case 'R': {
                if (request_remove(request)){
                    Color_Print(color_mode, "cyan", "Student was removed from class ");
                    Color_Print(color_mode, "yellow", request.get_current_classCode());
                    Color_Print(color_mode, "cyan", " in the UC ");
                    Color_Print(color_mode, "yellow", request.get_current_UcCode(), true);
                }
                else cout << "Student is not in the UC, it can't be removed.\n";
                break;
            }
            case 'S': {
                string message = request_switch(request);
                if (message == "") {
                    Color_Print(color_mode, "cyan", "Student was removed from class ");
                    Color_Print(color_mode, "yellow", request.get_current_classCode());
                    Color_Print(color_mode, "cyan", " in the UC ");
                    Color_Print(color_mode, "yellow", request.get_current_UcCode());
                    Color_Print(color_mode, "cyan", " and is now in the class ");
                    Color_Print(color_mode, "yellow", request.get_new_classCode());
                    Color_Print(color_mode, "cyan", " in the UC ");
                    Color_Print(color_mode, "yellow", request.get_new_UcCode(), true);
                }
                break;
            }
            case 'N': {
                cout << "Student with code " << request.get_studentCode() << " was added.\n";
                break;
            }
        }
    }
}

void LEIC::save_to_files() {
    ofstream students_classesSaveFile("../students_classes_save.csv", ofstream::out | ofstream::trunc);
    ofstream accepted_requests("../accepted_requests.csv", ofstream::out | ofstream::trunc);
    students_classesSaveFile << "StudentCode,StudentName,UcCode,ClassCode" << endl;
    accepted_requests << "Type,StudentCode,oldUcCode,newUcCode,oldClassCode,newClassCode" << endl;
    for (pair<string, Student> up_s: up_students) {
        if (up_s.second.get_classes().empty()) {
            students_classesSaveFile << up_s.first << ','
                                     << up_s.second.get_name() << ',' << ',' << endl;
        }
        for (Class *c: up_s.second.get_classes()) {
            students_classesSaveFile << up_s.first << ','
                                     << up_s.second.get_name() << ','
                                     << c->get_ucCode() << ','
                                     << c->get_classCode() << endl;
        }
    }
    while(!processed_requests.empty()){
        Request r = processed_requests.top();
        processed_requests.pop();
        accepted_requests << r.get_type() << ','
                          << r.get_studentCode() << ','
                          << r.get_current_UcCode() << ','
                          << r.get_new_UcCode() << ','
                          << r.get_current_classCode() << ','
                          << r.get_new_classCode() << endl;
    }
    students_classesSaveFile.close();
    accepted_requests.close();
}

void LEIC::empty_pending_requests() {
    while (!requests.empty()) {
        requests.pop();
    }
}
