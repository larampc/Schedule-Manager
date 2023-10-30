#ifndef PROJECTAED_LESSON_H
#define PROJECTAED_LESSON_H
#include "time.h"
#include <string>

/**
 * \class Lesson
 * \brief This is a class for representing Lessons.
 *
 * This class keeps track of the weekday, start time, end time, type and the parent Class.
 */
class Lesson {
private:
    std::string Class;
    std::string weekday; //int??
    const Time startTime;
    const Time endTime;
    std::string type;
public:
    /**
     * \brief Creates a new Lesson.
     *
     * @param thisclass Parent Class code.
     * @param weekday Weekday of the Lesson.
     * @param starttime Start time of the Lesson.
     * @param endtime End time of the Lesson.
     * @param type Type of the lesson, (T, TP or PL).
     */
    Lesson(std::string thisclass, std::string weekday, Time starttime, Time endtime, std::string type);
    /**
     * \brief Gets the parent Class of the Lesson.
     *
     * @return The parent Class of the Lesson.
     */
    std::string get_thisclass();
    /**
     * \brief Gets the weekday of the Lesson.
     *
     * @return The weekday of the Lesson.
     */
    std::string get_weekday() const;
    /**
     * \brief Gets the type of the Lesson.
     *
     * @return The type of the Lesson, (T, TP or PL).
     */
    std::string get_type() const;
    /**
     * \brief Gets the Start time of the Lesson.
     *
     * @return The start time of the Lesson.
     */
    Time get_starttime() const;
    /**
     * \brief Gets the end time of the Lesson.
     *
     * @return The end time of the Lesson.
     */
    Time get_endtime() const;
    /**
     * \brief Checks if a Lesson overlaps in time.
     *
     * @param oldlesson The Lesson to check.
     * @return True if the Lesson overlaps in time.
     */
    bool overlap(Lesson oldlesson) const;
    /**
     * \brief Prints the Lesson.
     */
    void print_lesson() const;
    /**
     * \brief Less than operator for Lesson comparison.
     *
     * @param l Lesson to compare.
     * @return True if this Lesson is before in time of the Lesson to compare.
     */
    bool operator<(const Lesson &l) const;
};



#endif //PROJECTAED_LESSON_H
