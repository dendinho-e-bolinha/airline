#ifndef AIRLINE_DATETIME_H
#define AIRLINE_DATETIME_H

#include <string>

class Date {
    unsigned int day;
    unsigned int month;
    unsigned int year;

public:
    /**
     * @brief Creates an object of type Date with the given attributes
     *
     * @param day A day between 1-31
     * @param month A month between 1-12
     * @param year A year
     */
    Date(unsigned int day, unsigned int month, unsigned int year);
    void setDay(unsigned int day);
    void setMonth(unsigned int month);
    void setYear(unsigned int year);
    unsigned int getDay() const;
    unsigned int getMonth() const;
    unsigned int getYear() const;
};

class Time {
    unsigned int hour, minute, second;

public:
    /**
     * @brief Creates an object of type Time with the given attributes
     *
     * @param hour An hour between 0-23
     * @param minute A minute between 0-59
     * @param second A second between 0-59
     */
    Time(unsigned int hour, unsigned int minute, unsigned int second);
    void setHour(unsigned int hour);
    void setMinute(unsigned int minute);
    void setSecond(unsigned int second);
    unsigned int getHour() const;
    unsigned int getMinute() const;
    unsigned int getSecond() const;

    /**
     * @brief Converts a time instance to a string instance, separated by :
     * @param time Time instance
     * @return A string containing all data about the time instance provided
     */
    static std::string toString(const Time &time);

    /**
     * @brief Converts a string to a Time instance
     * @param time The string to be converted
     * @return A Time instance
     */
    static Time toTime(std::string &time);
};

class Datetime: public Date, public Time {
public:
    /**
     * @brief Creates an object of type Datetime with the given attributes
     *
     * @param year A year
     * @param month A month between 1-12
     * @param day A day between 1-31
     * @param hour An hour between 0-23
     * @param minute A minute between 0-59
     * @param second A second between 0-59
     */
    Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);

    /**
     * @brief Converts a datetime instance to a string instance, separated by -
     * @param datetime Datetime instance
     * @return A string containing data about the datetime instance provided
     */
    static std::string toString(const Datetime &datetime);

    /**
     * @brief Converts a string to a datetime instance
     * @param datetime The string to be converted
     * @return A Datetime instance
     */
    static Datetime toDatetime(std::string &datetime);
};

#endif //AIRLINE_DATETIME_H
