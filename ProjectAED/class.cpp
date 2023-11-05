#include "class.h"
using namespace std;

Class::Class(std::string classCode, std::string ucCode): classCode(classCode), ucCode(ucCode), studentsCode(set<string>()), lessons(set<Lesson>()) {}

string Class::get_classCode() const {
    return classCode;
}

std::string Class::get_ucCode() const {
    return ucCode;
}

set<Lesson> Class::get_lessons() const {
    return lessons;
}

set<std::string> Class::get_students() const {
    return studentsCode;
}

void Class::add_lesson(Lesson l) {
    lessons.insert(l);
}

void Class::add_student(std::string studentCode) {
    studentsCode.insert(studentCode);
}

void Class::remove_student(std::string studentCode) {
    studentsCode.erase(studentCode);
}

bool Class::operator== (const Class& c) const{
    return this->classCode == c.classCode && this->ucCode == c.ucCode;
}

bool Class::operator<(const Class &c) const {
    return this->ucCode < c.ucCode || (this->ucCode == c.ucCode && this->classCode < c.classCode);
}
