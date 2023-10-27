#ifndef PROJECTAED_LESSON_H
#define PROJECTAED_LESSON_H
#include "time.h"
#include <string>

class Lesson {
private:
    std::string Class;
    std::string Weekday; //int??
    const Time StartTime;
    const Time EndTime;
    std::string Type;
public:
    Lesson(std::string thisclass, std::string weekday, Time starttime, Time endtime, std::string type);
    std::string get_thisclass();
    std::string get_weekday();
    std::string get_type() const;
    Time get_starttime();
    Time get_endtime();
    // To do: método de comparação :  Lesson operator<(lesson l);
    bool operator<(const Lesson &l) const;
};



#endif //PROJECTAED_LESSON_H
