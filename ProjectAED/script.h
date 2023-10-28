#ifndef AEDPROJECT_SCRIPT_H
#define AEDPROJECT_SCRIPT_H
#include "leic.h"

/**
 * \mainpage This is the main page
 *
 * \section description_sec Project description
 *
 * Place Project description here.
 *
 * \section utility_sec What can this project do?
 *
 * \subsection part1_subsec Part 1
 *
 * This is part 1.
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
    Script();
    /**
     * \brief Outputs the main menu and handles the respective inputs.
     */
    void run();
    /**
     * \brief Outputs the request menu and handles the respective inputs.
     */
    void request();
    /**
     * \brief Outputs the listings menu and handles the respective inputs.
     */
    void listings();
    /**
     * \brief Outputs the schedule listing menu and handles the respective inputs.
     */
    void listSchedules();
    /**
     * \brief Outputs the Student listing menu and handles the respective inputs.
     */
    void listStudents();
};


#endif //AEDPROJECT_SCRIPT_H
