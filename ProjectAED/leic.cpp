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
        Class c = Class(classcode, uccode);
        string thisclass = classcode + " " + uccode;
        Lesson lesson = Lesson(thisclass, weekday, Time(starthour), Time(duration), type); // create new lesson
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
    }
    studentsFile.close();
//    for(pair<string, Student> p : up_students){
//        cout << p.first << ' ' << p.second.get_name() << ' ' << p.second.get_classes().front()->get_classCode() << ' ' << p.second.get_classes().back()->get_classCode() << endl;
//    }
}

Student LEIC::get_student_from_up(std::string up) {
    return up_students.at(up);
}

void LEIC::listStudents() {
    //mudar sorts

    cout << "UPNUMBER\tNAME\n-----------------------------------------\n";
    for(pair<string, Student> p : up_students){
        cout << p.first << " | " << p.second.get_name() << '\n';
    }
}

