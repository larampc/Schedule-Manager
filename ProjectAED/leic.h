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
#include "Request.h"

class LEIC {
private:
  std::vector<Class> classes;
  std::map<std::string, Student> up_students;
  std::queue<Request> requests; //pedidos para serem tratados class Request?
  std::stack<Request> processed_requests; //últimos pedidos
public:
  LEIC(std::string filenameclasses, std::string filenamestudents); //ler ficheiros e guardar info
  Student get_student_from_up(std::string up);
  //outras funções: sorts, listings, pedidos...
  //opcao -> criar class script para ter as funcionlidades
};

#endif