#include "lesson.h"
#include <map>
#include <iomanip>
#include <iostream>
using namespace std;

Lesson::Lesson(string thisclass, string weekday, Time starttime, Time endtime, string type): Class(thisclass), weekday(weekday), startTime(starttime), endTime(endtime), type(type) {}

std::string Lesson::get_classCode() {
    return Class;
}

string Lesson::get_weekday() const {
    return weekday;
}

Time Lesson::get_start_time() const {
    return startTime;
}

Time Lesson::get_end_time() const {
    return endTime;
}

string Lesson::get_type() const {
    return type;
}

bool Lesson::overlap(Lesson l) const {
    return l.get_weekday() == weekday && (max(startTime.get_time_in_float(), l.startTime.get_time_in_float()) <
                                                  min(endTime.get_time_in_float(), l.endTime.get_time_in_float()));
}

void Lesson::print_lesson() const {
    cout << setw(2) << setfill('0') << startTime.get_hour() << ":"
         << setw(2) << setfill('0') << startTime.get_minute()
         << " - " << setw(2) << setfill('0') << endTime.get_hour() << ":"
         << setw(2) << setfill('0') << endTime.get_minute()
         << "\t" << setw(2) << setfill(' ') << type << " " << Class << endl;
}

bool Lesson::operator<(const Lesson &l) const{
    map<string, int> weekday_to_int = {{"Monday", 1},
                                       {"Tuesday", 2},
                                       {"Wednesday", 3},
                                       {"Thursday", 4},
                                       {"Friday", 5}};
    return (weekday_to_int[this->weekday] < weekday_to_int[l.weekday]) || (weekday_to_int[this->weekday] <= weekday_to_int[l.weekday] && this->startTime < l.startTime);
}
