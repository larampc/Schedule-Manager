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
    std::string get_weekday() const;
    std::string get_type() const;
    Time get_starttime() const;
    Time get_endtime() const;
    // To do: método de comparação :  Lesson operator<(lesson l);
    bool operator<(const Lesson &l) const;
};



#endif //PROJECTAED_LESSON_H
