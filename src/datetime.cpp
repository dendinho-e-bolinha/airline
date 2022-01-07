#include "datetime.h"
#include "interact.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>

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
        throw invalid_argument("Month value must be between 1 and 12");
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

Time::Time(unsigned int hour, unsigned int minute) {
    if (0 <= hour && hour < 24)
        this->hour = hour;
    else
        throw invalid_argument("Hour value must be between 0 and 23");
    if (0 <= minute && minute < 60)
        this->minute = minute;
    else
        throw invalid_argument("Minute value must be between 0 and 59");
}

Time::Time(const Time &time) {
    this->hour = time.hour;
    this->minute = time.minute;
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

unsigned int Time::getHour() const {
    return this->hour;
}

unsigned int Time::getMinute() const {
    return this->minute;
}

Datetime::Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute) : Date(day, month, year), Time(hour, minute) {}
Datetime::Datetime(const Datetime &datetime) : Datetime(datetime.getYear(), datetime.getMonth(), datetime.getDay(), datetime.getHour(), datetime.getMinute()) {};

string Datetime::str() const {
    ostringstream out;
    out << Date::str() << ' ' << Time::str();

    return out.str();
}

ostream &operator<<(ostream &os, const Datetime &datetime) {
    return os << datetime.str() << '\n';
}

string Date::str() const {
    ostringstream out;

    out << this->getYear() << '/'
        << setw(2) << setfill('0') << this->getMonth() << '/'
        << setw(2) << setfill('0') << this->getDay();

    return out.str();
}

string Time::str() const {
    ostringstream out;

    out << setw(2) << setfill('0') << this->getHour() << ':'
        << setw(2) << setfill('0') << this->getMinute();

    return out.str();
}

bool Time::operator<(const Time &time) const {
    if (this->getHour() != time.getHour())
        return this->getHour() < time.getHour();

    return this->getMinute() < time.getMinute();
}

bool Datetime::operator<(const Datetime &datetime) const {
    if (this->getYear() != datetime.getYear())
        return this->getYear() < datetime.getYear();
    if (this->getMonth() != datetime.getMonth())
        return this->getMonth() < datetime.getMonth();
    if (this->getDay() != datetime.getDay())
        return this->getDay() < datetime.getDay();
    if (this->getHour() != datetime.getHour())
        return this->getHour() < datetime.getHour();

    return this->getMinute() < datetime.getMinute();
}

bool Datetime::operator==(const Datetime &datetime) const {
    return !(*this < datetime) && !(datetime < (*this));
}

Datetime Datetime::readFromString(const string &str) {
    static validation_error invalid_date = validation_error("The provided date is not in the correct format (YYYY/MM/dd HH:mm)");
    
    istringstream in(str);
    unsigned int year, month, day, hour, minute;

    in >> year;
    if (!in || in.get() != '/')
        throw invalid_date;

    in >> month;
    if (!in || in.get() != '/')
        throw invalid_date;

    in >> day;
    if (!in || in.get() != ' ')
        throw invalid_date;

    in >> hour;
    if (!in || in.get() != ':')
        throw invalid_date;

    in >> minute;
    if (!in || !in.eof())
        throw invalid_date;

    try {
        return Datetime(year, month, day, hour, minute);
    } catch (invalid_argument exception) {
        throw invalid_date;
    }
}

Date Date::readFromString(const string &str) {
    static validation_error invalid_date = validation_error("The provided date is not in the correct format (YYYY/MM/dd)");
    
    istringstream in(str);
    unsigned int year, month, day, hour, minute;

    in >> year;
    if (!in || in.get() != '/')
        throw invalid_date;

    in >> month;
    if (!in || in.get() != '/')
        throw invalid_date;

    in >> day;
    if (!in || !in.eof())
        throw invalid_date;

    try {
        return Date(year, month, day);
    } catch (invalid_argument exception) {
        throw invalid_date;
    }
}

Time Time::readFromString(const string &str) {
    static validation_error invalid_date = validation_error("The provided time is not in the correct format (HH:mm)");
    
    istringstream in(str);
    unsigned int hour, minute;

    in >> hour;
    if (!in || in.get() != ':')
        throw invalid_date;

    in >> minute;
    if (!in || !in.eof())
        throw invalid_date;

    try {
        return Time(hour, minute);
    } catch (invalid_argument exception) {
        throw invalid_date;
    }
}