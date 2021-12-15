#ifndef AIRLINE_TICKET_H
#define AIRLINE_TICKET_H

#include "flight.h"

class Ticket {
private:
    Flight &flight;
    unsigned int seat_number;

public:
    Ticket(Flight &flight, unsigned int seat_number);
    unsigned int get_seat_number() const;
    Flight &get_flight() const;
};

#endif // AIRLINE_TICKET_H
