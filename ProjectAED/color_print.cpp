#include "color_print.h"
#include <iostream>
extern bool color_mode;
#ifdef _WIN32

#include <windows.h>
Color_Print::Color_Print(std::string color, std::string line, bool newLine,
                         std::string background_color) {
    if (color_mode)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int col = 0x0007;
        int back_col = 0x0000;

        if (color == "gray") col = 0x0000;
        else if (color == "blue") col = 0x0009;
        else if (color == "green") col = 0x000A;
        else if (color == "cyan") col = 0x000B;
        else if (color == "red") col = 0x000C;
        else if (color == "pink") col = 0x000D;
        else if (color == "yellow") col = 0x000E;
        else if (color == "white") col = 0x000F;

        if (background_color == "none") back_col = 0x0000;
        else if (background_color == "blue") back_col = 0x0090;
        else if (background_color == "green") back_col = 0x00A0;
        else if (background_color == "cyan") back_col = 0x00B0;
        else if (background_color == "red") back_col = 0x00C0;
        else if (background_color == "pink") back_col = 0x00D0;
        else if (background_color == "yellow") back_col = 0x00E0;
        else if (background_color == "white") back_col = 0x00F0;

        SetConsoleTextAttribute(hConsole, col + back_col);
        std::cout << line;
        SetConsoleTextAttribute(hConsole, 0x0007);
    }
    else std::cout << line;
    if (newLine) {
        std::cout << std::endl;
    }
}



#else

Color_Print::Color_Print(std::string color, std::string line, bool newLine,
                         std::string background_color) {
        if (color_mode)
        {
            std::string col = "\033[0";
            std::string back_col = "m";
            if (color == "gray") col = "\033[0;30";
            else if (color == "blue") col = "\033[0;34";
            else if (color == "green") col = "\033[0;32";
            else if (color == "cyan") col = "\033[0;36";
            else if (color == "red") col = "\033[0;31";
            else if (color == "pink") col = "\033[0;35";
            else if (color == "yellow") col = "\033[0;33";
            else if (color == "white") col = "\033[0";

            if (background_color == "none") back_col = "m";
            else if (background_color == "blue") back_col = ";44m";
            else if (background_color == "green") back_col = ";42m";
            else if (background_color == "cyan") back_col = ";46m";
            else if (background_color == "red") back_col = ";41m";
            else if (background_color == "pink") back_col = ";45m";
            else if (background_color == "yellow") back_col = ";43m";
            else if (background_color == "white") back_col = ";47m";

            std::cout << col + back_col  << line << "\033[0m";
        }
        else std::cout << line;
        if (newLine) {
            std::cout << std::endl;
        }
    }

#endif