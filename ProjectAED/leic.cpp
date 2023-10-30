#include "leic.h"
using namespace std;

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
        string thisclass = classcode + " " + uccode;
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
        Class c = Class(classcode,uccode);
        auto it2 = find(classes.begin(), classes.end(), c);
        up_students.at(up).add_class(&(*it2));
        it2->add_student(up);
    }
    studentsFile.close();
    if(save_file){
        ifstream requestsFile("../accepted_requests.csv");
        getline(requestsFile, line);
        string StudentCode,Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
        stack<Request> reverseOrderRequest;
        while (getline(requestsFile, line)) {     // read all lines from the given file
            istringstream iss(line);
            getline(iss, StudentCode, ',');
            getline(iss, Type, ',');
            getline(iss, oldUcCode, ',');
            getline(iss, newUcCode, ',');
            getline(iss, oldClassCode, ',');
            iss >> newClassCode;
            reverseOrderRequest.emplace(Type,!newClassCode.empty(),StudentCode,oldClassCode,newClassCode,oldUcCode,newUcCode);
        }
        while(!reverseOrderRequest.empty()){
            processed_requests.push(reverseOrderRequest.top());
            reverseOrderRequest.pop();
        }
        requestsFile.close();
    }
}

Student* LEIC::get_student_from_up(std::string up) {
    return (up_students.find(up) == up_students.end()) ? nullptr : &up_students.at(up);
}

vector<Class> LEIC::get_classes() const {
    return classes;
}

std::set<std::string> LEIC::get_ucs() const {
    return ucs;
}

std::set<std::string> LEIC::get_classcodes() const {
    set<string> classcodes;
    for(Class c: classes) classcodes.insert(c.get_classCode());
    return classcodes;
}

Class* LEIC::get_class_from_classcode_and_uccode(std::string classcode, std::string uccode) {
    if(!exists_class(uccode,classcode)) return nullptr;
    auto first_itr = lower_bound(classes.begin(),classes.end(),Class(classcode,uccode));
    return first_itr != classes.end() ? &(*first_itr) : nullptr;
}

