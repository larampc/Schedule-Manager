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
            getline(iss, StudentCode, ',');
            getline(iss, Type, ',');
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

void LEIC::list_UC_students_by_studentCode(std::string UcCode) {
    cout << "Students of UC " << UcCode << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(Class c : classes){
        if(c.get_ucCode() == UcCode){
            for(string up : c.get_students())
                cout << up << " | " << up_students.at(up).get_name() << endl;
        }
    }
}

void LEIC::list_UC_students_by_name(std::string uc) {
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

void LEIC::list_class_students_by_studentCode(Class *class_) const {
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

int LEIC::students_in_n_Ucs(int n){
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
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (request.get_Uc_class()) {
        Class* newclass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_new_UcCode());
        if (newclass->get_students().size() < CAP
            && !student->has_uc(request.get_new_UcCode()) && compatible_schedules(*student, newclass)) {
                add_student_to_class(student, newclass);
                processed_requests.push(request);
                return true;
        }
        return false;
    }
    else {
        if (Uc_has_vacancy(request.get_new_UcCode()) && student->get_classes().size()<7) {
            vector<Class*> classes_uccode = get_classes_from_UcCode(request.get_new_UcCode());
            for (Class* c: classes_uccode) {
                if (compatible_schedules(*student, c)) {
                    add_student_to_class(student, c);
                    request.set_new_class(c->get_classCode());
                    processed_requests.push(request);
                    return true;
                }
            }
        }
    }
    return false;
}

bool LEIC::request_remove(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    Class* currentClass = student->get_class_from_uc(request.get_current_UcCode());
    string currentclass = currentClass->get_classCode();
    string currentUc = request.get_current_UcCode();
    request.set_current_class(currentclass);
    remove_student_from_class(student, currentClass);
    processed_requests.push(request);
    return true; // vamos ter que ver consoante class balance?
}

bool LEIC::request_switch(Request& request) {
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
    if(processed_requests.empty()) {
        cout << "There are no requests to undo\n";
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
                cout << "Student " << newrequest.get_studentCode() << " was removed from class " << newrequest.get_current_classCode() << " in the UC " << newrequest.get_current_UcCode() << endl;
                processed_requests.pop();
            }
            return valid;
        }
        case 'R': {
            Request newrequest = Request("ADD", false, thisrequest.get_studentCode(), "", "",  "", thisrequest.get_new_UcCode());
            valid = request_add(newrequest);
            if (valid) {
                cout << "Student " << newrequest.get_studentCode() << " is now in the class " << newrequest.get_new_classCode() << " in the UC " << newrequest.get_new_UcCode() << endl;
                processed_requests.pop();
            }
            return valid;
        }
        case 'S': {
            Request newrequest = Request("SWITCH", true, thisrequest.get_studentCode(),thisrequest.get_new_classCode(), thisrequest.get_current_classCode(), thisrequest.get_new_UcCode(), thisrequest.get_current_UcCode());
            valid = request_switch(newrequest);
            if (valid) {
                cout << "Student " << newrequest.get_studentCode() << " was removed from class " << newrequest.get_current_classCode() << " in the UC " << newrequest.get_current_UcCode() << " and is now in the class " << newrequest.get_new_classCode() << " in the UC " << newrequest.get_new_UcCode() << endl;
                processed_requests.pop();
            }
            return valid;
        }
        case 'N': {
            cout << "Student with the student code " << thisrequest.get_studentCode() << " was removed.";
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
                if (request_add(request)) {
                    cout << "Student is now in the class " << request.get_new_classCode() << " in the UC " << request.get_new_UcCode() << endl;
                }
                else cout << "The request was denied.\n";
                break;
            }
            case 'R': {
                if (request_remove(request)) cout << "Student was removed from class " << request.get_current_classCode() << " in the UC " << request.get_current_UcCode() << endl;
                else cout << "The request was denied.";
                break;
            }
            case 'S': {
                if (request_switch(request)) {
                    cout << "Student was removed from class " << request.get_current_classCode() << " in the UC " << request.get_current_UcCode() << " and is now in the class" << request.get_new_classCode() << " in the UC " << request.get_new_UcCode() << endl;
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
        accepted_requests << r.get_studentCode() << ','
                          << r.get_type() << ','
                          << r.get_current_UcCode() << ','
                          << r.get_new_UcCode() << ','
                          << r.get_current_classCode() << ','
                          << r.get_new_classCode() << endl;
    }
    students_classesSaveFile.close();
    accepted_requests.close();
}