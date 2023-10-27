#include "lesson.h"
#include <map>
using namespace std;

Lesson::Lesson(string thisclass, string weekday, Time starttime, Time endtime, string type): Class(thisclass), Weekday(weekday), StartTime(starttime), EndTime(endtime), Type(type) {}

std::string Lesson::get_thisclass() {
    return Class;
}

string Lesson::get_weekday() {
    return Weekday;
}

Time Lesson::get_starttime() {
    return StartTime;
}

Time Lesson::get_endtime() {
    return EndTime;
}

string Lesson::get_type() const {
    return Type;
}

bool Lesson::operator<(const Lesson &l) const{
    map<string, int> weekday_to_int = {{"Monday", 1},
                                       {"Tuesday", 2},
                                       {"Wednesday", 3},
                                       {"Thursday", 4},
                                       {"Friday", 5}};
    if (weekday_to_int[this->Weekday] < weekday_to_int[l.Weekday]) return true;
    return weekday_to_int[this->Weekday] <= weekday_to_int[l.Weekday] && this->StartTime < l.StartTime;
}