#include "student.h"
#include <iostream>
#include <iomanip>
using namespace std;

Student::Student(string name, string studentCode): name(name), student_up(studentCode), classes() {}

string Student::get_name() const {
  return name;
}

string Student::get_studentCode() const {
  return student_up;
}

list<Class*> Student::get_classes() {
  return classes;  
}

Class* Student::get_class_from_uc(string ucCode) {
    for (Class* c: classes) {
        if (c->get_ucCode() == ucCode) return c;
    }
    return nullptr;
}

bool Student::has_uc(string ucCode) {
    return get_class_from_uc(ucCode) != nullptr;
}

void Student::add_class(Class* c) {
  classes.push_back(c);
}

void Student::remove_class_from_uc(string ucCode) {
    classes.remove(get_class_from_uc(ucCode));
}

void Student::print_schedule() {
    cout << "Schedule of " << name << endl;
    set<Lesson> allClasses;
    for(Class* someClass: classes) {
        set<Lesson> thisLessons = someClass->get_lessons();
        allClasses.insert(thisLessons.begin(), thisLessons.end());
    }
    string current;
    for(Lesson lesson: allClasses) {
        if (current.empty()) {
            current = lesson.get_weekday();
            cout << lesson.get_weekday() << endl;
        }
        if (lesson.get_weekday() != current) {
            current = lesson.get_weekday();
            cout << lesson.get_weekday() << endl;
        }
        cout << setw(2) << setfill('0') << lesson.get_start_time().get_hour() << ":" <<
        setw(2) << setfill('0') << lesson.get_start_time().get_minute() <<
        " - " << setw(2) << setfill('0') << lesson.get_end_time().get_hour() << ":" <<
        setw(2) << setfill('0') << lesson.get_end_time().get_minute() <<
        "\t" << setw(2) << setfill(' ') << lesson.get_type() << "  " << lesson.get_classCode() << endl;
    }
}


