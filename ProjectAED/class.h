#ifndef PROJECTAED_CLASS_H
#define PROJECTAED_CLASS_H
#include "lesson.h"
#include <set>

class Class {
private:
    std::string classCode;
    std::string ucCode; 
    std::set<Lesson> lessons; //unir sets de lessons para formar horário
    std::set<std::string> students_up;
public:
    Class(std::string classcode, std::string uccode);
    std::string get_classCode();
    std::string get_ucCode();
    std::set<Lesson> get_lessons();
    std::set<std::string> get_students(); 
    void add_lesson(Lesson l);
    void add_student(std::string student_up);
    void remove_student(std::string student_up);
    bool operator == (const Class& c) const;
};
#endif //PROJECTAED_CLASS_H
