#ifndef AEDPROJECT_COLOR_PRINT_H
#define AEDPROJECT_COLOR_PRINT_H

#include <string>

class Color_Print {
public:
    Color_Print(bool use_colors, std::string color, std::string line, bool newLine = false, std::string background_color = "none");
};


#endif //AEDPROJECT_COLOR_PRINT_H