vector<Class*> LEIC::get_classes_from_uccode(string ucCode) {
    vector<Class*> classes_ucCode;
    if(!exists_uc(ucCode)) return classes_ucCode;
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

bool LEIC::exists_uc(std::string ucCode) {
    return ucs.count(ucCode);
}

bool LEIC::uc_has_vacancy(std::string uccode) {
    for (Class& c: classes) {
        if (c.get_ucCode() == uccode){
            if (c.get_students().size() < CAP) return true;
        }
    }
    return false;
}

void LEIC::list_students_by_up() {
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(pair<string, Student> p : up_students){
        cout << p.first << " | " << p.second.get_name() << '\n';
    }
}

void LEIC::list_students_by_name(){
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    map<string, string> students_up;
    for(pair<string, Student> p : up_students){
        students_up[p.second.get_name()] = p.first;
    }
    for(pair<string, string> p : students_up){
        cout << p.second << '\t' << p.first << '\n';
    }
}

void LEIC::list_uc_students_by_up(string uc) {
    cout << "Students of UC " << uc << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(Class c : classes){
        if(c.get_ucCode() == uc){
            for(string up : c.get_students())
                cout << up << " | " << up_students.at(up).get_name() << endl;
        }
    }
}

void LEIC::list_uc_students_by_name(std::string uc) {
    cout << "Students of UC " << uc << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    map<string, string> UCstudents_up;
    for(Class c : classes){
        if(c.get_ucCode() == uc) {
            for (string up: c.get_students()) UCstudents_up[up_students.at(up).get_name()] = up;
        }
    }
    for (pair<string, string> p: UCstudents_up) cout << p.second << '\t' << p.first << '\n';
}

void LEIC::list_class_students_by_up(Class* class_) const {
    cout << "Students of Class " << class_->get_classCode() << " of UC " << class_->get_ucCode() << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(string up : class_->get_students()) cout << up << " | " << up_students.at(up).get_name() << endl;
}

void LEIC::list_class_students_by_name(Class *class_) const {
    cout << "Students of Class " << class_->get_classCode() << " of UC " << class_->get_ucCode() << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    map<string, string> UCstudents_up;
    for (string up: class_->get_students()) UCstudents_up[up_students.at(up).get_name()] = up;
    for (pair<string, string> p: UCstudents_up) cout << p.second << '\t' << p.first << '\n';
}

void LEIC::list_number_students_class() {
    for (Class c: classes) {
        int currentSize = c.get_students().size();
        cout << c.get_classCode() << " " << c.get_ucCode() << " " << currentSize << endl;
    }
}

int LEIC::students_in_n_ucs(int n){
    int count = 0;
    for (pair<string, Student> p: up_students) {
        count += p.second.get_classes().size() >= n;
    }
    return count;
}

bool LEIC::class_balance_valid(Class newClass) {
    int max = newClass.get_students().size() + 1;
    int min = newClass.get_students().size();
    for (Class c: classes) {
        if (c.get_ucCode() == newClass.get_ucCode()){
            if (c.get_students().size() < min) min = c.get_students().size();
            if (c.get_students().size() > max) max = c.get_students().size();
        }
    }
    return max-min <= 4;
}

bool LEIC::compatible_schedules(Student student, Class* newclass, Class* oldclass) {
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
    up_students.insert({student.get_student_up(), student});
}

void LEIC::add_student_to_class(Student* student, Class *newclass) {
    newclass->add_student(student->get_student_up());
    student->add_class(newclass);
}

void LEIC::remove_student_from_class(Student* student, Class *newclass) {
    newclass->remove_student(student->get_student_up());
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
    string StudentCode,Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
    while (getline(requestsFile, line)) {     // read all lines from the given file
        istringstream iss(line);
        getline(iss, StudentCode, ',');
        getline(iss, Type, ',');
        getline(iss, oldUcCode, ',');
        getline(iss, newUcCode, ',');
        getline(iss, oldClassCode, ',');
        iss >> newClassCode;
        requests.emplace(Type,!newClassCode.empty(),StudentCode,oldClassCode,newClassCode,oldUcCode,newUcCode);
    }
    requestsFile.close();
    process_requests();
}

bool LEIC::request_add(Request& request) {
    Student* student = get_student_from_up(request.get_student_up());
    if (request.get_uc_class()) {
        Class* newclass = get_class_from_classcode_and_uccode(request.get_new_class(), request.get_new_uc());
        if (newclass->get_students().size() < CAP
            && !student->has_uc(request.get_new_uc()) && compatible_schedules(*student, newclass)) {
                add_student_to_class(student, newclass);
                processed_requests.push(request);
                return true;
        }
        return false;
    }
    else {
        if (uc_has_vacancy(request.get_new_uc()) && student->get_classes().size()<7) {
            vector<Class*> classes_uccode = get_classes_from_uccode(request.get_new_uc());
            for (Class* c: classes_uccode) {
                if (compatible_schedules(*student, c)) {
                    add_student_to_class(student, c);
                    processed_requests.push(request);
                    request.set_new_class(c->get_classCode());
                    return true;
                }
            }
        }
    }
    return false;
}

bool LEIC::request_remove(Request request) {
    Student* student = get_student_from_up(request.get_student_up());
    Class* currentClass = student->get_class_from_uc(request.get_current_uc());
    string currentclass = currentClass->get_classCode();
    string currentUc = request.get_current_uc();
    remove_student_from_class(student, currentClass);
    processed_requests.push(request);
    return true;
}

bool LEIC::request_switch(Request request) {
    request.set_type("REMOVE");
    request_remove(request);
    request.set_type("ADD");
    if (request_add(request)) {
        processed_requests.pop();
        processed_requests.pop();
        request.set_type("SWITCH");
        processed_requests.push(request);
        return true;
    }
    else {
        undo_request();
    }
    return false;
}

bool LEIC::undo_request() {
    if(processed_requests.empty()) return false;
    Request thisrequest = processed_requests.top();
    processed_requests.pop();
    bool res = false;
    switch (thisrequest.get_type()[0]) {
        case 'A': {
            Request newrequest = Request("REMOVE", false, thisrequest.get_student_up(), "", "", thisrequest.get_new_uc(), "");
            res = request_remove(newrequest);
            if (res) processed_requests.pop();
            return res;
        }
        case 'R': {
            Request newrequest = Request("ADD", false, thisrequest.get_student_up(), "", "",  "", thisrequest.get_new_uc());
            res = request_add(newrequest);
            if (res) processed_requests.pop();
            return res;
        }
        case 'S': {
            Request newrequest = Request("SWITCH", thisrequest.get_uc_class(), thisrequest.get_student_up(), thisrequest.get_new_class(), thisrequest.get_current_class(),  thisrequest.get_new_uc(), thisrequest.get_current_uc());
            res = request_switch(newrequest);
            if (res) processed_requests.pop();
            return res;
        }
        case 'N': {
            up_students.erase(thisrequest.get_student_up());
            return true;
        }
    }
    return res;
}

void LEIC::process_requests() {
    while (!requests.empty()) {
        Request request = requests.front();
        requests.pop();
        switch (request.get_type()[0]) {
            case 'A': {
                if (request_add(request)) {
                    cout << "Student is now in the class " << request.get_new_class() << " in the UC " << request.get_new_uc() << endl;
                }
                else cout << "The request was denied.\n";
                break;
            }
            case 'R': {
                if (request_remove(request)) cout << "Student was removed from class " << request.get_current_class() << " in the UC " << request.get_current_uc() << endl;
                else cout << "The request was denied.";
                break;
            }
            case 'S': {
                if (request_switch(request)) {
                    cout << "Student was removed from class " << request.get_current_class() << " in the UC " << request.get_current_uc() << " and is now in the class" << request.get_new_class() << " in the UC " << request.get_new_uc() << endl;
                }
                break;
            }
        }
    }

}

void LEIC::save_to_files() {
    ofstream students_classesSaveFile("../students_classes_save.csv", ofstream::out | ofstream::trunc);
    ofstream accepted_requests("../accepted_requests.csv", ofstream::out | ofstream::trunc);
    students_classesSaveFile << "StudentCode,StudentName,UcCode,ClassCode" << endl;
    accepted_requests << "StudentCode,Type,oldUcCode,newUcCode,oldClassCode,newClassCode" << endl;
    for (pair<string, Student> up_s: up_students) {
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
        accepted_requests << r.get_student_up() << ','
                          << r.get_type() << ','
                          << r.get_current_uc() << ','
                          << r.get_new_uc() << ','
                          << r.get_current_class() << ','
                          << r.get_new_class() << endl;
    }
    students_classesSaveFile.close();
    accepted_requests.close();
}