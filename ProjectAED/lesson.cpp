#include "lesson.h"
#include <map>
#include <iomanip>
#include <iostream>
using namespace std;
#include "color_print.h"

Lesson::Lesson(string parentClass, string weekday, Time starttime, Time endtime, string type): Class(parentClass), weekday(weekday), startTime(starttime), endTime(endtime), type(type) {}

string Lesson::get_weekday() const {
    return weekday;
}

string Lesson::get_type() const {
    return type;
}

bool Lesson::overlap(Lesson l) const {
    return l.get_weekday() == weekday && (max(startTime.get_time_in_float(), l.startTime.get_time_in_float()) <
                                                  min(endTime.get_time_in_float(), l.endTime.get_time_in_float()));
}

void Lesson::print_lesson() const {
    string out;
    stringstream outstream;
    string uc;
    string cl;
    stringstream ss(Class);
    getline(ss, uc, ' ');
    getline(ss, cl, ' ');
    outstream << setw(2) << setfill('0') << startTime.get_hour() << ":"
              << setw(2) << setfill('0') << startTime.get_minute()
              << " - " << setw(2) << setfill('0') << endTime.get_hour() << ":"
              << setw(2) << setfill('0') << endTime.get_minute()
              << "\t" << setw(2) << setfill(' ') << type << " "
              << left << setw(9) << setfill(' ') << uc << cl;
    getline(outstream, out);
    Color_Print("white", out, true);
}

bool Lesson::operator<(const Lesson &l) const{
    map<string, int> weekday_to_int = {{"Monday", 1},
                                       {"Tuesday", 2},
                                       {"Wednesday", 3},
                                       {"Thursday", 4},
                                       {"Friday", 5}};
    return (weekday_to_int[this->weekday] < weekday_to_int[l.weekday])
            || (weekday_to_int[this->weekday] == weekday_to_int[l.weekday] && this->startTime < l.startTime)
            || (weekday_to_int[this->weekday] == weekday_to_int[l.weekday] && this->startTime == l.startTime && this->Class < l.Class);
}
