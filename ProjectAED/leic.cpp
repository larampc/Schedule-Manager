#include "leic.h"
#include "color_print.h"
using namespace std;
extern bool color_mode;

bool is_number2(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

bool LEIC::improves_balance(Class* currentClass, Class* newClass){
    if(newClass->get_students().size() < currentClass->get_students().size()) return true;
    vector<Class*> uc_classes = get_classes_from_UcCode(newClass->get_ucCode());
    // int avg = 0;
    int min = currentClass->get_students().size();
    int max = min;
    for(Class* c : uc_classes) {
        if (c == newClass){
            if (c->get_students().size() < min) min = c->get_students().size()+1;
            if (c->get_students().size() > max) max = c->get_students().size()+1;
        }
        if (c == currentClass){
            if (c->get_students().size() < min) min = c->get_students().size()-1;
            if (c->get_students().size() > max) max = c->get_students().size()-1;
        }
        else{
            if (c->get_students().size() < min) min = c->get_students().size();
            if (c->get_students().size() > max) max = c->get_students().size();
        }
       // avg += c->get_students().size();
    }
    return (max - min <= 4);
//    avg /= uc_classes.size();
//    double current_stddev = 0, new_stddev = 0;
//    for(Class* c : uc_classes) current_stddev += (c->get_students().size()-avg)*(c->get_students().size()-avg);
//
//    for(Class* c : uc_classes) {
//        if (currentClass->get_ucCode() == newClass->get_ucCode() && c == currentClass)
//            new_stddev += (c->get_students().size() - 1 - avg) * (c->get_students().size() - 1 - avg);
//        else if (c == newClass)
//            new_stddev += (c->get_students().size() + 1 - avg) * (c->get_students().size() + 1 - avg);
//        else new_stddev += (c->get_students().size() - avg) * (c->get_students().size() - avg);
//    }
//   return new_stddev <= current_stddev;
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
        string StudentCode, studentName, Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
        stack<Request> reverseOrderRequest;
        while (getline(requestsFile, line)) {     // read all lines from the given file
            istringstream iss(line);
            getline(iss, Type, ',');
            getline(iss, StudentCode, ',');
            getline(iss, studentName, ',');
            getline(iss, oldUcCode, ',');
            getline(iss, oldClassCode, ',');
            getline(iss, newUcCode, ',');
            iss >> newClassCode;
            reverseOrderRequest.emplace(Type, StudentCode, studentName, oldUcCode,oldClassCode,newUcCode,newClassCode);
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

const vector<Class>* LEIC::get_classes() const {
    return &classes;
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
    Color_Print(color_mode, "blue", "StudentCode");
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
    Color_Print(color_mode, "white", "StudentCode");
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
    Color_Print(color_mode, "blue", "StudentCode");
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
    Color_Print(color_mode, "white", "StudentCode");
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
    Color_Print(color_mode, "blue", "StudentCode");
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
    Color_Print(color_mode, "white", "StudentCode");
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

set<Class*> LEIC::class_balance_valid(Student* student, Class* newClass) {
    int min = newClass->get_students().size();
    int max = min;
    set<Class*> res;
    vector<Class*> classes_in_uc = get_classes_from_UcCode(newClass->get_ucCode());
    for (Class* c: classes_in_uc) {
        if (c->get_students().size() < min) min = c->get_students().size();
        if (c->get_students().size() > max) max = c->get_students().size();
    }
    if(newClass->get_students().size() + 1 - min <= 4) return {};
    for (Class* c: classes_in_uc) {
        if ((c->get_students().size() + 1 - min <= 4) && compatible_schedules(student, c)) res.insert(c);
    }
    if (res.empty() && max == newClass->get_students().size()) {
        for (Class* c: classes_in_uc) {
            if ((c->get_students().size() < max) && compatible_schedules(student, c)) res.insert(c);
        }
    }
    return res;
}

Class* LEIC::best_class_balance(Student* student, std::string uc) {
    vector<Class*> classes_in_uc = get_classes_from_UcCode(uc);
    Class* min = classes_in_uc[0];
    for (Class* c: classes_in_uc) if ((c->get_students().size() < min->get_students().size()) && compatible_schedules(student, c)) min = c;
    return min;
}

bool LEIC::compatible_schedules(Student* student, Class* newclass, Class* oldclass) {
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
            for (Class* c: student->get_classes()) {
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


void LEIC::add_student_to_class(Student* student, Class *newclass) {
    newclass->add_student(student->get_studentCode());
    student->add_class(newclass);
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
    int countLines = 1;
    while (getline(requestsFile, line)) {     // read all lines from the given file
        string StudentCode,Type,oldUcCode,newUcCode,newClassCode, studentName;
        istringstream iss(line);
        getline(iss, Type, ',');
        bool add = Type == "ADD";
        bool remove = Type == "REMOVE";
        bool Switch = Type == "SWITCH";
        if (Type == "NEW") {
            getline(iss, StudentCode, ',');
            if (!(is_number2(StudentCode) && StudentCode.length() == 9)) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            getline(iss,studentName);
            requests.emplace(Type,StudentCode, studentName,"","","", "");
        }
        else if (Type == "DELETE") {
            iss >> StudentCode;
            if (!is_number2(StudentCode) || StudentCode.length() != 9) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            getline(iss,studentName);
            requests.emplace(Type,StudentCode, "","","","", "");
        }
        else if (add || remove || Switch) {
            getline(iss, StudentCode, ',');
            if (!is_number2(StudentCode) || StudentCode.length() != 9) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            getline(iss, oldUcCode, ',');
            if ((add && !oldUcCode.empty())
                || ((!add) && (!exists_Uc(oldUcCode) || oldUcCode.empty()))) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            getline(iss, newUcCode, ',');
            if ((!remove) && (!exists_Uc(newUcCode))
                || (remove && !newUcCode.empty())) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            iss >> newClassCode;
            if (((!remove) && !exists_class(newUcCode,newClassCode))
                || (remove && !newClassCode.empty())) {
                cout << "Invalid input in the given file. Line " << countLines << endl;
                empty_pending_requests();
                return;
            }
            requests.emplace(Type,StudentCode, "",oldUcCode,"",newUcCode, newClassCode);
        }
        else {cout << "Invalid input in the given file. Line " << countLines << endl; empty_pending_requests(); return;}
        countLines++;
    }
    requestsFile.close();
    process_requests();
}

bool LEIC::request_add(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (student == nullptr) {
        cout << "Invalid request. The student " << request.get_studentCode() << " doesn't exist.";
        return false;
    }
    if (student->get_classes().size() > 7) {
        cout << "The student " << request.get_studentCode() << " already has 7 UC's.";
        return false;
    }
    if (student->has_uc(request.get_new_UcCode())) {
        cout << "Invalid request. Student " << request.get_studentCode() << " is already in UC " << request.get_new_UcCode() << "." << endl;
        return false;
    }
    if (!request.get_new_classCode().empty()) {
        Class* newclass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_new_UcCode());
        if (newclass->get_students().size() >= CAP){
            Color_Print(color_mode, "red", "Request rejected. The class ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "red", " from UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " it's full.", true);
            return false;
        }
        if (!compatible_schedules(student, newclass)){
            Color_Print(color_mode, "red", "Request rejected. The schedule of class ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "red", " from UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " it's not compatible.", true);
            return false;
        }
        set<Class*> suggestions = class_balance_valid(student, newclass);
        if (!suggestions.empty()) {
            cout << request.get_studentCode() << " joining class " << request.get_new_classCode()
                << " in UC " << request.get_new_UcCode() << " affects class balance. Do you want to:\n";
            Color_Print(color_mode, "cyan", "1- ");
            Color_Print(color_mode, "white", "Cancel operation \n");
            auto it = suggestions.begin();
            int i = 2;
            for (Class* c: suggestions) {
                cout << i++ << "- Change to class " << (*it++)->get_classCode() << endl;
            }
            string option;
            cin >> option;
            while(!is_number2(option) || stoi(option) >= i || stoi(option) == 0) {Color_Print(color_mode, "red", "Invalid Input, please try again", true); cin >> option;}
            if (option == "1") return false;
            else {
                it = suggestions.begin();
                advance(it, stoi(option) - 2);
                Class* suggestion = *it;
                add_student_to_class(student, suggestion);
                request.set_new_class(suggestion->get_classCode());
                processed_requests.emplace("ADD", request.get_studentCode(), "", "", "", suggestion->get_ucCode(),suggestion->get_classCode());
                return true;
            }
        }
        add_student_to_class(student, newclass);
        processed_requests.push(request);
        return true;
    }
    else {
        if (!Uc_has_vacancy(request.get_new_UcCode())) {
            Color_Print(color_mode, "red", "All classes from UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " are full.", true);
            return false;
        }
        vector<Class*> classes_uccode = get_classes_from_UcCode(request.get_new_UcCode());
        for (Class* c: classes_uccode) {
            if (best_class_balance(student, request.get_new_UcCode())) {
                add_student_to_class(student, c);
                request.set_new_class(c->get_classCode());
                processed_requests.push(request);
                return true;
            }
        }
        cout << "No compatible classes.";
        return false;
    }
}

bool LEIC::request_remove(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (student == nullptr) {
        cout << "Invalid request. The student " << request.get_studentCode() << " doesn't exist.";
        return false;
    }
    Class* currentClass = student->get_class_from_uc(request.get_current_UcCode());
    if (currentClass == nullptr) {
        cout << "Invalid request. The student " << request.get_studentCode() << " doesn't have UC " << request.get_current_UcCode() << endl;
        return false;
    }
    string currentclass = currentClass->get_classCode();
    string currentUc = request.get_current_UcCode();
    request.set_current_class(currentclass);
    currentClass->remove_student(student->get_studentCode());
    student->remove_class_from_uc(currentClass->get_ucCode());
    processed_requests.push(request);
    return true;
}

bool LEIC::request_new(Request &request) {
    if (get_student_from_studentCode(request.get_studentCode()) == nullptr) {
        up_students.insert({request.get_studentCode(), Student(request.get_studentName(), request.get_studentCode())});
        processed_requests.push(request);
        return true;
    }
    cout << "Student " << request.get_studentCode() << " already exists.\n";
    return false;
}

bool LEIC::request_delete(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    request.set_name(student->get_name());
    if (student == nullptr) {
        cout << "The student " << request.get_studentCode() << " doesn't exist.\n";
        return false;
    }
    for (Class* c: student->get_classes()) {
        c->remove_student(request.get_studentCode());
    }
    up_students.erase(request.get_studentCode());
    processed_requests.push(request);
    return true;
}

bool LEIC::request_switch(Request& request) {
    request.set_type("REMOVE");
    if (!request_remove(request)) return false;
    request.set_type("ADD");
    Student* student = get_student_from_studentCode(request.get_studentCode());
    Class* newClass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_new_UcCode());
    Class* currentClass = get_class_from_classCode_and_UcCode(request.get_current_classCode(), request.get_current_UcCode());

//    if (!request.get_new_classCode().empty() && !improves_balance(currentClass, newClass)) {
//        add_student_to_class(student,currentClass);
//        processed_requests.pop();
//        Color_Print(color_mode, "red", "Invalid request. Switching class ");
//        Color_Print(color_mode, "yellow", request.get_current_classCode());
//        if (request.get_new_UcCode() != request.get_current_UcCode()) {
//            Color_Print(color_mode, "red", " in UC ");
//            Color_Print(color_mode, "yellow", request.get_current_UcCode());
//        }
//        Color_Print(color_mode, "red", " for ");
//        Color_Print(color_mode, "yellow", request.get_new_classCode());
//        Color_Print(color_mode, "red", " in UC ");
//        Color_Print(color_mode, "yellow", request.get_new_UcCode());
//        Color_Print(color_mode, "red", " violates class balance.", true);
//        return false;
//    }
    if (request.get_current_UcCode() == request.get_new_UcCode()){ // mesma UC
        if (!improves_balance(currentClass, newClass)){
            add_student_to_class(student,currentClass);
            processed_requests.pop();
            Color_Print(color_mode, "red", "Invalid request. Switching class ");
            Color_Print(color_mode, "yellow", request.get_current_classCode());
            Color_Print(color_mode, "red", " for ");
            Color_Print(color_mode, "yellow", request.get_new_classCode());
            Color_Print(color_mode, "red", " in UC ");
            Color_Print(color_mode, "yellow", request.get_new_UcCode());
            Color_Print(color_mode, "red", " violates class balance.", true);
            return false;
        }
        add_student_to_class(get_student_from_studentCode(request.get_studentCode()), newClass);
        processed_requests.pop();
        request.set_type("SWITCH");
        processed_requests.push(request);
        return true;
    }
    if (request_add(request)) {
        processed_requests.pop();
        processed_requests.pop();
        request.set_type("SWITCH");
        processed_requests.push(request);
        return true;
    }
    else {
        add_student_to_class(get_student_from_studentCode(request.get_studentCode()),
                             get_class_from_classCode_and_UcCode(request.get_current_classCode(), request.get_current_UcCode()));
        processed_requests.pop();
    }
    return false;
}

void LEIC::undo_request() {
    if(processed_requests.empty()) {
        Color_Print(color_mode, "cyan", "There are no requests to undo", true);
        return;
    }
    Request thisrequest = processed_requests.top();
    processed_requests.pop();
    switch (thisrequest.get_type()[0]) {
        case 'A': {
            Request newrequest = Request("REMOVE", thisrequest.get_studentCode(), thisrequest.get_studentName(),thisrequest.get_new_UcCode(),"","", "");
            if (request_remove(newrequest)) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " was removed from class ");
                Color_Print(color_mode, "yellow", newrequest.get_current_classCode());
                Color_Print(color_mode, "cyan", " in UC ");
                Color_Print(color_mode, "yellow", newrequest.get_current_UcCode(), true);
                processed_requests.pop();
            }
            if (get_student_from_studentCode(newrequest.get_studentCode())->get_classes().empty()) {
                cout << "Student " << newrequest.get_studentCode() << " no longer has classes, do you want to delete him? [Y/N] \n";
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    Color_Print(color_mode, "red", "Invalid Input, please try again", true);
                    cin >> answer;
                }
                if (answer == "Y") {
                    Request thisrequest = Request("DELETE", newrequest.get_studentCode(), newrequest.get_studentName());
                    request_delete(thisrequest);
                }
            }
            return;
        }
        case 'R': {
            Request newrequest = Request("ADD", thisrequest.get_studentCode(), thisrequest.get_studentName(), "", "", thisrequest.get_current_UcCode(), thisrequest.get_current_classCode());
            if (request_add(newrequest)) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " is now in class ");
                Color_Print(color_mode, "yellow", newrequest.get_new_classCode());
                Color_Print(color_mode, "cyan", " in UC ");
                Color_Print(color_mode, "yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return;
        }
        case 'S': {
            Request newrequest = Request("SWITCH", thisrequest.get_studentCode(), thisrequest.get_studentName(), thisrequest.get_new_UcCode(), thisrequest.get_new_classCode(), thisrequest.get_current_UcCode(), thisrequest.get_current_classCode());
            if (request_switch(newrequest)) {
                Color_Print(color_mode, "cyan", "Student ");
                Color_Print(color_mode, "yellow", newrequest.get_studentCode());
                Color_Print(color_mode, "cyan", " was removed from class ");
                Color_Print(color_mode, "yellow", newrequest.get_current_classCode());
                Color_Print(color_mode, "cyan", " in UC ");
                Color_Print(color_mode, "yellow", newrequest.get_current_UcCode());
                Color_Print(color_mode, "cyan", " and is now in class ");
                Color_Print(color_mode, "yellow", newrequest.get_new_classCode());
                Color_Print(color_mode, "cyan", " in UC ");
                Color_Print(color_mode, "yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return;
        }
        case 'N': {
            thisrequest.set_type("DELETE");
            request_delete(thisrequest);
            processed_requests.pop();
            Color_Print(color_mode, "cyan", "Student with the student code ");
            Color_Print(color_mode, "yellow", thisrequest.get_studentCode());
            Color_Print(color_mode, "cyan", " was removed.", true);
            return;
        }
        case 'D': {
            thisrequest.set_type("NEW");
            request_new(thisrequest);
            processed_requests.pop();
            Color_Print(color_mode, "cyan", "Student with the student code ");
            Color_Print(color_mode, "yellow", thisrequest.get_studentCode());
            Color_Print(color_mode, "cyan", " was added.", true);
            return;
        }
    }
}

void LEIC::process_requests() {
    while (!requests.empty()) {
        Request request = requests.front();
        requests.pop();
        switch (request.get_type()[0]) {
            case 'A': {
                if (request_add(request)) {
                    Color_Print(color_mode, "cyan", "Student ");
                    Color_Print(color_mode, "yellow", request.get_studentCode());
                    Color_Print(color_mode, "cyan", " is now in class ");
                    Color_Print(color_mode, "yellow", request.get_new_classCode());
                    Color_Print(color_mode, "cyan", " in UC ");
                    Color_Print(color_mode, "yellow", request.get_new_UcCode(), true);
                }
                break;
            }
            case 'R': {
                if (request_remove(request)){
                    Color_Print(color_mode, "cyan", "Student ");
                    Color_Print(color_mode, "yellow", request.get_studentCode());
                    Color_Print(color_mode, "cyan", " was removed from class ");
                    Color_Print(color_mode, "yellow", request.get_current_classCode());
                    Color_Print(color_mode, "cyan", " in UC ");
                    Color_Print(color_mode, "yellow", request.get_current_UcCode(), true);
                }
                if (get_student_from_studentCode(request.get_studentCode())->get_classes().empty()) {
                    cout << "Student " << request.get_studentCode() << " no longer has classes, do you want to delete him? [Y/N] \n";
                    string answer;
                    cin >> answer;
                    while(answer != "Y" && answer != "N") {
                        Color_Print(color_mode, "red", "Invalid Input, please try again", true);
                        cin >> answer;
                    }
                    if (answer == "Y") {
                        Request thisrequest = Request("DELETE", request.get_studentCode(), request.get_studentName());
                        request_delete(thisrequest);
                    }
                }
                break;
            }
            case 'S': {
                if (request_switch(request)) {
                    Color_Print(color_mode, "cyan", "Student ");
                    Color_Print(color_mode, "yellow", request.get_studentCode());
                    Color_Print(color_mode, "cyan", " was removed from class ");
                    Color_Print(color_mode, "yellow", request.get_current_classCode());
                    Color_Print(color_mode, "cyan", " in UC ");
                    Color_Print(color_mode, "yellow", request.get_current_UcCode());
                    Color_Print(color_mode, "cyan", " and is now in class ");
                    Color_Print(color_mode, "yellow", request.get_new_classCode());
                    Color_Print(color_mode, "cyan", " in UC ");
                    Color_Print(color_mode, "yellow", request.get_new_UcCode(), true);
                }
                break;
            }
            case 'N': {
                if(request_new(request))
                    cout << "Student with code " << request.get_studentCode() << " was added.\n";
                break;
            }
            case 'D': {
                if (request_delete(request))
                    cout << "Student with code " << request.get_studentCode() << " was removed.\n";
                break;
            }
        }
    }
}

void LEIC::save_to_files() {
    ofstream students_classesSaveFile("../students_classes_save.csv", ofstream::out | ofstream::trunc);
    ofstream accepted_requests("../accepted_requests.csv", ofstream::out | ofstream::trunc);
    students_classesSaveFile << "StudentCode,StudentName,UcCode,ClassCode" << endl;
    accepted_requests << "Type,StudentCode,StudentName,oldUcCode,newUcCode,oldClassCode,newClassCode" << endl;
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
                          << r.get_studentName() << ','
                          << r.get_current_UcCode() << ','
                          << r.get_current_classCode() << ','
                          << r.get_new_UcCode() << ','
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

string LEIC::studentCode_last_request() {
    if (requests.empty()) return "";
    else return requests.back().get_studentCode();
}


