#include "datetime.h"
#include <stdexcept>
#include <string>

using namespace std;

Date::Date(unsigned int day, unsigned int month,unsigned int year) {
    if (0 < day && day < 32)
        this->day = day;
    else throw invalid_argument("Day value must be between 1 and 31");
    if (0 < month && month < 13)
        this->month = month;
    else throw invalid_argument("Month value must be between 1 and 12");
    this->year = year;
}

void Date::setDay(unsigned int day) {
    if (0 < day  && day < 32)
        this->day= day;
    else
        throw invalid_argument("Day value must been between 1 and 31");
}

void Date::setMonth(unsigned int month) {
    if (0 < month && month < 13)
        this->month = month;
    else
        throw std::invalid_argument("Month value must be between 1 and 12");
}

void Date::setYear(unsigned int year) {
    this->year = year;
}

unsigned int Date::getDay() const {
    return this->day;
}

unsigned int Date::getMonth() const {
    return this->month;
}

unsigned int Date::getYear() const {
    return this->year;
}

Time::Time(unsigned int hour, unsigned int minute, unsigned int second) {
    if (0 <= hour && hour < 24)
        this->hour = hour;
    else
        throw invalid_argument("Hour value must be between 0 and 23");
    if (0 <= minute && minute < 60)
        this->minute = minute;
    else
        throw invalid_argument("Minute value must be between 0 and 59");
    if (0 <= second && second < 60)
        this->second = second;
    else
        throw invalid_argument("Second value must be between 0 and 59");
}

void Time::setHour(unsigned int hour) {
    if (0 <= hour && hour < 24)
        this->hour = hour;
    else
        throw invalid_argument("Hour value must be between 0 and 23");
}

void Time::setMinute(unsigned int minute) {
    if (0 <= minute && minute < 60)
        this->minute = minute;
    else
        throw invalid_argument("Minute value must be between 0 and 59");
}

void Time::setSecond(unsigned int second) {
    if (0 <= second && second < 60)
        this->second = second;
    else
        throw invalid_argument("Second value must be between 0 and 59");
}

unsigned int Time::getHour() const {
    return this->hour;
}

unsigned int Time::getMinute() const {
    return this->minute;
}

unsigned int Time::getSecond() const {
    return this->second;
}

std::string Time::toString(const Time &time) {
    return std::to_string(time.getHour()) + ':'
    + std::to_string(time.getMinute()) + ':'
    + std::to_string(time.getSecond());
}

Time Time::toTime(std::string &time) {
    unsigned hour, minute, second;
    char delim = ':';
    unsigned long pos = time.find(delim);
    hour = stoi(time.substr(0, pos));
    time.erase(0, pos + 1);

    pos = time.find(delim);
    minute = stoi(time.substr(0, pos));
    time.erase(0, pos + 1);

    second = stoi(time);

    return {hour, minute, second};
}

Datetime::Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second) : Date(day, month, year), Time(hour, minute, second) {}

std::string Datetime::toString(const Datetime &datetime) {
    return std::to_string(datetime.getDay()) + '-' + std::to_string(datetime.getMonth())
    + '-' + std::to_string(datetime.getYear()) + '-' + std::to_string(datetime.getHour())
    + '-' + std::to_string(datetime.getMinute()) + '-' + std::to_string(datetime.getSecond());
}

Datetime Datetime::toDatetime(string &datetime) {
    unsigned year, month, day, hour, minute, second;
    char delim = '-';
    unsigned long pos = datetime.find(delim);
    year = stoi(datetime.substr(0, pos));
    datetime.erase(0, pos + 1);

    pos = datetime.find(delim);
    month = stoi(datetime.substr(0, pos));
    datetime.erase(0, pos + 1);

    pos = datetime.find(delim);
    day = stoi(datetime.substr(0, pos));
    datetime.erase(0, pos + 1);

    pos = datetime.find(delim);
    hour = stoi(datetime.substr(0, pos + 1));
    datetime.erase(0, pos + 1);

    pos = datetime.find(delim);
    minute = stoi(datetime.substr(0, pos));
    datetime.erase(0, pos + 1);

    second = stoi(datetime);
    return {year, month, day, hour, minute, second};
}
