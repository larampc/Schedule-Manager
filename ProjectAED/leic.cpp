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

Student LEIC::get_student_from_up(std::string up) {
    return up_students.at(up);
}

std::set<std::string> LEIC::get_ucs() const {
    return ucs;
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
            for(string up : c.get_students()){
                cout << up << " | " << up_students.at(up).get_name() << '\n';
            }
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

bool LEIC::classBalanceValid(Class newClass) {
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

bool LEIC::compatibleSchedules(Student student, Class* newclass, Class* oldclass) {
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

void LEIC::addStudenttoClass(Student student, Class *newclass) {
    newclass->add_student(student.get_student_up());
    student.add_class(newclass);
}

void LEIC::removeStudentfromClass(Student student, Class *newclass) {
    newclass->remove_student(student.get_student_up());
    student.remove_class(newclass);
}

bool LEIC::ucHasVacancy(std::string uccode, int cap) {
    for (Class& c: classes) {
        if (c.get_ucCode() == uccode){
            if (c.get_students().size() < cap) return true;
        }
    }
    return false;
}




