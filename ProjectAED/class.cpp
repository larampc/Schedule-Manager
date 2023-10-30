#include "class.h"
#include <iostream>
using namespace std;

Class::Class(std::string classCode, std::string ucCode): classCode(classCode), ucCode(ucCode), students_up(set<string>()), lessons(set<Lesson>()) {}

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

void Class::add_student(std::string studentCode) {
    students_up.insert(studentCode);
}

void Class::remove_student(std::string studentCode) {
    students_up.erase(studentCode);
}

bool Class::operator== (const Class& c) const{
    return this->classCode == c.classCode && this->ucCode == c.ucCode;
}

bool Class::operator<(const Class &c) const {
    return this->ucCode < c.ucCode || (this->ucCode == c.ucCode && this->classCode < c.classCode);
}
