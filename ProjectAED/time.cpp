#include "time.h"
using namespace std;

Time::Time(float time) : hour(int(time)), minute((time - int(time)) * 60) {}

Time::Time(std::string time) {
    *this = Time(stof(time));
}

int Time::get_hour() const {
    return hour;
}

int Time::get_minute() const {
    return minute;
}

bool Time::operator<(const Time &t) const{
    return this->get_hour()*100+this->get_minute() < t.get_hour()*100+t.get_minute();
}

