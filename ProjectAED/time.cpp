#include "time.h"
using namespace std;

Time::Time(float time) : hour(int(time)), minute((time - int(time)) * 60), timefloat(time) {}

Time::Time(std::string time) {
    timefloat = stof(time);
    *this = Time(timefloat);
}

int Time::get_hour() const {
    return hour;
}

int Time::get_minute() const {
    return minute;
}

float Time::get_timefloat() const {
    return timefloat;
}

bool Time::operator<(const Time &t) const{
    return timefloat < t.timefloat;
}

