#ifndef PROJ_1_FLIGHT_H
#define PROJ_1_FLIGHT_H

#include <string>

class Flight {
    int flight_id;
    unsigned int date; //(?) date.h?
    unsigned int duration;
    unsigned int seats;
    string origin;
    string destiny;

public:
    Flight();
    Flight(int id, unsigned int date, unsigned int duration, unsigned int seats, string origin, string destiny);
    int getFlightId();
    unsigned int getDate();
    unsigned int getDuration();
    string getOrigin();
    string getDestiny();
    void setDate(unsigned int date);
    void setDuration(unsigned int duration);
    void setSeats(unsigned int seats);
    void setOrigin(string origin);
    void setDestiny(string destiny);
    void freeSeat();
    void takeSeat();
};

#endif //PROJ_1_FLIGHT_H
