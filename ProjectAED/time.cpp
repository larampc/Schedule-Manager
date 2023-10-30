#include "time.h"
using namespace std;

Time::Time(float time) : hour(int(time)), minute((time - int(time)) * 60), timeFloat(time) {}

Time::Time(std::string time) {
    timeFloat = stof(time);
    *this = Time(timeFloat);
}

int Time::get_hour() const {
    return hour;
}

int Time::get_minute() const {
    return minute;
}

float Time::get_time_in_float() const {
    return timeFloat;
}

bool Time::operator<(const Time &t) const{
    return timeFloat < t.timeFloat;
}

