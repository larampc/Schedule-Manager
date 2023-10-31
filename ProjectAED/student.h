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
     * \brief Creates a new student.
     *
     * @param name The student's name.
     * @param studentCode The student's code.
     */
    Student(std::string name, std::string studentCode);
    /**
     * \brief Gets student's name.
     *
     * @return The student's name.
     */
    std::string get_name() const;
    /**
     * \brief Gets student's code.
     *
     * @return The student's code.
     */
    std::string get_studentCode() const;
    /**
     * \brief Gets classes in which the student is enrolled.
     *
     * @return A list of pointers to the classes in which a student is enrolled.
     */
    std::list<Class*> get_classes() const;
    /**
     * \brief Gets the Class of the given UC in which the student is enrolled.
     *
     * @param ucCode The UC code of the class.
     * @return A pointer to the Class of the given UC.
     */
    Class* get_class_from_uc(std::string ucCode) const;
    /**
     * \brief Checks if the student is enrolled in the given UC.
     *
     * @param ucCode The code of the UC to check.
     * @return True if the student is enrolled in the given UC.
     */
    bool has_uc(std::string ucCode) const;
    /**
     * \brief Adds a new Class.
     *
     * @param c A pointer to the new Class to add.
     */
    void add_class(Class* c);
    /**
     * \brief Removes a Class based on its UC code.
     *
     * @param ucCode The UC code of the Class to remove.
     */
    void remove_class_from_uc(std::string ucCode);
    /**
     * \brief Prints the student's schedule.
     */
    void print_schedule() const;
};

#endif //PROJECTAED_STUDENT_H
