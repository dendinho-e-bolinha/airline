#pragma once

#include <ostream>

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

    /**
     * @brief Copies a Date instance
     */
    Date(const Date &date);


    // Setters

    void setDay(unsigned int day);
    void setMonth(unsigned int month);
    void setYear(unsigned int year);

    // Getters

    unsigned int getDay() const;
    unsigned int getMonth() const;
    unsigned int getYear() const;

    /**
     * @brief Converts a Date instance to a String
     */
    std::string str() const;

    /**
    * @brief Converts a string into a Date intance
    */
    static Date readFromString(const std::string &str);
};

class Time {
    unsigned int hour, minute;

public:
    /**
     * @brief Creates an object of type Time with the given attributes
     *
     * @param hour An hour between 0-23
     * @param minute A minute between 0-59
     */
    Time(unsigned int hour, unsigned int minute);
    Time(const Time &time);

    // Setters

    void setHour(unsigned int hour);
    void setMinute(unsigned int minute);

    // Getters

    unsigned int getHour() const;
    unsigned int getMinute() const;

    /**
     * @brief Converts a Time instance to a String
     */
    std::string str() const;

    static Time readFromString(const std::string &str);

    /**
     * @overload Relational operator overload
     */
    bool operator<(const Time &time) const;
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
     */
    Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute);

    /**
     * @brief Copies a Datetime instance
     */
    Datetime(const Datetime &datetime);


    /**
     * @overload Displays a Datetime
     */
    friend std::ostream &operator<<(std::ostream &os, const Datetime &datetime);

    /**
     * @brief Converts a Datetime instance to a string 
     */
    std::string str() const;

    // Relational operators overload
    bool operator<(const Datetime &datetime) const;
    bool operator==(const Datetime &datetime) const;
    
    /**
     * @brief Converts a string to a Datetime instance
     * @param str String representing the datetime
     * 
     * @note The string must be in a valid format: `YYYY/MM/dd HH:mm`
     */
    static Datetime readFromString(const std::string &str);

};