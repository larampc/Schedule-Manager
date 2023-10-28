#include "student.h"
#include <iostream>
#include <iomanip>
using namespace std;

Student::Student(string name, string studentup): name(name), student_up(studentup), classes() {}

string Student::get_name() const {
  return name;
}

string Student::get_student_up() const {
  return student_up;
}

list<Class*> Student::get_classes() {
  return classes;  
}

void Student::add_class(Class* c) {
  classes.push_back(c);
}

void Student::remove_class_from_uc(string uccode) {
    Class* to_remove = nullptr;
    for(Class* c: classes) {
        if (c->get_ucCode()==uccode) to_remove = c;
    }
    classes.remove(to_remove);
}

void Student::print_schedule() {
    cout << "Schedule of " << name << endl;
    set<Lesson> allclasses;
    for(Class* someclass: classes) {
        set<Lesson> thislessons = someclass->get_lessons();
        allclasses.insert(thislessons.begin(), thislessons.end());
    }
    string current = "";
    for(Lesson lesson: allclasses) {
        if (current == "") {
            current = lesson.get_weekday();
            cout << lesson.get_weekday() << endl;
        }
        if (lesson.get_weekday() != current) {
            current = lesson.get_weekday();
            cout << lesson.get_weekday() << endl;
        }
        cout << setw(2) << setfill('0') << lesson.get_starttime().get_hour() << ":" <<
        setw(2) << setfill('0') << lesson.get_starttime().get_minute() <<
        " - " << setw(2) << setfill('0') << lesson.get_endtime().get_hour() << ":" <<
        setw(2) << setfill('0') << lesson.get_endtime().get_minute() <<
        "\t" << setw(2) << setfill(' ') << lesson.get_type() << "  " << lesson.get_thisclass() << endl;
    }
}

bool Student::has_uc(std::string uccode) {
    for (Class *c: classes) {
        if (c->get_ucCode() == uccode){
            return true;
        }
    }
    return false;
}

std::string Student::get_class_from_uc(std::string uccode) {
    for (Class* c: classes) {
        if (c->get_ucCode() == uccode) return c->get_classCode();
    }
    return "Not Found";
}
