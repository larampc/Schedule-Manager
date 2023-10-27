#ifndef AEDPROJECT_SCRIPT_H
#define AEDPROJECT_SCRIPT_H
#include "leic.h"

class Script {
private:
    LEIC data;
public:
    Script();
    void run();
    void request();
    void listings();
    void listSchedules();
};


#endif //AEDPROJECT_SCRIPT_H
