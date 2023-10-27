#ifndef AEDPROJECT_REQUEST_H
#define AEDPROJECT_REQUEST_H
#include <string>
#include "student.h"

class Request {
private:
    std::string type;
    std::string student_up;
    std::string current;
    std::string new_;
public:
    Request(std::string type, std::string student_up, std::string current, std::string new_);
};


#endif //AEDPROJECT_REQUEST_H
