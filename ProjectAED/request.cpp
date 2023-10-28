#include "request.h"
using namespace std;

Request::Request(std::string type, bool uc_class, std::string student_up, std::string current_class, std::string new_class,
                 std::string current_uc, std::string new_uc): type(type), uc_class(uc_class), student_up(student_up), current_class(current_class), new_class(new_class), current_uc(current_uc), new_uc(new_uc) {}

std::string Request::get_type() {
    return type;
}

bool Request::get_uc_class() {
    return uc_class;
}

std::string Request::get_student_up() {
    return student_up;
}

std::string Request::get_current_class() {
    return current_class;
}

std::string Request::get_new_class() {
    return new_class;
}

std::string Request::get_current_uc() {
    return current_uc;
}

std::string Request::get_new_uc() {
    return new_uc;
}


