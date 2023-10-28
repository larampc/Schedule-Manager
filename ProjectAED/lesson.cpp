#include "lesson.h"
#include <map>
using namespace std;

Lesson::Lesson(string thisclass, string weekday, Time starttime, Time endtime, string type): Class(thisclass), weekday(weekday), startTime(starttime), endTime(endtime), type(type) {}

std::string Lesson::get_thisclass() {
    return Class;
}

string Lesson::get_weekday() const {
    return weekday;
}

Time Lesson::get_starttime() const {
    return startTime;
}

Time Lesson::get_endtime() const {
    return endTime;
}

string Lesson::get_type() const {
    return type;
}

bool Lesson::overlap(Lesson oldlesson) const {
    if (oldlesson.get_weekday() == weekday){
        return (max(startTime.get_timefloat(), oldlesson.startTime.get_timefloat()) < min(endTime.get_timefloat(), oldlesson.endTime.get_timefloat()));
    }
    return false;
}

bool Lesson::operator<(const Lesson &l) const{
    map<string, int> weekday_to_int = {{"Monday", 1},
                                       {"Tuesday", 2},
                                       {"Wednesday", 3},
                                       {"Thursday", 4},
                                       {"Friday", 5}};
    if (weekday_to_int[this->weekday] < weekday_to_int[l.weekday]) return true;
    return weekday_to_int[this->weekday] <= weekday_to_int[l.weekday] && this->startTime < l.startTime;
}
