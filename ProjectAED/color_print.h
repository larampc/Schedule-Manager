#ifndef AEDPROJECT_COLOR_PRINT_H
#define AEDPROJECT_COLOR_PRINT_H

#include <string>

/**
 * \class Color_Print
 * \brief A custom class to handle prints with colors.
 *
 * This class can print to the console with or without colors.
 */
class Color_Print {
public:
    /**
     * \brief Prints the given string with the given colors.
     *
     * @param use_colors True to enable prints with colors, false otherwise.
     * @param color The color of the letters, if applicable.
     * @param line The string to print.
     * @param newLine True to end line after the given string, false otherwise.
     * @param background_color The color of the background, if applicable.
     */
    Color_Print(bool use_colors, std::string color, std::string line, bool newLine = false, std::string background_color = "none");
};


#endif //AEDPROJECT_COLOR_PRINT_H
