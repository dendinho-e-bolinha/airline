#ifndef AIRLINE_DATETIME_H
#define AIRLINE_DATETIME_H

class Date {
    unsigned int day;
    unsigned int month;
    unsigned int year;

public:
    Date(unsigned int day, unsigned int month,unsigned int year);
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
    Time(unsigned int hour, unsigned int minute, unsigned int second);
    void setHour(unsigned int hour);
    void setMinute(unsigned int minute);
    void setSecond(unsigned int second);
    unsigned int getHour() const;
    unsigned int getMinute() const;
    unsigned int getSecond() const;
};

class Datetime: public Date, public Time {
    Datetime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
};


#endif //AIRLINE_DATETIME_H
