#ifndef PROJECTAED_STUDENT_H
#define PROJECTAED_STUDENT_H
#include <string>
#include <list>
#include "class.h"

/**
 * \class Student
 * \brief This is a class for representing students.
 *
 * This class keeps track of the student's code, name and the classes they are registered in.
 * There is also various methods to handle student actions such as adding or removing a class.
*/
class Student{
private:
    std::string studentCode;
    std::string name;
    std::list<Class*> classes;
public:
    /**
     * \brief Creates a new Student.
     *
     * @param name The Student's name.
     * @param studentCode The Student's code.
     */
    Student(std::string name, std::string studentCode);
    /**
     * \brief Gets Student's name.
     *
     * @return The Student's name.
     *
     * \par Complexity
     * O(1)
     */
    std::string get_name() const;
    /**
     * \brief Gets Student's code.
     *
     * @return The Student's code.
     *
     * \par Complexity
     * O(1)
     */
    std::string get_studentCode() const;
    /**
     * \brief Gets classes in which the student is enrolled.
     *
     * @return A list of pointers to all the Class in which a Student is enrolled.
     *
     * \par Complexity
     * O(n) in which n is the number of Lessons of the Student.
     */
    std::list<Class*> get_classes() const;
    /**
     * \brief Gets the Class of the given UC in which the Student is enrolled.
     *
     * @param ucCode The UC code of the class.
     * @return A pointer to the Class of the given UC.
     *
     * \par Complexity
     * O(n) in which n is the number of Lessons of the Student.
     */
    Class* get_class_from_uc(std::string ucCode) const;
    /**
     * \brief Checks if the Student is enrolled in the given UC.
     *
     * @param ucCode The code of the UC to check.
     * @return True if the Student is enrolled in the given UC.
     *
     * \par Complexity
     * O(n) in which n is the number of Lessons of the Student.
     */
    bool has_uc(std::string ucCode) const;
    /**
     * \brief Adds a new Class.
     *
     * @param c A pointer to the new Class to add.
     *
     * \par Complexity
     * O(1)
     */
    void add_class(Class* c);
    /**
     * \brief Removes a Class based on its UC code.
     *
     * @param ucCode The UC code of the Class to remove.
     *
     * \par Complexity
     * O(n) in which n is the number of classes of the Student.
     */
    void remove_class_from_uc(std::string ucCode);
    /**
     * \brief Prints the Student's schedule.
     *
     * \par Complexity
     * O(n) in which n is the number of Lessons of the Student.
     */
    void print_schedule() const;
};

#endif //PROJECTAED_STUDENT_H
