#ifndef PROJECTAED_LEIC_H
#define PROJECTAED_LEIC_H
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "student.h"
#include "request.h"
#include "color_print.h"

/**
 * \class LEIC
 * \brief This is a class for the course LEIC.
 *
 * This class keeps track of all LEIC's Classes, UC's, Students and respective changes.
 */
class LEIC {
private:
  std::vector<Class> classes;
  std::set<std::string> ucs;
  std::map<std::string, Student> code_students;
  std::queue<Request> pending_requests;
  std::stack<Request> processed_requests;
  int CAP = 30;
public:
    /**
     * \brief Creates a new LEIC.
     *
     * Reads the files and populates the data in LEIC.
     *
     * @param filenameclasses The file path for the Class file.
     * @param filenamestudents The file path for the Student file.
     * @param save_file Whether to read the modified files or the original files, True or False.
     */
    LEIC(std::string filenameclasses, std::string filenamestudents, bool save_file);
    /**
     * \brief Gets the Student with the given student code.
     *
     * @param studentCode The student code of the Student to get.
     * @return Reference to the Student with the given student code.
     *
     * \par Complexity
     * O(log n) in which n is the number of students in LEIC.
     */
    Student* get_student_from_studentCode(std::string studentCode);
    /**
     * \brief Gets all existing Class in LEIC.
     *
     * @return A vector with all existing Class in LEIC.
     *
     * \par Complexity
     * O(n) in which n is the number of classes in LEIC.
     */
    std::vector<Class> get_classes() const;
    /**
     * \brief Gets the UC codes of all existing UCs.
     *
     * @return A set with the UC codes of all existing UCs.
     *
     * \par Complexity
     * O(n) in which n is the number of UCs in LEIC.
     */
    std::set<std::string> get_UcCodes() const;
    /**
     * \brief Gets the class codes of all existing Class.
     *
     * @return A set with the class codes of all existing Class.
     *
     * \par Complexity
     * O(n log m) in which n is the number of classes in LEIC and m is the number of distinct class codes.
     */
    std::set<std::string> get_classCodes() const;
    /**
     * \brief Gets the Class with the given class code and UC code.
     *
     * @param classCode The class code of the Class to get.
     * @param UcCode The UC code of the Class to get.
     * @return Reference to the Class with the given class code and UC code.
     *
     * \par Complexity
     * O(log n) in which n is the number of classes in LEIC.
     */
    Class* get_class_from_classCode_and_UcCode(std::string classCode, std::string UcCode);
    /**
     * \brief Gets all Class with the given UC code.
     *
     * @param UcCode The UC code of the Class to get.
     * @return A vector of references to all the Class with the given UC code.
     *
     * \par Complexity
     * O(log n) in which n is the number of classes in LEIC.
     */
    std::vector<Class*> get_classes_from_UcCode(std::string UcCode);
    /**
     * \brief Gets the student code of the last pending Request.
     *
     * @return The student code or empty string if there are no pending Request.
     *
     * \par Complexity
     * O(1)
     */
    std::string get_studentCode_last_request() const;
    /**
     * \brief Gets the Cap, the maximum number of Student that can be in a Class.
     *
     * @return The Cap.
     *
     * \par Complexity
     * O(1)
     */
    int get_cap();
    /**
     * \brief Sets a new Cap, the maximum number of Student that can be in a Class.
     *
     * @param cap The value to set the Cap.
     *
     * \par Complexity
     * O(1)
     */
    void set_cap(int cap);
    /**
     * \brief Checks, using binary search, if a Class with the given class code and UC code exists.
     *
     * @param UcCode The UC code of the Class to check.
     * @param classCode The class code of the Class to check.
     * @return True if the Class with the given class code and UC code exists.
     *
     * \par Complexity
     * O(log n) in which n is the number of classes in LEIC.
     */
    bool exists_class(std::string UcCode, std::string classCode);
    /**
     * \brief Checks if a UC with the given UC code exists.
     *
     * @param UcCode The UC code of the UC to check.
     * @return True if a UC with the given UC code exists.
     *
     * \par Complexity
     * O(log n) in which n is the number of classes in LEIC.
     */
    bool exists_Uc(std::string UcCode);
    /**
     * \brief Checks if any Class with the given UC code has less Student than the Cap.
     *
     * @param UcCode The UC code of the UC to check.
     * @return True if any Class with the given UC code has less Student than the Cap.
     *
     * \par Complexity
     * O(max(log n, m)) in which n is the number of classes in LEIC and m is the number of classes in the UC.
     */
    bool Uc_has_vacancy(std::string UcCode);
    /**
     * \brief Prints all of the Student enrolled in the course sorted by their student code in a given order
     *
     * @param order The order to sort by True if ascending, false if descending..
     *
     * \par Complexity
     * O(n) in which n is the number of students in LEIC.
     */
    void list_students_by_studentCode(bool order) const;
    /**
     * \brief Prints all of the Student enrolled in the course sorted by their name in a given order.
     *
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n log n) in which n is the number of students in LEIC.
     */
    void list_students_by_name(bool order) const;
    /**
     * \brief Prints all of the Student enrolled in the given UC sorted by their up in a given order.
     *
     * @param UcCode The UC code of the UC to print all Student.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n m log m) in which n is the number of classes in the UC and m is the number of students in the UC.
     */
    void list_UC_students_by_studentCode(std::string UcCode, bool order);
    /**
     * \brief Prints all of the Student enrolled in the given UC sorted by their name in a given order.
     *
     * @param UcCode The UC code of the UC to print all Student.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n m log m) in which n is the number of classes in the UC and m is the number of students in the UC.
     */
    void list_UC_students_by_name(std::string UcCode, bool order);
    /**
     * \brief Prints all of the Student enrolled in the given Class sorted by their up in a given order.
     *
     * @param class_ The Class to print all Student.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n) in which n is the number of students in the Class.
     */
    void list_class_students_by_studentCode(Class* class_, bool order) const;
    /**
     * \brief Prints all of the Student enrolled in the given Class sorted by their name in a given order.
     *
     * @param class_ The Class to print all Student.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n log n) in which n is the number of students in the Class.
     */
    void list_class_students_by_name(Class* class_, bool order) const;
    /**
     * \brief Outputs the occupations of the given year sorting by UcCode in a given order.
     *
     * @param year The year to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n) in which n is the number of classes in LEIC.
     */
    void list_year_occupations_by_UC(std::string year, bool order);
    /**
     * \brief Outputs the occupations of the given year sorting by Class code in a given order.
     *
     * @param year The year to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(max(n, m log m)) in which n is the number of classes in LEIC and m is the number of classes in the given year.
     */
    void list_year_occupations_by_classCode(std::string year, bool order);
    /**
     * \brief Outputs the occupations of the given year sorting by its occupation in a given order.
     *
     * @param year The year to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(max(n, m log m)) in which n is the number of classes in LEIC and m is the number of classes in the given year.
     */
    void list_year_occupations_by_occupation(std::string year, bool order);
    /**
     * \brief Outputs the occupations of the given UC sorting by Class code in a given order
     *
     * @param UcCode The UC code of the UC to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(max(n, log m)) in which n is the number of classes in the given UC and m is the number of classes in LEIC.
     */
    void list_Uc_occupations_by_classCode(std::string UcCode, bool order);
    /**
     * \brief Outputs the occupations of the given UC sorting by its occupation in a given order
     *
     * @param UcCode The UC code of the UC to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(max(n, m log m)) in which n is the number of classes in LEIC and m is the number of classes in the given UC.
     */
    void list_Uc_occupations_by_occupation(std::string UcCode, bool order);
    /**
     * \brief Outputs the occupations of the given class sorting by UC coder in a given order
     *
     * @param classCode The class code of the UC to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(n) in which n is the number of classes in LEIC.
     */
    void list_class_occupations_by_UC(std::string classCode, bool order);
    /**
     * \brief Outputs the occupations of the given class sorting by its occupation in a given order
     *
     * @param classCode The class code of the UC to get the occupations.
     * @param order The order to sort by True if ascending, false if descending.
     *
     * \par Complexity
     * O(max(n, m log m)) in which n is the number of classes in LEIC and m is the number of classes with the given class code.
     */
    void list_class_occupations_by_occupation(std::string classCode, bool order);
    /**
     * \brief Prints all Students with at least the given number of UCs.
     *
     * @param n The number of UCs to check.
     *
     * \par Complexity
     * O(n) in which n is the number of students in LEIC.
     */
    void students_in_n_Ucs(int n);
    /**
     * \brief Checks if by switching the Student from the current Class to the new Class the class balance is not disturbed.
     *
     * @param currentClass The current Class.
     * @param newClass The new Class.
     * @return True if by switching from the current Class to the new Class the class balance is not disturbed, false otherwise.
     *
     * \par Complexity
     * O(max(log n, m)) in which n is the number of classes in LEIC and m is the number of classes in the UC.
     */
    bool not_disturb_balance(Class *currentClass, Class *newClass);
    /**
     * \brief Checks if by adding a Student into a Class, the class balance and schedule are valid.
     *
     * @param student The Student to add to the Class.
     * @param newClass The Class to add the Student to.
     * @return A vector of references to all Class with the same UC of the given Class that by adding the given Student the class balance and schedule are valid, or empty if adding a Student to the given Class, the class balance and schedule are valid.
     *
     * \par Complexity
     * O(q*n*m*r) in which q is the number of classes in the UC, n is the number of lessons of the Class checking, m is the number of classes of the student and r the number of lessons in each class.
     */
    std::vector<Class*> class_balance_valid(Student* student, Class* newClass);
    /**
     * \brief Gets the Class in the given UC that best improves class balance and is compatible with the given Student's schedule.
     *
     * @param student The Student whose schedule must be compatible with the Class.
     * @param uc The UC code of the UC to get the Class from.
     * @return The Class in the given UC that best improves the class balance and is compatible with the given Student's schedule.
     *
     * \par Complexity
     * O(q*n*m*r) in which q is the number of classes in the UC, n is the number of lessons of the Class checking, m is the number of classes of the student and r the number of lessons in each class.
     */
    Class* best_class_balance(Student* student, std::string uc);
    /**
     * \brief Checks if a Student schedule has no overlapping TP or PL Lesson with the given Class,
     *        excluding all of the old Class's Lesson, if applicable.
     *
     * @param student The Student to check schedule compatibility.
     * @param newClass The Class to check schedule compatibility.
     * @param oldClass The Class to exclude from the Student's Schedule, if applicable.
     * @return True if the Student schedule can accept all TP or PL Lesson of the given Class,
     *        excluding all of the old Class's Lesson, if applicable.
     *
     * \par Complexity
     * O(n*m*r) in which n is the number of lessons of the new Class, m is the number of classes of the student and r the number of lessons in each class.
     */
    bool compatible_schedules(Student* student, Class* newClass, Class* oldClass = nullptr);
    /**
     * \brief Adds the given Student to the given Class, updating the data accordingly.
     *
     * @param student The Student to enroll in the given Class.
     * @param newClass The Class to enroll the given Student in.
     *
     * \par Complexity
     * O(log n)
     */
    void add_student_to_class(Student* student, Class* newClass);
    /**
     * \brief Adds the given Request to the Request queue to be executed later.
     *
     * @param request The Request to add to the queue.
     *
     * \par Complexity
     * O(1)
     */
    void add_request_to_process(Request request);
    /**
     * \brief Executes the given Request of type ADD about the given Student, if possible.
     *
     * @param request The Request to run.
     * @return True if the given Request is valid and ran with no errors, False otherwise.
     */
    bool request_add(Request& request);
    /**
     * \brief Executes the given Request of type REMOVE about the given Student, if possible.
     *
     * @param request The Request to run.
     * @return True if the given Request is valid and ran with no errors, False otherwise.
     */
    bool request_remove(Request& request);
    /**
     * \brief Executes the given Request of type NEW creating a new Student, if possible.
     *
     * @param request The Request to run.
     * @return True if the given Request is valid and ran with no errors, False otherwise.
     */
    bool request_new(Request& request);
    /**
     * \brief Executes the given Request of type DELETE removing the Student, if possible.
     *
     * @param request The Request to run.
     * @return True if the given Request is valid and ran with no errors, False otherwise.
     */
    bool request_delete(Request& request);
    /**
     * \brief Executes the given Request of type SWITCH about the given Student, if possible.
     *
     * @param request The Request to run.
     * @return True if the given Request is valid and ran with no errors, False otherwise.
     */
    bool request_switch(Request& request);
    /**
     * \brief Checks if there are any pending requests.
     *
     * @return True if there are no pending requests, False otherwise.
     *
     * \par Complexity
     * O(1)
     */
    bool pending_request_is_empty();
    /**
     * \brief Empties the queue of all pending Request.
     *
     * \par Complexity
     * O(n) in which n is the number of pending requests.
     */
    void empty_pending_requests();
    /**
     * \brief Prints the queue of all pending Request.
     *
     * \par Complexity
     * O(n) in which n is the number of pending requests.
     */
    void print_pending_requests();
    /**
     * \brief Undoes the last Request that was done successfully.
     */
    void undo_request();
    /**
     * \brief Executes all Request in the queue by checking its type.
     */
    void process_requests();
    /**
     * \brief Executes next Request in the queue by checking its type.
     */
    void process_next_request();
    /**
     * \brief Uploads all the Request in the requests.csv file to the queue and runs them.
     */
    void upload_requests();
    /**
     * \brief Saves all modifications, pending and successful Requests to files.
     */
    void save_to_files();
};

#endif