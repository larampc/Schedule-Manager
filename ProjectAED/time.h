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
    float timeFloat;
public:
     /**
      * \brief Creates a new Time from a string.
      *
      * @param time The Time to be created in string format.
      */
    Time(std::string time);
    /**
     * \brief Creates a new Time from a float.
     *
     * @param time The Time to be created in float format.
     */
    Time(float time);
    /**
     * \brief Gets the hour.
     *
     * @return The hour of the Time.
     *
     * \paragraph Complexity Complexity O(1)
     */
    int get_hour() const;
    /**
     * \brief Gets the minutes.
     *
     * @return The minutes of the Time.
     *
     * \paragraph Complexity Complexity O(1)
     */
    int get_minute() const;
    /**
     * \brief Gets the Time in float format.
     *
     * @return The Time in float format.
     *
     * \paragraph Complexity Complexity O(1)
     */
    float get_time_in_float() const;
    /**
     * \brief Less than operator for Time comparison.
     *
     * @param t The Time to compare.
     * @return True if this Time is less than the Time to compare.
     */
    bool operator<(const Time &t) const;
    /**
     * \brief Equal operator for Time comparison.
     *
     * @param t The Time to compare.
     * @return True if this Time is equal to the Time to compare.
     */
    bool operator==(const Time &t) const;
};


#endif //AEDPROJECT_TIME_H
