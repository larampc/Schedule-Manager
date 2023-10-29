#include "leic.h"
using namespace std;

LEIC::LEIC(std::string filenameclasses, std::string filenamestudents) {
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
}

Student* LEIC::get_student_from_up(std::string up) {
    if(up_students.find(up) == up_students.end()) return nullptr;
    return &up_students.at(up);
}

std::set<std::string> LEIC::get_ucs() const {
    return ucs;
}

vector<Class> LEIC::get_classes() const {
    return classes;
}

void LEIC::listStudentsByUP() {
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(pair<string, Student> p : up_students){
        cout << p.first << " | " << p.second.get_name() << '\n';
    }
}
void LEIC::listStudentsByName(){
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    map<string, string> students_up;
    for(pair<string, Student> p : up_students){
        students_up[p.second.get_name()] = p.first;
    }
    for(pair<string, string> p : students_up){
        cout << p.second << '\t' << p.first << '\n';
    }
}

void LEIC::listUCStudentsByUP(string uc) {
    cout << "Students of UC " << uc << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(Class c : classes){
        if(c.get_ucCode() == uc){
            for(string up : c.get_students())
                cout << up << " | " << up_students.at(up).get_name() << endl;
        }
    }
}

void LEIC::listUCStudentsByName(std::string uc) {
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

void LEIC::list_class_students_by_UP(Class* class_) const {
    cout << "Students of Class " << class_->get_classCode() << " of UC " << class_->get_ucCode() << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    for(string up : class_->get_students()) cout << up << " | " << up_students.at(up).get_name() << endl;
}

void LEIC::list_class_students_by_Name(Class *class_) const {
    cout << "Students of Class " << class_->get_classCode() << " of UC " << class_->get_ucCode() << endl;
    cout << "UPNUMBER\tNAME\n------------------------------------\n";
    map<string, string> UCstudents_up;
    for (string up: class_->get_students()) UCstudents_up[up_students.at(up).get_name()] = up;
    for (pair<string, string> p: UCstudents_up) cout << p.second << '\t' << p.first << '\n';
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

Class* LEIC::get_class_from_classcode_and_uccode(std::string classcode, std::string uccode) {
    for (Class& c: classes) {
        if (c.get_classCode() == classcode && c.get_ucCode() == uccode) return &c;
    }
    return nullptr;
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

void LEIC::numberstudents_class() {
    for (Class c: classes) {
        int currentSize = c.get_students().size();
        cout << c.get_classCode() << " " << c.get_ucCode() << " " << currentSize << endl;
    }
}

void LEIC::add_student_to_class(Student* student, Class *newclass) {
    newclass->add_student(student->get_student_up());
    student->add_class(newclass);
}

void LEIC::remove_student_from_class(Student* student, Class *newclass) {
    newclass->remove_student(student->get_student_up());
    student->remove_class_from_uc(newclass->get_ucCode());
}

bool LEIC::uc_has_vacancy(std::string uccode) {
    for (Class& c: classes) {
        if (c.get_ucCode() == uccode){
            if (c.get_students().size() < CAP) return true;
        }
    }
    return false;
}

bool LEIC::add_request(Request request, Student* student) {
    if (request.get_uc_class()) {
        Class* newclass = get_class_from_classcode_and_uccode(request.get_new_class(), request.get_new_uc());
        if (newclass->get_students().size()<CAP
            && !student->has_uc(request.get_new_uc()) && compatible_schedules(*student, newclass)) {
            add_student_to_class(student, newclass);
            processed_requests.push(request);
            cout << "Student is now in the class " << newclass->get_classCode() << " in the UC " << newclass->get_ucCode() << endl;
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
                    cout << "Student is now in the class " << c->get_classCode() << " in the UC " << c->get_ucCode() << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

bool LEIC::remove_request(Request request, Student *student) {
    Class* currentClass = get_class_from_classcode_and_uccode(student->get_class_from_uc(request.get_current_uc()), request.get_current_uc());
    string currentclass = currentClass->get_classCode();
    string currentUc = request.get_current_uc();
    remove_student_from_class(student, currentClass);
    cout << "Student was removed from class " << currentclass << " in the UC " << currentUc << endl;
    processed_requests.push(request);
    return true;
}

bool LEIC::switch_request(Request request, Student *student) {
    request.set_type("REMOVE");
    remove_request(request, student);
    request.set_type("ADD");
    if (add_request(request, student)) {
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

bool LEIC::process_requests(Request request) {
    Student* student = get_student_from_up(request.get_student_up());
    switch (request.get_type()[0]) {
        case 'A': {
            return add_request(request, student);
        }
        case 'R': {
            return remove_request(request, student);
        }
        case 'S': {
            return switch_request(request, student);
        }
    }
    return false;
}

vector<Class*> LEIC::get_classes_from_uccode(string uccode) {
    vector<Class*> classes_uccode;
    for (Class& c: classes) {
        if (c.get_ucCode() == uccode) classes_uccode.push_back(&c);
    }
    return classes_uccode;
}

bool LEIC::undo_request() {
    Request thisrequest = processed_requests.top();
    processed_requests.pop();
    switch (thisrequest.get_type()[0]) {
        case 'A': {
            Request newrequest = Request("REMOVE", false, thisrequest.get_student_up(), "", "", thisrequest.get_new_uc(), "");
            return process_requests(newrequest);
        }
        case 'R': {
            Request newrequest = Request("ADD", false, thisrequest.get_student_up(), "", "",  "", thisrequest.get_new_uc());
            return process_requests(newrequest);
        }
        case 'S': {
            Request newrequest = Request("SWITCH", thisrequest.get_uc_class(), thisrequest.get_student_up(), thisrequest.get_new_class(), thisrequest.get_current_class(),  thisrequest.get_new_uc(), thisrequest.get_current_uc());
            return process_requests(newrequest);
        }
    }
    return false;
}

std::set<std::string> LEIC::get_classcodes() const {
    set<string> classcodes;
    for(Class c: classes) classcodes.insert(c.get_classCode());
    return classcodes;
}

void LEIC::set_cap(int cap) {
    CAP = cap;
}

int LEIC::get_cap() {
    return CAP;
}



