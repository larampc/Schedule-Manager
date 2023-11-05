#include <limits>
#include "leic.h"
using namespace std;

bool is_number2(string s) {
    return all_of(s.begin(),s.end(),  [] (char c){return isdigit(c);});
}

LEIC::LEIC(std::string filenameclasses, std::string filenamestudents, bool save_file) {
    ifstream classesFile(filenameclasses);
    string line;
    getline(classesFile, line);
    while (getline(classesFile, line)) {
        string classcode, uccode, weekday, starthour, duration, type;
        istringstream iss(line);
        getline(iss, classcode, ',');
        getline(iss, uccode, ',');
        getline(iss, weekday, ',');
        getline(iss, starthour, ',');
        getline(iss, duration, ',');
        iss >> type;
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
    if (save_file) {
        studentsFile.ignore(5);
        string cap;
        getline(studentsFile, cap);
        if (cap.empty()) {
            cout << "No current dataset to import.";
            exit(0);
        }
        CAP = stoi(cap);
    }
    getline(studentsFile, line);
    while (getline(studentsFile, line)) {
        string up, name, ucCode, classCode;
        istringstream iss(line);
        getline(iss, up, ',');
        getline(iss, name, ',');
        getline(iss, ucCode, ',');
        iss >> classCode;
        Student s = Student(name, up);
        auto it = code_students.find(up);
        if(it == code_students.end()){
            code_students.insert({up,s});
        }
        if (!ucCode.empty()) {
            Class c = Class(classCode, ucCode);
            auto it2 = find(classes.begin(), classes.end(), c);
            code_students.at(up).add_class(&(*it2));
            it2->add_student(up);
        }
    }
    studentsFile.close();
    if(save_file){
        ifstream requestsFile("../accepted_requests.csv");
        getline(requestsFile, line);
        stack<Request> reverseOrderRequest;
        while (getline(requestsFile, line)) {
            string StudentCode, studentName, Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
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

        ifstream pending_requestsFile("../pending_requests.csv");
        getline(pending_requestsFile, line);
        while (getline(pending_requestsFile, line)) {
            string StudentCode, studentName, Type,oldUcCode,newUcCode,oldClassCode,newClassCode;
            istringstream iss(line);
            getline(iss, Type, ',');
            getline(iss, StudentCode, ',');
            getline(iss, studentName, ',');
            getline(iss, oldUcCode, ',');
            getline(iss, oldClassCode, ',');
            getline(iss, newUcCode, ',');
            iss >> newClassCode;
            pending_requests.emplace(Type, StudentCode, studentName, oldUcCode,oldClassCode,newUcCode,newClassCode);
        }
        requestsFile.close();
    }
}

Student* LEIC::get_student_from_studentCode(std::string studentCode) {
    return (code_students.find(studentCode) == code_students.end()) ? nullptr : &code_students.at(studentCode);
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

string LEIC::get_studentCode_last_request() const {
    if (pending_requests.empty()) return "";
    return pending_requests.back().get_studentCode();
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
    vector<Class*> classes_uc = get_classes_from_UcCode(uccode);
    for (Class* c: classes_uc) {
        if (c->get_students().size() < CAP) return true;
    }
    return false;
}

void LEIC::list_students_by_studentCode() {
    Color_Print("blue", "Students of Course ");
    Color_Print("yellow", "LEIC", true);
    Color_Print("blue", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("white", "NAME", true);
    Color_Print("green", "-----------------------", true);
    for(pair<string, Student> p : code_students){
        Color_Print("blue", p.first);
        Color_Print("green", "\t| ");
        Color_Print("white", p.second.get_name(), true);
    }
}

void LEIC::list_students_by_name(){
    Color_Print("blue", "Students of Course ");
    Color_Print("yellow", "LEIC", true);
    Color_Print("white", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("blue", "NAME", true);
    Color_Print("green", "-----------------------", true);
    map<string, string> students_up;
    for(pair<string, Student> p : code_students){
        students_up[p.second.get_name()] = p.first;
    }
    for(pair<string, string> p : students_up){
        Color_Print("white", p.second);
        Color_Print("green", "\t| ");
        Color_Print("blue", p.first, true);
    }
}

void LEIC::list_UC_students_by_studentCode(std::string UcCode) {
    Color_Print("blue", "Students of UC ");
    Color_Print("yellow", UcCode, true);
    Color_Print("blue", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("white", "NAME", true);
    Color_Print("green", "-----------------------", true);
    set<string> studentinUC;
    vector<Class*> classes_uc = get_classes_from_UcCode(UcCode);
    for(Class* c : classes_uc){
        set<string> studentUC = c->get_students();
        studentinUC.insert(studentUC.begin(), studentUC.end());
    }
    for(string code : studentinUC){
        Color_Print("blue", code);
        Color_Print("green", "\t| ");
        Color_Print("white", code_students.at(code).get_name(), true);
    }
}

void LEIC::list_UC_students_by_name(std::string ucCode) {
    Color_Print("blue", "Students of UC ");
    Color_Print("yellow", ucCode, true);
    Color_Print("white", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("blue", "NAME", true);
    Color_Print("green", "-----------------------", true);
    map<string, string> UCstudents_up;
    vector<Class*> classes_uc = get_classes_from_UcCode(ucCode);
    for(Class* c : classes_uc){
        for (string up: c->get_students()) UCstudents_up[code_students.at(up).get_name()] = up;
    }
    for (pair<string, string> p: UCstudents_up){
        Color_Print("white", p.second);
        Color_Print("green", "\t| ");
        Color_Print("blue", p.first, true);
    }
}

void LEIC::list_class_students_by_studentCode(Class *class_) const {
    Color_Print("blue", "Students of Class ");
    Color_Print("yellow", class_->get_classCode());
    Color_Print("blue", " of UC ");
    Color_Print("yellow", class_->get_ucCode(), true);
    Color_Print("blue", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("white", "NAME", true);
    Color_Print("green", "-----------------------", true);
    for(string up : class_->get_students()){
        Color_Print("blue", up);
        Color_Print("green", "\t| ");
        Color_Print("white", code_students.at(up).get_name(), true);
    }
}

void LEIC::list_class_students_by_name(Class *class_) const {
    Color_Print("blue", "Students of Class ");
    Color_Print("yellow", class_->get_classCode());
    Color_Print("blue", " of UC ");
    Color_Print("yellow", class_->get_ucCode(), true);
    Color_Print("white", "StudentCode");
    Color_Print("green", "\t| ");
    Color_Print("blue", "NAME", true);
    Color_Print("green", "-----------------------", true);
    map<string, string> UCstudents_up;
    for (string up: class_->get_students()) UCstudents_up[code_students.at(up).get_name()] = up;
    for (pair<string, string> p: UCstudents_up){
        Color_Print("white", p.second);
        Color_Print("green", "\t| ");
        Color_Print("blue", p.first, true);
    }
}

void LEIC::list_Uc_occupations_by_classCode(std::string UcCode, bool order) {
    vector<Class*> UcClasses = get_classes_from_UcCode(UcCode);
    Color_Print("blue", "Occupations of UC ");
    Color_Print("yellow", UcCode, true);
    Color_Print("blue", "Class code\t");
    Color_Print("green", "| ");
    Color_Print("white", "Occupation", true);
    Color_Print("green", "-------------------------", true);
    if (order) {
        for (Class* c: UcClasses) {
            Color_Print("blue", c->get_classCode() + "    \t");
            Color_Print("green", "| ");
            Color_Print("white", to_string(c->get_students().size()), true);
        }
        return;
    }
    auto itr = UcClasses.end();
    while (itr-- != UcClasses.begin()) {
        Color_Print("blue", (*itr)->get_classCode() + "    \t");
        Color_Print("green", "| ");
        Color_Print("white", to_string((*itr)->get_students().size()), true);
    }
}

void LEIC::list_Uc_occupations_by_occupation(std::string UcCode, bool order) {
    vector<Class> UcClasses;
    for (Class c: classes) {
        if (c.get_ucCode() == UcCode) UcClasses.push_back(c);
    }
    Color_Print("blue", "Occupations of UC ");
    Color_Print("yellow", UcCode, true);
    Color_Print("white", "Class code\t");
    Color_Print("green", "| ");
    Color_Print("blue", "Occupation", true);
    Color_Print("green", "-------------------------", true);
    (order) ? sort(UcClasses.begin(),UcClasses.end(), [] (Class c1,Class c2) -> bool {
        return (c1.get_students().size() < c2.get_students().size())
               || (c1.get_students().size() == c2.get_students().size() && c1.get_classCode() < c2.get_classCode())  ;})
            : sort(UcClasses.rbegin(),UcClasses.rend(), [] (Class c1,Class c2) -> bool {
        return (c1.get_students().size() < c2.get_students().size())
               || (c1.get_students().size() == c2.get_students().size() && c1.get_classCode() < c2.get_classCode());});
    for (Class c: UcClasses) {
        Color_Print("white", c.get_classCode() + "    \t");
        Color_Print("green", "| ");
        Color_Print("blue", to_string(c.get_students().size()), true);
    }
}

void LEIC::list_class_occupations_by_occupation(std::string classCode, bool order) {
    vector<Class> classClasses;
    for (Class c: classes) {
        if (c.get_classCode() == classCode) classClasses.push_back(c);
    }
    Color_Print("blue", "Occupations of Class ");
    Color_Print("yellow", classCode, true);
    Color_Print("white", "UC code    \t");
    Color_Print("green", "| ");
    Color_Print("blue", "Occupation", true);
    Color_Print("green", "---------------------------------", true);

    (order) ? sort(classClasses.begin(), classClasses.end(), [](Class c1, Class c2) -> bool {
        return (c1.get_students().size() < c2.get_students().size())
               || (c1.get_students().size() == c2.get_students().size() && c1.get_ucCode() < c2.get_ucCode());
    })
            : sort(classClasses.rbegin(), classClasses.rend(), [](Class c1, Class c2) -> bool {
        return (c1.get_students().size() < c2.get_students().size())
               || (c1.get_students().size() == c2.get_students().size() && c1.get_ucCode() < c2.get_ucCode());
    });
    for (Class c: classClasses) {
        Color_Print("white", c.get_ucCode() + "    \t");
        Color_Print("green", "| ");
        Color_Print("blue", to_string(c.get_students().size()), true);
    }
}

void LEIC::list_class_occupations_by_UC(std::string classCode, bool order) {
    vector<Class> classClasses;
    for (Class c: classes) {
        if (c.get_classCode() == classCode) classClasses.push_back(c);
    }
    Color_Print("blue", "Occupations of Class ");
    Color_Print("yellow", classCode, true);
    Color_Print("blue", "UC code    \t");
    Color_Print("green", "| ");
    Color_Print("white", "Occupation", true);
    Color_Print("green", "---------------------------------", true);
    if(order){
        for (Class c: classClasses) {
            Color_Print("blue", c.get_ucCode() + "    \t");
            Color_Print("green", "| ");
            Color_Print("white", to_string(c.get_students().size()), true);
        }
        return;
    }
    auto itr = classClasses.end();
    while(itr-- != classClasses.begin()){
        Color_Print("blue", itr->get_ucCode() + "    \t");
        Color_Print("green", "| ");
        Color_Print("white", to_string(itr->get_students().size()), true);
    }
}

void LEIC::list_year_occupations_by_occupation(string year, bool order) {
    vector<Class> yearClasses;
    for (Class c: classes) {
        if (c.get_classCode()[0] == year[0]) yearClasses.push_back(c);
    }
    (order) ? sort(yearClasses.begin(),yearClasses.end(), [] (Class c1,Class c2) -> bool { return
            (c1.get_students().size() < c2.get_students().size()) || (c1.get_students().size() == c2.get_students().size()
                                                                      && ((c1.get_ucCode() < c2.get_ucCode()) || (c1.get_ucCode() == c2.get_ucCode() && c1.get_classCode() < c2.get_classCode())));
    } )
            : sort(yearClasses.rbegin(),yearClasses.rend(), [] (Class c1,Class c2) -> bool { return
            (c1.get_students().size() < c2.get_students().size()) || (c1.get_students().size() == c2.get_students().size()
                                                                      && ((c1.get_ucCode() < c2.get_ucCode()) || (c1.get_ucCode() == c2.get_ucCode() && c1.get_classCode() < c2.get_classCode())));
    } );
    Color_Print("blue", "Occupations of year ");
    Color_Print("yellow", year, true);
    Color_Print("white", "UC code  ");
    Color_Print("green", "| ");
    Color_Print("white", "Class code ");
    Color_Print("green", "| ");
    Color_Print("blue", "Occupation", true);
    Color_Print("green", "-----------------------------------", true);
    string out;
    stringstream outstream;
    for (Class c: yearClasses) {
        outstream << left << setw(9) << setfill(' ') << c.get_ucCode();
        getline(outstream, out);
        Color_Print("white", out);
        outstream.clear();
        outstream << left << setw(11) << setfill(' ') << c.get_classCode();
        getline(outstream, out);
        Color_Print("green", "| ");
        Color_Print("white", out);
        outstream.clear();
        Color_Print("green", "| ");
        Color_Print("blue", to_string(c.get_students().size()), true);
    }
}

void LEIC::list_year_occupations_by_classCode(string year, bool order) {
    vector<Class> yearClasses;
    for (Class c: classes) {
        if (c.get_classCode()[0] == year[0]) yearClasses.push_back(c);
    }

    (order) ? sort(yearClasses.begin(), yearClasses.end(), [](Class c1, Class c2) -> bool {
        return c1.get_classCode() < c2.get_classCode()
               || (c1.get_classCode() == c2.get_classCode() && c1.get_ucCode() < c2.get_ucCode());
    })
            : sort(yearClasses.rbegin(), yearClasses.rend(), [](Class c1, Class c2) -> bool {
        return c1.get_classCode() < c2.get_classCode()
               || (c1.get_classCode() == c2.get_classCode() && c1.get_ucCode() < c2.get_ucCode());
    });

    Color_Print("blue", "Occupations of year ");
    Color_Print("yellow", year, true);
    Color_Print("white", "UC code  ");
    Color_Print("green", "| ");
    Color_Print("blue", "Class code ");
    Color_Print("green", "| ");
    Color_Print("white", "Occupation", true);
    Color_Print("green", "-----------------------------------", true);
    string out;
    stringstream outstream;
    for (Class c: yearClasses) {
        outstream << left << setw(9) << setfill(' ') << c.get_ucCode();
        getline(outstream, out);
        Color_Print("white", out);
        outstream.clear();
        outstream << left << setw(11) << setfill(' ') << c.get_classCode();
        getline(outstream, out);
        Color_Print("green", "| ");
        Color_Print("blue", out);
        outstream.clear();
        Color_Print("green", "| ");
        Color_Print("white", to_string(c.get_students().size()), true);
    }
}

void LEIC::list_year_occupations_by_UC(string year, bool order) {
    vector<Class> yearClasses;
    for (Class c: classes) {
        if (c.get_classCode()[0] == year[0]) yearClasses.push_back(c);
    }
    Color_Print("blue", "Occupations of year ");
    Color_Print("yellow", year, true);
    Color_Print("blue", "UC code  ");
    Color_Print("green", "| ");
    Color_Print("white", "Class code ");
    Color_Print("green", "| ");
    Color_Print("white", "Occupation", true);
    Color_Print("green", "-----------------------------------", true);
    if(order) {
        string out;
        stringstream outstream;
        for (Class c: yearClasses) {
            outstream << left << setw(9) << setfill(' ') << c.get_ucCode();
            getline(outstream, out);
            Color_Print("blue", out);
            outstream.clear();
            outstream << left << setw(11) << setfill(' ') << c.get_classCode();
            getline(outstream, out);
            Color_Print("green", "| ");
            Color_Print("white", out);
            outstream.clear();
            Color_Print("green", "| ");
            Color_Print("white", to_string(c.get_students().size()), true);
        }
        return;
    }
    string out;
    stringstream outstream;
    auto itr = yearClasses.end();
    while(itr-- != yearClasses.begin()){
        outstream << left << setw(9) << setfill(' ') << itr->get_ucCode();
        getline(outstream, out);
        Color_Print("blue", out);
        outstream.clear();
        outstream << left << setw(11) << setfill(' ') << itr->get_classCode();
        getline(outstream, out);
        Color_Print("green", "| ");
        Color_Print("white", out);
        outstream.clear();
        Color_Print("green", "| ");
        Color_Print("white", to_string(itr->get_students().size()), true);
    }
}

int LEIC::students_in_n_Ucs(int n){
    Color_Print("blue", "Students with ");
    Color_Print("yellow", to_string(n));
    Color_Print("white", " UC's:", true);
    Color_Print("blue", "UC's");
    Color_Print("green", " - ");
    Color_Print("white", "StudentCode ");
    Color_Print("green", "| ");
    Color_Print("white", "Name", true);
    Color_Print("green", "------------------------------------", true);
    int count = 0;
    for (pair<string, Student> p: code_students) {
        if(p.second.get_classes().size() >= n) {
            count++;
            Color_Print("blue", " " + to_string(p.second.get_classes().size()));
            Color_Print("green", "   -  ");
            Color_Print("white", p.first);
            Color_Print("green", "  |  ");
            Color_Print("white", p.second.get_name(), true);
        }
    }
    return count;
}

bool LEIC::not_disturb_balance(Class* currentClass, Class* newClass){
    if(newClass->get_students().size() < currentClass->get_students().size()) return true;
    vector<Class*> uc_classes = get_classes_from_UcCode(newClass->get_ucCode());
    int min = currentClass->get_students().size() - 1;
    for(Class* c : uc_classes) {
        if (c->get_students().size() < min) min = c->get_students().size();
    }
    return (newClass->get_students().size() + 1 - min <= 4);
}

vector<Class*> LEIC::class_balance_valid(Student* student, Class* newClass) {
    int min = newClass->get_students().size();
    int max = min;
    vector<Class*> res;
    vector<Class*> classes_in_uc = get_classes_from_UcCode(newClass->get_ucCode());
    for (Class* c: classes_in_uc) {
        if (c->get_students().size() < min) min = c->get_students().size();
        if (c->get_students().size() > max) max = c->get_students().size();
    }
    if(newClass->get_students().size() + 1 - min <= 4) return {};
    for (Class* c: classes_in_uc) {
        if ((c->get_students().size() + 1 - min <= 4) && compatible_schedules(student, c)) res.push_back(c);
    }
    if (res.empty() && max == newClass->get_students().size()) {
        for (Class* c: classes_in_uc) {
            if ((c->get_students().size() < max) && compatible_schedules(student, c)) res.push_back(c);
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
    pending_requests.push(request);
}


bool LEIC::request_add(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (student == nullptr) {
        Color_Print("red", "Invalid request. The student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " doesn't exist.", true);
        return false;
    }
    if (student->get_classes().size() >= 7) {
        Color_Print("red", "The student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " already has 7 UC's.", true);
        return false;
    }
    if (student->has_uc(request.get_new_UcCode())) {
        Color_Print("red", "Invalid request. Student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " is already in UC ");
        Color_Print("yellow", request.get_new_UcCode());
        Color_Print("red", ".", true);
        return false;
    }
    if (!request.get_new_classCode().empty()) {
        Class* newclass = get_class_from_classCode_and_UcCode(request.get_new_classCode(), request.get_new_UcCode());
        if (newclass->get_students().size() >= CAP){
            Color_Print("red", "Request rejected. The class ");
            Color_Print("yellow", request.get_new_classCode());
            Color_Print("red", " from UC ");
            Color_Print("yellow", request.get_new_UcCode());
            Color_Print("red", " it's full.", true);
            return false;
        }
        if (!compatible_schedules(student, newclass)){
            Color_Print("red", "Request rejected. The schedule of class ");
            Color_Print("yellow", request.get_new_classCode());
            Color_Print("red", " from UC ");
            Color_Print("yellow", request.get_new_UcCode());
            Color_Print("red", " it's not compatible.", true);
            return false;
        }
        vector<Class*> suggestions = class_balance_valid(student, newclass);
        if (!suggestions.empty()) {
            Color_Print("yellow", request.get_studentCode());
            Color_Print("red", " joining class ");
            Color_Print("yellow", request.get_new_classCode());
            Color_Print("red", " in UC ");
            Color_Print("yellow", request.get_new_UcCode());
            Color_Print("red", " affects class balance.");
            Color_Print("blue", " Do you want to:", true);
            Color_Print("cyan", "1- ");
            Color_Print("red", "Cancel operation", true);
            auto it = suggestions.begin();
            int i = 2;
            for (Class* c: suggestions) {
                Color_Print("cyan", to_string(i++) + "- ");
                Color_Print("white", "Change to class " + (*it++)->get_classCode(), true);
            }
            string option;
            cin >> option;
            while(!is_number2(option) || stoi(option) >= i || stoi(option) == 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Color_Print("red","Invalid Input, please try again",true);
                cin >> option;
            }
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
            Color_Print("red", "All classes from UC ");
            Color_Print("yellow", request.get_new_UcCode());
            Color_Print("red", " are full.", true);
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
        Color_Print("red", "No compatible classes.", true);
        return false;
    }
}

bool LEIC::request_remove(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    if (student == nullptr) {
        Color_Print("red", "Invalid request. The student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " doesn't exist.", true);
        return false;
    }
    Class* currentClass = student->get_class_from_uc(request.get_current_UcCode());
    if (currentClass == nullptr) {
        Color_Print("red", "Invalid request. The student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " doesn't have UC ");
        Color_Print("yellow", request.get_current_UcCode(), true);
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
        code_students.insert({request.get_studentCode(), Student(request.get_studentName(), request.get_studentCode())});
        processed_requests.push(request);
        return true;
    }
    Color_Print("red", "The student ");
    Color_Print("yellow", request.get_studentCode());
    Color_Print("red", " already exists.", true);
    return false;
}

bool LEIC::request_delete(Request& request) {
    Student* student = get_student_from_studentCode(request.get_studentCode());
    request.set_name(student->get_name());
    if (student == nullptr) {
        Color_Print("red", "The student ");
        Color_Print("yellow", request.get_studentCode());
        Color_Print("red", " doesn't exist.", true);
        return false;
    }
    request.set_current_uc(to_string(student->get_classes().size()));
    for (Class* c: student->get_classes()) {
        Request r = Request("REMOVE", request.get_studentCode(), "", c->get_ucCode());
        request_remove(r);
    }
    code_students.erase(request.get_studentCode());
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
    if (request.get_current_UcCode() == request.get_new_UcCode()) { // mesma UC
        if (!not_disturb_balance(currentClass, newClass)){
            add_student_to_class(student,currentClass);
            processed_requests.pop();
            Color_Print("red", "Invalid request. Switching class ");
            Color_Print("yellow", request.get_current_classCode());
            Color_Print("red", " for ");
            Color_Print("yellow", request.get_new_classCode());
            Color_Print("red", " in UC ");
            Color_Print("yellow", request.get_new_UcCode());
            Color_Print("red", " violates class balance.", true);
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

bool LEIC::pending_request_is_empty() {
    return pending_requests.empty();
}

void LEIC::empty_pending_requests() {
    while (!pending_requests.empty()) {
        pending_requests.pop();
    }
}

void LEIC::print_pending_requests(){
    if(pending_requests.empty()) {
        Color_Print("cyan", "There are no pending requests", true);
        return;
    }
    for (size_t i = 0; i < pending_requests.size(); i++) {
        pending_requests.front().print_request();
        pending_requests.push(pending_requests.front());
        pending_requests.pop();
    }
}

void LEIC::undo_request() {
    if(processed_requests.empty()) {
        Color_Print("cyan", "There are no requests to undo", true);
        return;
    }
    Request thisrequest = processed_requests.top();
    processed_requests.pop();
    switch (thisrequest.get_type()[0]) {
        case 'A': {
            Request newrequest = Request("REMOVE", thisrequest.get_studentCode(), thisrequest.get_studentName(),thisrequest.get_new_UcCode(),"","", "");
            if (request_remove(newrequest)) {
                Color_Print("cyan", "Student ");
                Color_Print("yellow", newrequest.get_studentCode());
                Color_Print("cyan", " was removed from class ");
                Color_Print("yellow", newrequest.get_current_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", newrequest.get_current_UcCode(), true);
                processed_requests.pop();
            }
            if (get_student_from_studentCode(newrequest.get_studentCode())->get_classes().empty()) {
                Color_Print("red", "The student ");
                Color_Print("yellow", newrequest.get_studentCode());
                Color_Print("red", " no longer has classes.");
                Color_Print("blue", " Do you want to delete him?");
                Color_Print("cyan", " [Y/N]", true);
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    Color_Print("red", "Invalid Input, please try again", true);
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
                Color_Print("cyan", "Student ");
                Color_Print("yellow", newrequest.get_studentCode());
                Color_Print("cyan", " is now in class ");
                Color_Print("yellow", newrequest.get_new_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return;
        }
        case 'S': {
            Request newrequest = Request("SWITCH", thisrequest.get_studentCode(), thisrequest.get_studentName(), thisrequest.get_new_UcCode(), thisrequest.get_new_classCode(), thisrequest.get_current_UcCode(), thisrequest.get_current_classCode());
            if (request_switch(newrequest)) {
                Color_Print("cyan", "Student ");
                Color_Print("yellow", newrequest.get_studentCode());
                Color_Print("cyan", " was removed from class ");
                Color_Print("yellow", newrequest.get_current_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", newrequest.get_current_UcCode());
                Color_Print("cyan", " and is now in class ");
                Color_Print("yellow", newrequest.get_new_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", newrequest.get_new_UcCode(), true);
                processed_requests.pop();
            }
            return;
        }
        case 'N': {
            thisrequest.set_type("DELETE");
            request_delete(thisrequest);
            processed_requests.pop();
            Color_Print("cyan", "Student with the student code ");
            Color_Print("yellow", thisrequest.get_studentCode());
            Color_Print("cyan", " was removed.", true);
            return;
        }
        case 'D': {
            thisrequest.set_type("NEW");
            request_new(thisrequest);
            processed_requests.pop();
            Color_Print("cyan", "Student with the student code ");
            Color_Print("yellow", thisrequest.get_studentCode());
            Color_Print("cyan", " was added with its previous UCs.", true);
            Student* student = get_student_from_studentCode(thisrequest.get_studentCode());
            for (int i = 0; i < stoi(thisrequest.get_current_UcCode()); i++) {
                Request add_uc = processed_requests.top();
                Class* newclass = get_class_from_classCode_and_UcCode(add_uc.get_current_classCode(), add_uc.get_current_UcCode());
                add_student_to_class(student, newclass);
                processed_requests.pop();
            }
            return;
        }
    }
}

void LEIC::process_next_request() {
    if(pending_requests.empty()) {
        Color_Print("cyan", "There are no requests to process", true);
        return;
    }
    Request request = pending_requests.front();
    pending_requests.pop();
    switch (request.get_type()[0]) {
        case 'A': {
            if (request_add(request)) {
                Color_Print("cyan", "Student ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("cyan", " is now in class ");
                Color_Print("yellow", request.get_new_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", request.get_new_UcCode(), true);
            }
            break;
        }
        case 'R': {
            if (request_remove(request)){
                Color_Print("cyan", "Student ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("cyan", " was removed from class ");
                Color_Print("yellow", request.get_current_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", request.get_current_UcCode(), true);
            }
            if (get_student_from_studentCode(request.get_studentCode())->get_classes().empty()) {
                Color_Print("red", "The student ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("red", " no longer has classes.");
                Color_Print("blue", " Do you want to delete him?");
                Color_Print("cyan", " [Y/N]", true);
                string answer;
                cin >> answer;
                while(answer != "Y" && answer != "N") {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    Color_Print("red", "Invalid Input, please try again", true);
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
                Color_Print("cyan", "Student ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("cyan", " was removed from class ");
                Color_Print("yellow", request.get_current_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", request.get_current_UcCode());
                Color_Print("cyan", " and is now in class ");
                Color_Print("yellow", request.get_new_classCode());
                Color_Print("cyan", " in UC ");
                Color_Print("yellow", request.get_new_UcCode(), true);
            }
            break;
        }
        case 'N': {
            if(request_new(request)){
                Color_Print("cyan", "The student with code ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("cyan", " was added.", true);
            }
            break;
        }
        case 'D': {
            if (request_delete(request)){
                Color_Print("cyan", "The student with code ");
                Color_Print("yellow", request.get_studentCode());
                Color_Print("cyan", " was removed.", true);
            }
            break;
        }
    }
}

void LEIC::process_requests() {
    if(pending_requests.empty()) {
        Color_Print("cyan", "There are no requests to process", true);
        return;
    }
    while (!pending_requests.empty()) {
        process_next_request();
    }
}

void LEIC::upload_requests() {
    queue<Request> file_requests;
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
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            getline(iss,studentName);
            file_requests.emplace(Type,StudentCode, studentName,"","","", "");
        }
        else if (Type == "DELETE") {
            iss >> StudentCode;
            if (!is_number2(StudentCode) || StudentCode.length() != 9) {
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            getline(iss,studentName);
            file_requests.emplace(Type,StudentCode, "","","","", "");
        }
        else if (add || remove || Switch) {
            getline(iss, StudentCode, ',');
            if (!is_number2(StudentCode) || StudentCode.length() != 9) {
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            getline(iss, oldUcCode, ',');
            if ((add && !oldUcCode.empty())
                || ((!add) && (!exists_Uc(oldUcCode) || oldUcCode.empty()))) {
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            getline(iss, newUcCode, ',');
            if ((!remove) && (!exists_Uc(newUcCode))
                || (remove && !newUcCode.empty())) {
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            iss >> newClassCode;
            if (((!remove) && !exists_class(newUcCode,newClassCode))
                || (remove && !newClassCode.empty())) {
                Color_Print("red", "Invalid input in the given file. Line ");
                Color_Print("yellow", to_string(countLines), true);
                return;
            }
            file_requests.emplace(Type,StudentCode, "",oldUcCode,"",newUcCode, newClassCode);
        }
        else {
            Color_Print("red", "Invalid input in the given file. Line ");
            Color_Print("yellow", to_string(countLines), true);
            return;
        }
        countLines++;
    }
    requestsFile.close();
    while(!file_requests.empty()) {
        add_request_to_process(file_requests.front());
        file_requests.pop();
    }
}

void LEIC::save_to_files() {
    ofstream students_classesSaveFile("../students_classes_save.csv", ofstream::out | ofstream::trunc);
    ofstream accepted_requestsFile("../accepted_requests.csv", ofstream::out | ofstream::trunc);
    ofstream pending_requestsFile("../pending_requests.csv", ofstream::out | ofstream::trunc);
    students_classesSaveFile << "CAP = " << CAP << endl;
    students_classesSaveFile << "StudentCode,StudentName,UcCode,ClassCode" << endl;
    accepted_requestsFile << "Type,StudentCode,StudentName,oldUcCode,newUcCode,oldClassCode,newClassCode" << endl;
    pending_requestsFile << "Type,StudentCode,StudentName,oldUcCode,newUcCode,oldClassCode,newClassCode" << endl;

    for (pair<string, Student> up_s: code_students) {
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
        accepted_requestsFile << r.get_type() << ','
                          << r.get_studentCode() << ','
                          << r.get_studentName() << ','
                          << r.get_current_UcCode() << ','
                          << r.get_current_classCode() << ','
                          << r.get_new_UcCode() << ','
                          << r.get_new_classCode() << endl;
    }
    while(!pending_requests.empty()){
        Request r = pending_requests.front();
        pending_requests.pop();
        pending_requestsFile << r.get_type() << ','
                          << r.get_studentCode() << ','
                          << r.get_studentName() << ','
                          << r.get_current_UcCode() << ','
                          << r.get_current_classCode() << ','
                          << r.get_new_UcCode() << ','
                          << r.get_new_classCode() << endl;
    }
    students_classesSaveFile.close();
    accepted_requestsFile.close();
    pending_requestsFile.close();
}