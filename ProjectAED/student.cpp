#include "student.h"
#include <iostream>
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

void Student::remove_class(Class* c) {
  classes.remove(c);
}

void Student::print_schedule() {
    cout << "Schedule of " << name << endl;
    set<Lesson> allclasses;
    for(Class* someclass: classes) {
        set<Lesson> thislessons = someclass->get_lessons();
        allclasses.insert(thislessons.begin(), thislessons.end());
    }
    for(Lesson lesson: allclasses) {
        cout << lesson.get_weekday() << ": " << lesson.get_starttime().get_hour() << " " << lesson.get_thisclass() << endl;
    }
}