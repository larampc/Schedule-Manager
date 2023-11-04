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
     *
     * @param save_file True to use saved data, false to use original data.
     */
    Script(bool save_file);
    /**
     * \brief Outputs the main menu and handles the respective inputs.
     */
    void run();
    /**
     * \brief Outputs the update registration menu and handles the respective inputs.
     */
    void update_registration();
    /**
     * \brief Gets from input the information needed for a specific request.
     *
     * @param studentCode studentCode from student who is requesting.
     * @param option Type of request to get.
     */
    void get_request(std::string studentCode, std::string option);
    /**
     * \brief Outputs the listings menu and handles the respective inputs.
     */
    void listings();
    /**
     * \brief Handles the creation of a new Student and inserts it into the data.
     */
    void handle_registration();
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
    /**
     * \brief Outputs the occupations menu and handles the respective inputs.
     */
    void occupations();
    /**
     * \brief Prints the default message of input error.
     */
    void invalid();
   /**
    * \brief Outputs the menu for listing occupations of a given year, handling the respective inputs
    *
    * @param year The year to get the occupations.
    */
    void year_occupations(std::string year);
   /**
    * \brief Outputs the menu for listing occupations of a given UC, handling the respective inputs
    *
    * @param UcCode The UC code to get the occupations.
    */
    void Uc_occupations(std::string UcCode);
   /**
    * \brief Outputs the menu for listing occupations of a given Class, handling the respective inputs
    *
    * @param classCode The Class code of the Class to get the occupations.
    */
    void class_occupations(std::string classCode);

    void settings();
};


#endif //AEDPROJECT_SCRIPT_H
