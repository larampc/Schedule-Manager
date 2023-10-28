#ifndef PROJECTAED_LESSON_H
#define PROJECTAED_LESSON_H
#include "time.h"
#include <string>

class Lesson {
private:
    std::string Class;
    std::string weekday; //int??
    const Time startTime;
    const Time endTime;
    std::string type;
public:
    Lesson(std::string thisclass, std::string weekday, Time starttime, Time endtime, std::string type);
    std::string get_thisclass();
    std::string get_weekday() const;
    std::string get_type() const;
    Time get_starttime() const;
    Time get_endtime() const;
    bool overlap(Lesson oldlesson) const;
    bool operator<(const Lesson &l) const;
};



#endif //PROJECTAED_LESSON_H
