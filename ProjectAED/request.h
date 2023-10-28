#ifndef AEDPROJECT_REQUEST_H
#define AEDPROJECT_REQUEST_H
#include <string>
#include "student.h"

class Request {
private:
    std::string type;
    bool uc_class; //true if it's class
    std::string student_up;
    std::string current_class;
    std::string new_class;
    std::string current_uc;
    std::string new_uc;
public:
    Request(std::string type, bool uc_class, std::string student_up, std::string current_class = "", std::string new_class = "", std::string current_uc = "", std::string new_uc = "");
    std::string get_type();
    bool get_uc_class();
    std::string get_student_up();
    std::string get_current_class();
    std::string get_new_class();
    std::string get_current_uc();
    std::string get_new_uc();
};

#endif //AEDPROJECT_REQUEST_H
