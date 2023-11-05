#ifndef AEDPROJECT_REQUEST_H
#define AEDPROJECT_REQUEST_H
#include <string>
#include <iostream>
#include "student.h"

/**
 * \class Request
 * \brief This is a class for representing requests.
 *
 * This class keeps track of the type of request and the needed parameters to execute that Request.
 */
class Request {
private:
    std::string type;
    std::string studentCode;
    std::string studentName;
    std::string current_UcCode;
    std::string current_classCode;
    std::string new_UcCode;
    std::string new_classCode;
public:
    /**
     * \brief Creates a new Request for a given student.
     *
     * @param type The type of Request (NEW, DELETE, ADD, REMOVE, SWITCH).
     * @param studentCode The Student's code.
     * @param studentName The Student's name, if applicable (NEW, DELETE).
     * @param current_UcCode The current UC code, if applicable (REMOVE, SWITCH), or the number of UCs removed (DELETE).
     * @param current_classCode The current Class code, set automatically by the program if applicable (REMOVE, SWITCH).
     * @param new_UcCode The new UC code, if applicable (ADD, SWITCH).
     * @param new_classCode The new Class code, if applicable (ADD, SWITCH).
     */
    Request(std::string type, std::string studentCode, std::string studentName = "", std::string current_UcCode = "", std::string current_classCode = "", std::string new_UcCode = "", std::string new_classCode = "");
    /**
     * \brief Gets the type of the Request.
     *
     * @return The type of the Request (ADD, REMOVE, SWITCH, NEW, DELETE).
     */
    std::string get_type() const;
    /**
     * \brief Gets the name of the Student the Request is related to.
     *
     * @return The Student's name.
     */
    std::string get_studentName() const;
    /**
     * \brief Gets the Student's code.
     *
     * @return The Student's code.
     */
    std::string get_studentCode() const;
    /**
     * \brief Gets the current Class code.
     *
     * @return The current Class code, empty string if not applicable.
     */
    std::string get_current_classCode() const;
    /**
     * \brief Gets the new Class code.
     *
     * @return The new Class code, empty string if not applicable.
     */
    std::string get_new_classCode() const;
    /**
     * \brief Gets the current UC code.
     *
     * @return The current UC code, empty string if not applicable.
     */
    std::string get_current_UcCode() const;
    /**
     * \brief Gets the new UC code.
     *
     * @return The new UC code, empty string if not applicable.
     */
    std::string get_new_UcCode() const;
    /**
     * \brief Sets the type of the Request.
     *
     * @param type The type (ADD, REMOVE, SWITCH, NEW, DELETE).
     */
    void set_type(std::string type);
    /**
     * \brief Sets the name of the Student in the Request.
     *
     * @param name The name of the Student.
     */
    void set_name(std::string name);
    /**
     * \brief Sets the new Class code of the Request.
     *
     * @param classCode The Class code.
     */
    void set_new_class(std::string classCode);
    /**
     * \brief Sets the current Class code of the Request.
     *
     * @param classCode The Class code.
     */
    void set_current_class(std::string classCode);
    /**
     * \brief Sets the current UC code of the Request.
     *
     * @param classCode The UC code.
     */
    void set_current_uc(std::string ucCode);
    /**
     * \brief Prints the Request.
     */
    void print_request() const;
};

#endif //AEDPROJECT_REQUEST_H
