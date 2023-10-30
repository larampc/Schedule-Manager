#ifndef AEDPROJECT_SCRIPT_H
#define AEDPROJECT_SCRIPT_H
#include "leic.h"

/**
 * \mainpage This is the main page
 *
 * \section description_sec Project description
 *
 * This program allows the visualization of schedules and related information, new registrations and update existing registrations.
 *
 * \section utility_sec What can this project do?
 *
 * \subsection part1_subsec Part 1
 *
 * This program provides several options for consulting data such as sorted lists of students in a given class and number of students in a class.
 *
 * \subsection part2_subsec Part 2
 *
 * This is part 2.
 */

/**
 * \class Script
 * \brief This class handles and runs the different commands.
 *
 * This class has the course LEIC and different functions to handle alterations to that data.
 * It is also responsible for handling inputs and outputs.
 */
class Script {
private:
    LEIC data;
public:
    /**
     * \brief Creates a new Script.
     */
    Script(bool save_file);
    /**
     * \brief Outputs the main menu and handles the respective inputs.
     */
    void run();
    /**
     * \brief Outputs the update registration menu and handles the respective inputs.
     */
    void request();
    /**
     * \brief Gets from input the information needed for a specific request.
     *
     * @param student_up UP from student who is requesting.
     * @param option Type of request to get.
     */
    void get_request(std::string student_up, std::string option);
    /**
     * \brief Outputs the listings menu and handles the respective inputs.
     */
    void listings();
    /**
     * \brief Handles the creation of a new Student and inserts it into the data.
     */
    void new_registration();
    /**
     * \brief Outputs the schedule listing menu and handles the respective inputs.
     */
    void listSchedules();
    /**
     * \brief Outputs the Student listing menu and handles the respective inputs.
     */
    void listStudents();
    /**
     * \brief Quits the program and saves all the data to files.
     */
    void quit();
    /**
     * \brief Outputs the requests menu and handles the respective inputs.
     */
    void handle_requests();
    /**
     * \brief Outputs the instructions to upload a request file and runs it if user chooses to.
     */
    void request_file();
};


#endif //AEDPROJECT_SCRIPT_H
