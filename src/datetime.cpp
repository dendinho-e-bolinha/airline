#include "datetime.h"
#include <stdexcept>

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

Date::Date(const Date &date) {
    this->year = date.year;
    this->month = date.month;
    this->day = date.day;
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

Time::Time(const Time &time) {
    this->hour = time.hour;
    this->minute = time.minute;
    this->second = time.second;
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

Datetime::Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second) : Date(day, month, year), Time(hour, minute, second) {}
Datetime::Datetime(const Datetime &datetime) : Datetime(datetime.getYear(), datetime.getMonth(), datetime.getDay(), datetime.getHour(), datetime.getMinute(), datetime.getSecond()) {};


ostream &operator<<(ostream &os, const Datetime &datetime) {
    os << datetime.getYear() << datetime.getMonth()
       << datetime.getHour() << datetime.getDay()
       << datetime.getHour() << datetime.getMinute()
       << datetime.getSecond();

    return os;
}

bool Datetime::operator<(const Datetime &datetime) {
    if (this->getYear() != datetime.getYear())
        return this->getYear() < datetime.getYear();
    if (this->getMonth() != datetime.getMonth())
        return this->getMonth() < datetime.getMonth();
    if (this->getDay() != datetime.getDay())
        return this->getDay() < datetime.getDay();
    if (this->getHour() != datetime.getHour())
        return this->getHour() < datetime.getHour();
    if (this->getMinute() != datetime.getMinute())
        return this->getMinute() < datetime.getMinute();
    if (this->getSecond() != datetime.getSecond())
        return this->getSecond() < datetime.getSecond();
}


bool Datetime::operator==(const Datetime &datetime) {
    return !(this < datetime) && !(datetime < this);
}