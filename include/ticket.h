#ifndef AIRLINE_TICKET_H
#define AIRLINE_TICKET_H

class Ticket;

#include <list>
#include "flight.h"
#include "luggage.h"

class Ticket {
private:
    Flight &flight;
    unsigned int seat_number;

    std::list<Luggage*> luggage;

public:
    Ticket(Flight &flight, unsigned int seat_number);
    unsigned int getSeatNumber() const;
    Flight &getFlight();

    void addLuggage(Luggage *luggage);
};

#endif // AIRLINE_TICKET_H
