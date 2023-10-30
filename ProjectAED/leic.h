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

/**
 * \class LEIC
 * \brief This is a class for the course LEIC.
 *
 * This class keeps track of all of LEIC's Classe, UC's, all Student and respective changes.
 */
class LEIC {
private:
  std::vector<Class> classes;
  std::set<std::string> ucs;
  std::map<std::string, Student> up_students;
  std::queue<Request> requests; //pedidos para serem tratados class request?
  std::stack<Request> processed_requests; //últimos pedidos
  int CAP = 30;
public:
    /**
     * \brief Creates a new LEIC.
     *
     * Reads the files and populates the data in LEIC.
     *
     * @param filenameclasses The file name and location for the Classes file.
     * @param filenamestudents The file name and location for the Students file.
     * @param save_file Whether to read the modified files or the original files, True or False.
     */
    LEIC(std::string filenameclasses, std::string filenamestudents, bool save_file);
    /**
    * \brief Gets the Student with the given up.
    *
    * @param up The up of the Student to get.
    * @return Reference to the Student with the given up.
    */
    Student* get_student_from_up(std::string up);
    /**
    * \brief Gets all existing Class in LEIC.
    *
    * @return All existing Class in LEIC.
    */
    std::vector<Class> get_classes() const;
    /**
    * \brief Gets the UC codes of all existing UC.
    *
    * @return The UC codes of all existing UC.
    */

    std::set<std::string> get_ucs() const;
    /**
    * \brief Gets the Class codes of all existing Class.
    *
    * @return The Class codes of all existing Class.
    */
    std::set<std::string> get_classcodes() const;
    /**
     * \brief Gets the Class with the given Class code and UC code.
     *
     * @param classcode The Class code of the Class to get.
     * @param uccode The UC code of the Class to get.
     * @return Reference to the Class with the given Class code and UC code.
     */
    Class* get_class_from_classcode_and_uccode(std::string classcode, std::string uccode);
    /**
     * \brief Gets all Class with the given UC code.
     *
     * @param uccode The UC code of all Class to get.
     * @return References of all Class with the given UC code.
     */
    std::vector<Class*> get_classes_from_uccode(std::string uccode);
    /**
    * \brief Gets the Cap, the max number of Student that can be in a Class.
    *
    * @return The Cap.
    */
    int get_cap();
    /**
    * \brief Sets a new Cap, the max number of Student that can be in a Class.
    *
    * @param cap The value to set the Cap.
    */
    void set_cap(int cap);
    /**
     * \brief Checks using binary search if a Class with the given Class code and UC code exists.
     *
     * @param ucCode The UC code of the Class to check.
     * @param classCode The Class code of the Class to check.
     * @return True if a Class with the given Class code and UC code exists.
     */
    bool exists_class(std::string ucCode, std::string classCode);
    /**
    * \brief Checks if a UC with the given UC code exists.
    *
    * @param ucCode The UC code of the UC to check.
    * @return True if a UC with the given UC code exists.
    */
    bool exists_uc(std::string ucCode);
    /**
     * \brief Checks if any Class with the given UC code has less Student than the Cap.
     *
     * @param uccode The UC code of the UC to check.
     * @return True if any Class with the given UC code has less Student than the Cap.
     */
    bool uc_has_vacancy(std::string uccode);
    /**
    * \brief Prints all of the Student enrolled in the course sorted by their up.
    */
    void list_students_by_up();
    /**
    * \brief Prints all of the Student enrolled in the course sorted by their name.
    */
    void list_students_by_name();
    /**
     * \brief Prints all of the Student enrolled in the given UC sorted by their up.
     *
     * @param uc The UC code of the UC to print all Student.
     */
    void list_uc_students_by_up(std::string uc);
    /**
     * \brief Prints all of the Student enrolled in the given UC sorted by their name.
     *
     * @param uc The UC code of the UC to print all Student.
     */
    void list_uc_students_by_name(std::string uc);
    /**
     * \brief Prints all of the Student enrolled in the given Class sorted by their up.
     *
     * @param class_ The Class to print all Student.
     */
    void list_class_students_by_up(Class* class_) const;
    /**
     * \brief Prints all of the Student enrolled in the given Class sorted by their name.
     *
     * @param class_ The Class to print all Student.
     */
    void list_class_students_by_name(Class* class_) const;
    /**
     * \brief Prints all Class and their respective number of Student enrolled.
     */
    void list_number_students_class();
    /**
     * \brief Gets the number of Student with at least the given number of UCs.
     *
     * @param n The number of UCs to check.
     * @return The number of Student with at least the given number of UCs.
     */
    int students_in_n_ucs(int n);
    /**
     * \brief Checks if by inserting a Student into a Class, the Class Balance is valid.
     *
     * @param newClass The Class to insert a Student to check.
     * @return True if by inserting a Student into a Class, the Class Balance is valid.
     */
    bool class_balance_valid(Class newClass);
    /**
     * \brief Checks if a Student schedule can accept all TP or PL Lesson of the given Class,
     *        excluding all of the old Class's Lesson if applicable.
     *
     * @param student The Student to check Schedule compatibility.
     * @param newclass The Class to check Schedule compatibility.
     * @param oldclass The Class to exclude from the Student's Schedule, if applicable.
     * @return True if the Student schedule can accept all TP or PL Lesson of the given Class,
     *        excluding all of the old Class's Lesson if applicable.
     */
    bool compatible_schedules(Student student, Class* newclass, Class* oldclass = nullptr);
    /**
     * \brief Adds a new Student to the data.
     *
     * @param student The Student to add.
     */
    void add_student(Student student);
    /**
     * \brief Adds the given Student to the given Class, updating the data accordingly.
     *
     * @param student The Student to enroll in the given Class.
     * @param newclass The Class to enroll the given Student.
     */
    void add_student_to_class(Student* student, Class* newclass);
    /**
     * \brief Removes the given Student from the given Class, updating the data accordingly.
     *
     * @param student The Student to remove from the given Class.
     * @param newclass The Class to remove the given Student.
     */
    void remove_student_from_class(Student* student, Class* newclass);
    /**
     * \brief Adds the given Request to the Request queue to be executed later.
     *
     * @param request The Request to add to the queue.
     */
    void add_request_to_process(Request request);
    /**
     * \brief Uploads all the Request in the requests.csv file to the queue and runs them.
     */
    void upload_requests();
    /**
     * \brief Runs the given Request of type ADD about the given Student and returns True if it is valid and ran with no errors.
     *
     * @param request The Request to run.
     * @param student The Student related to the Request.
     * @return True if the given Request is valid and ran with no errors.
     */
    bool request_add(Request request);
    /**
     * \brief Runs the given Request of type REMOVE about the given Student and returns True if it is valid and ran with no errors.
     *
     * @param request The Request to run.
     * @param student The Student related to the Request.
     * @return True if the given Request is valid and ran with no errors.
     */
    bool request_remove(Request request);
    /**
     * \brief Runs the given Request of type SWITCH about the given Student and returns True if it is valid and ran with no errors.
     *
     * @param request The Request to run.
     * @param student The Student related to the Request.
     * @return True if the given Request is valid and ran with no errors.
     */
    bool request_switch(Request request);
    /**
     * \brief Undoes the last Request that was done successfully.
     *
     * @return True if it undid with no errors.
     */
    bool undo_request();
    /**
     * \brief Executes all Request in the queue by checking the type running it.
     */
    void process_requests();
    /**
     * \brief Saves all modifications and all successful Requests to files.
     */
    void save_to_files();
};

#endif