#ifndef PROJECTAED_CLASS_H
#define PROJECTAED_CLASS_H
#include "lesson.h"
#include <set>

/**
 * \class Class
 * \brief This is a class for representing Classes.
 *
 * This class keeps track of the class code, UC code, all Lesson of the Class and all Student enrolled in it.
 */
class Class {
private:
    std::string classCode;
    std::string ucCode; 
    std::set<Lesson> lessons; //unir sets de lessons para formar horário
    std::set<std::string> students_up;
public:
    /**
     * \brief Creates a new Class.
     *
     * @param classcode The Class's class code.
     * @param uccode The Class's UC code.
     */
    Class(std::string classcode, std::string uccode);
    /**
     * \brief Gets the class code of the Class.
     *
     * @return The class code of the Class.
     */
    std::string get_classCode();
    /**
     * \brief Gets the UC code of the Class.
     *
     * @return The UC code of the Class.
     */
    std::string get_ucCode();
    /**
     * \brief Gets all Lesson of the Class.
     *
     * @return All Lesson of the Class.
     */
    std::set<Lesson> get_lessons();
    /**
     * \brief Gets all Student of the Class.
     *
     * @return All Student of the Class.
     */
    std::set<std::string> get_students();
    /**
     * \brief Adds a new Lesson to the Class.
     *
     * @param l The new Lesson to add.
     */
    void add_lesson(Lesson l);
    /**
     * \brief Adds a new Student to the Class.
     *
     * @param student_up The new Student to add.
     */
    void add_student(std::string student_up);
    /**
     * \brief Removes a Student from the Class.
     *
     * @param student_up The Student to be removed.
     */
    void remove_student(std::string student_up);
    /**
     * \brief Equal operator for Class comparison.
     *
     * @param c Class to compare.
     * @return True if this Class is equal to the Class to compare.
     */
    bool operator == (const Class& c) const;
    bool operator < (const Class& c) const;
};
#endif //PROJECTAED_CLASS_H
