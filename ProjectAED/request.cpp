#include "request.h"
using namespace std;

Request::Request(string type, string studentCode, string studentName, string current_UcCode, string current_classCode, string new_uc, string new_classCode
                  ): type(type), studentCode(studentCode), studentName(studentName), current_classCode(current_classCode), new_classCode(new_classCode), current_UcCode(current_UcCode), new_UcCode(new_uc) {}

string Request::get_type() {
    return type;
}

string Request::get_studentCode() {
    return studentCode;
}

string Request::get_studentName() {
    return studentName;
}

string Request::get_current_classCode() {
    return current_classCode;
}

string Request::get_new_classCode() {
    return new_classCode;
}

string Request::get_current_UcCode() {
    return current_UcCode;
}

string Request::get_new_UcCode() {
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

void Request::set_name(std::string name) {
    this->studentName = name;
}
