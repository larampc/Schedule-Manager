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

class Script {
private:
    LEIC data;
public:
    Script();
    void run();
    void request();
    void listings();
    void listSchedules();
    void listStudents();
};


#endif //AEDPROJECT_SCRIPT_H
