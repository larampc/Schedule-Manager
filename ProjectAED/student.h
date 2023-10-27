#ifndef PROJECTAED_STUDENT_H
#define PROJECTAED_STUDENT_H
#include <string>
#include <list>
#include "class.h"
class Student{
private:
    std::string student_up;
    std::string name;
    std::list<Class*> classes;
public:
    //ver coisas const
    Student(std::string name, std::string studentup);
    std::string get_name() const;
    std::string get_student_up() const;
    std::list<Class*> get_classes();
    void add_class(Class* c);
    void remove_class(Class* c); //faz sentido ser pointers?
};

#endif //PROJECTAED_STUDENT_H
