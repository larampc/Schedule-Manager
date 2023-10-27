#include "class.h"
#include <iostream>
using namespace std;

Class::Class(std::string classcode, std::string uccode): classCode(classcode), ucCode(uccode), students_up(set<string>()), lessons(set<Lesson>()) {}

string Class::get_classCode(){
    return classCode;
}

std::string Class::get_ucCode() {
    return ucCode;
}

set<Lesson> Class::get_lessons() {
    return lessons;
}

set<std::string> Class::get_students() {
    return students_up;
}

void Class::add_lesson(Lesson l){
    lessons.insert(l);
}

void Class::add_student(std::string student_up) {
    students_up.insert(student_up);
}

void Class::remove_student(std::string student_up) {
    students_up.erase(student_up);
}

bool Class::operator== (const Class& c) const{
    return this->classCode == c.classCode && this->ucCode == c.ucCode;
}
