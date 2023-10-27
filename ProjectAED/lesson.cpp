#include "lesson.h"
using namespace std;

Lesson::Lesson(string thisclass, string weekday, Time starttime, Time duration, string type): Class(thisclass), Weekday(weekday), StartTime(starttime), Duration(duration), Type(type) {}

std::string Lesson::get_thisclass() {
    return Class;
}

string Lesson::get_weekday() {
    return Weekday;
}

Time Lesson::get_starttime() {
    return StartTime;
}

Time Lesson::get_duration() {
    return Duration;
}

string Lesson::get_type() const {
    return Type;
}

bool Lesson::operator<(const Lesson &l) const{
    if (this->Weekday < l.Weekday) return true; //weekday é string
    return !(this->Weekday > l.Weekday) && this->StartTime < l.StartTime;
}