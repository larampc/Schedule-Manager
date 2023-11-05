#ifndef PROJECTAED_CLASS_H
#define PROJECTAED_CLASS_H
#include "lesson.h"
#include <set>

/**
 * \class Class
 * \brief This is a class for representing Classes.
 *
 * This class keeps track of the class code, UC code, all Lessons of the Class and all Students enrolled in it.
 */
class Class {
private:
    std::string classCode;
    std::string ucCode; 
    std::set<Lesson> lessons;
    std::set<std::string> studentsCode;
public:
    /**
     * \brief Creates a new Class.
     *
     * @param classCode The Class's class code.
     * @param ucCode The Class's UC code.
     */
    Class(std::string classCode, std::string ucCode);
    /**
     * \brief Gets the class code of the Class.
     *
     * @return The class code of the Class.
     *
     * \par Complexity
     * O(1)
     */
    std::string get_classCode() const;
    /**
     * \brief Gets the UC code of the Class.
     *
     * @return The UC code of the Class.
     *
     * \par Complexity
     * O(1)
     */
    std::string get_ucCode() const;
    /**
     * \brief Gets all Lesson of the Class.
     *
     * @return All Lesson of the Class.
     *
     * \par Complexity
     * O(1)
     */
    std::set<Lesson> get_lessons() const;
    /**
     * \brief Gets all Student of the Class.
     *
     * @return All Student of the Class.
     *
     * \par Complexity
     * O(1)
     */
    std::set<std::string> get_students() const;
    /**
     * \brief Adds a new Lesson to the Class.
     *
     * @param l The new Lesson to add.
     *
     * \par Complexity
     * O(log n)
     */
    void add_lesson(Lesson l);
    /**
     * \brief Adds a new Student to the Class.
     *
     * @param studentCode The new Student to add.
     *
     * \par Complexity
     * O(log n)
     */
    void add_student(std::string studentCode);
    /**
     * \brief Removes a Student from the Class.
     *
     * @param studentCode The Student to be removed.
     *
     * \par Complexity
     * O(log n)
     */
    void remove_student(std::string studentCode);
    /**
     * \brief Equality operator for Class comparison.
     *
     * @param c Class to compare.
     * @return True if this Class is equal to the Class to compare.
     */
    bool operator == (const Class& c) const;
    /**
     * \brief Less than operator for Class comparison.
     *
     * @param c Class to compare.
     * @return True if this Class's code is less than the Class code of the Class to compare, or, if equal, if this Class's UC code is less than the one of the Class to compare.
     */
    bool operator < (const Class& c) const;
};
#endif //PROJECTAED_CLASS_H
