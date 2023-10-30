#include "request.h"
using namespace std;

Request::Request(std::string type, bool uc_class, std::string studentCode, std::string current_classCode, std::string new_classCode,
                 std::string current_UcCode, std::string new_uc): type(type), uc_class(uc_class), studentCode(studentCode), current_classCode(current_classCode), new_classCode(new_classCode), current_UcCode(current_UcCode), new_UcCode(new_uc) {}

std::string Request::get_type() {
    return type;
}

bool Request::get_Uc_class() {
    return uc_class;
}

std::string Request::get_studentCode() {
    return studentCode;
}

std::string Request::get_current_classCode() {
    return current_classCode;
}

std::string Request::get_new_classCode() {
    return new_classCode;
}

std::string Request::get_current_UcCode() {
    return current_UcCode;
}

std::string Request::get_new_UcCode() {
    return new_UcCode;
}

void Request::set_type(string type) {
    this->type = type;
}

void Request::set_new_class(std::string classCode) {
    this->new_classCode = classCode;
}

void Request::set_current_class(std::string classCode) {
    this->current_classCode = classCode;
}
