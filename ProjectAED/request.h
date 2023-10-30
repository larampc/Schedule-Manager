#ifndef AEDPROJECT_REQUEST_H
#define AEDPROJECT_REQUEST_H
#include <string>
#include "student.h"

/**
 * \class Request
 * \brief This is a class for representing requests.
 *
 * This class keeps track of the type of request, the student's up to execute the request on and the needed parameters to execute that Request if applicable.
 */
class Request {
private:
    std::string type;
    bool uc_class; //true if it's class
    std::string student_up;
    std::string current_class;
    std::string new_class;
    std::string current_uc;
    std::string new_uc;
public:
    /**
     * \brief Creates a new Request.
     *
     * @param type The type of Request.
     * @param uc_class True if the Request is related an UC, False if it is related to a Class.
     * @param student_up The Student's up to execute the Request on.
     * @param current_class The current Class code of the Student to execute the Request on, if applicable.
     * @param new_class The new Class code for the Student to execute the Request on, if applicable.
     * @param current_uc The current UC code of the Student to execute the Request on, if applicable.
     * @param new_uc The new UC code for the Student to execute the Request on, if applicable.
     */
    Request(std::string type, bool uc_class, std::string student_up, std::string current_class = "", std::string new_class = "", std::string current_uc = "", std::string new_uc = "");
    /**
     * \brief Gets the type of the Request.
     *
     * @return The type of the Request, ADD, REMOVE or SWITCH.
     */
    std::string get_type();
    /**
     * \brief Gets if the Request is related to an UC ou a Class.
     *
     * @return True if the Request is related an UC, False if it is related to a Class.
     */
    bool get_uc_class();
    /**
     * \brief Gets the Student's up to execute the Request on.
     *
     * @return The Student's up to execute the Request on.
     */
    std::string get_student_up();
    /**
     * \brief Gets the current Class code of the Student to execute the Request on, if applicable.
     *
     * @return The current Class code of the Student to execute the Request on, if applicable.
     */
    std::string get_current_class();
    /**
     * \brief Gets the new Class code for the Student to execute the Request on, if applicable.
     *
     * @return The new Class code for the Student to execute the Request on, if applicable.
     */
    std::string get_new_class();
    /**
     * \brief Gets the current UC code of the Student to execute the Request on, if applicable.
     *
     * @return The current UC code of the Student to execute the Request on, if applicable.
     */
    std::string get_current_uc();
    /**
     * \brief Gets the new UC code for the Student to execute the Request on, if applicable.
     *
     * @return The new UC code for the Student to execute the Request on, if applicable.
     */
    std::string get_new_uc();
    /**
     * \brief Sets the type of the Request.
     *
     * @param type The type to set the Request, ADD, REMOVE or SWITCH.
     */
    void set_type(std::string type);
    void set_new_class(std::string new_class);
    void set_current_class(std::string current_class);
};

#endif //AEDPROJECT_REQUEST_H
