#include "datetime.h"

using namespace std;

Date::Date(unsigned int day, unsigned int month,unsigned int year) {
    this->day = day;
    this->month = month;
    this->year = year;
}

void Date::setDay(unsigned int day) {
    this->day= day;
}

void Date::setMonth(unsigned int month) {
    this->month = month;
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
    this->hour = hour;
    this->minute = minute;
    this->second = second;
}

void Time::setHour(unsigned int hour) {
    this->hour = hour;
}

void Time::setMinute(unsigned int minute) {
    this->minute = minute;
}

void Time::setSecond(unsigned int second) {
    this->second = second;
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