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

class LEIC {
private:
  std::vector<Class> classes;
  std::set<std::string> ucs;
  std::map<std::string, Student> up_students;
  std::queue<Request> requests; //pedidos para serem tratados class request?
  std::stack<Request> processed_requests; //últimos pedidos
public:
  LEIC(std::string filenameclasses, std::string filenamestudents); //ler ficheiros e guardar info
  Student get_student_from_up(std::string up);
  std::set<std::string> get_ucs() const;
  std::vector<Class> get_classes() const;
  void listStudentsByUP();
  void listStudentsByName();
  void listUCStudentsByUP(std::string uc);
  void listUCStudentsByName(std::string uc);
  void list_class_students_by_UP(Class* class_) const;
  void list_class_students_by_Name(Class* class_) const;
  Class* get_class_from_classcode_and_uccode(std::string classcode, std::string uccode);
  //void listStudents();
  bool classBalanceValid(Class newClass);
  bool compatibleSchedules(Student student, Class* newclass, Class* oldclass = nullptr);
  void numberstudents_class();
  void addStudenttoClass(Student student, Class* newclass);
  void removeStudentfromClass(Student student, Class* newclass);
  bool uc_Has_Vacancy(std::string uccode, int cap);
  //outras funções: sorts, listings, pedidos...
  //opcao -> criar class script para ter as funcionlidades
};

#endif