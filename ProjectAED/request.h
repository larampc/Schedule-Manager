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
    std::string studentCode;
    std::string current_classCode;
    std::string new_classCode;
    std::string current_UcCode;
    std::string new_UcCode;
public:
    /**
     * \brief Creates a new Request.
     *
     * @param type The type of Request.
     * @param uc_class True if the Request is related an UC, False if it is related to a Class.
     * @param studentCode The Student's up to execute the Request on.
     * @param current_classCode The current Class code of the Student to execute the Request on, if applicable.
     * @param new_classCode The new Class code for the Student to execute the Request on, if applicable.
     * @param current_UcCode The current UC code of the Student to execute the Request on, if applicable.
     * @param new_UcCode The new UC code for the Student to execute the Request on, if applicable.
     */
    Request(std::string type, bool uc_class, std::string studentCode, std::string current_classCode = "", std::string new_classCode = "", std::string current_UcCode = "", std::string new_UcCode = "");
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
    bool get_Uc_class();
    /**
     * \brief Gets the Student's up to execute the Request on.
     *
     * @return The Student's up to execute the Request on.
     */
    std::string get_studentCode();
    /**
     * \brief Gets the current Class code of the Student to execute the Request on, if applicable.
     *
     * @return The current Class code of the Student to execute the Request on, if applicable.
     */
    std::string get_current_classCode();
    /**
     * \brief Gets the new Class code for the Student to execute the Request on, if applicable.
     *
     * @return The new Class code for the Student to execute the Request on, if applicable.
     */
    std::string get_new_classCode();
    /**
     * \brief Gets the current UC code of the Student to execute the Request on, if applicable.
     *
     * @return The current UC code of the Student to execute the Request on, if applicable.
     */
    std::string get_current_UcCode();
    /**
     * \brief Gets the new UC code for the Student to execute the Request on, if applicable.
     *
     * @return The new UC code for the Student to execute the Request on, if applicable.
     */
    std::string get_new_UcCode();
    /**
     * \brief Sets the type of the Request.
     *
     * @param type The type to set the Request, ADD, REMOVE or SWITCH.
     */
    void set_type(std::string type);
    void set_new_class(std::string classCode);
    void set_current_class(std::string classCode);
};

#endif //AEDPROJECT_REQUEST_H
