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
    void remove_class_from_uc(std::string uccode); //faz sentido ser pointers?
    void print_schedule();
    bool valid_UCnumber();
    bool has_uc(std::string uccode);
    std::string get_class_from_uc(std::string uccode);
};

#endif //PROJECTAED_STUDENT_H
