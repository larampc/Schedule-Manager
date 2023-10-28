#ifndef PROJECTAED_LEIC_H
#define PROJECTAED_LEIC_H
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "student.h"
#include "request.h"

const int CAP = 30;

class LEIC {
private:
  std::vector<Class> classes;
  std::set<std::string> ucs;
  std::map<std::string, Student> up_students;
  std::queue<Request> requests; //pedidos para serem tratados class request?
  std::stack<Request> processed_requests; //últimos pedidos
public:
  LEIC(std::string filenameclasses, std::string filenamestudents); //ler ficheiros e guardar info
  Student* get_student_from_up(std::string up);
  std::set<std::string> get_ucs() const;
  std::vector<Class> get_classes() const;
  void listStudentsByUP();
  void listStudentsByName();
  void listUCStudentsByUP(std::string uc);
  void listUCStudentsByName(std::string uc);
  void list_class_students_by_UP(Class* class_) const;
  void list_class_students_by_Name(Class* class_) const;
  Class* get_class_from_classcode_and_uccode(std::string classcode, std::string uccode);
  std::vector<Class*> get_classes_from_uccode(std::string uccode);
  //void listStudents();
  bool class_balance_valid(Class newClass);
  bool compatible_schedules(Student student, Class* newclass, Class* oldclass = nullptr);
  void numberstudents_class();
  void add_student_to_class(Student* student, Class* newclass);
  void remove_student_from_class(Student* student, Class* newclass);
  bool uc_has_vacancy(std::string uccode, int cap);
  bool process_request(Request request);
  //outras funções: sorts, listings, pedidos...
  //opcao -> criar class script para ter as funcionlidades
};

#endif