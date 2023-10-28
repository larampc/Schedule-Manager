#ifndef AEDPROJECT_TIME_H
#define AEDPROJECT_TIME_H
#include <string>

/**
* \class Time
* \brief This is a class for handling time provided in float or string format.
*/
 class Time {
private:
    int hour, minute;
    float timefloat;
public:
/**
 * \brief Constructor with string.
 */
    Time(std::string time);
/**
 * \brief Constructor with float.
 */
    Time(float time);
/**
 * \brief Getter function for hour.
 */
    int get_hour() const;
/**
 * \brief Getter function for minute.
 */
    int get_minute() const;
/**
 * \brief Getter function time in float format.
 */
    float get_timefloat() const;
/**
 * \brief Less than operator for Time comparison.
 */
    bool operator<(const Time &t) const;
};


#endif //AEDPROJECT_TIME_H
