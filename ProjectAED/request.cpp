#include "request.h"
using namespace std;

Request::Request(string type, string studentCode, string studentName, string current_UcCode, string current_classCode, string new_uc, string new_classCode
                  ): type(type), studentCode(studentCode), studentName(studentName), current_classCode(current_classCode), new_classCode(new_classCode), current_UcCode(current_UcCode), new_UcCode(new_uc) {}

string Request::get_type() const {
    return type;
}

string Request::get_studentCode() const {
    return studentCode;
}

string Request::get_studentName() const {
    return studentName;
}

string Request::get_current_classCode() const {
    return current_classCode;
}

string Request::get_new_classCode() const {
    return new_classCode;
}

string Request::get_current_UcCode() const {
    return current_UcCode;
}

string Request::get_new_UcCode() const {
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

void Request::set_current_uc(std::string ucCode) {
    current_UcCode = ucCode;
}

void Request::print_request() const {
    switch(type[0]) {
        case 'A' : {
            cout << type << ' ' << studentCode << ' ' << new_UcCode << ' ' << new_classCode << endl;
            return;
        }
        case 'R' : {
            cout << type << ' ' << studentCode << ' ' << current_UcCode << endl;
            return;
        }
        case 'S' : {
            cout << type << ' ' << studentCode << ' ' << current_UcCode << ' ' << new_UcCode << ' ' << new_classCode << endl;
            return;
        }
    }
    cout << type << ' ' << studentCode << ' ' << studentName << endl;
}
