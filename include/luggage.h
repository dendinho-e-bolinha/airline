#ifndef AIRLINE_LUGGAGE_H
#define AIRLINE_LUGGAGE_H

class Luggage;

#include "ticket.h"

class Luggage {
    Ticket &ticket;
    float weight;

public:
    Luggage(Ticket &ticket, float weight);

    Ticket &getTicket();
    float const &getWeight() const;
};

#endif // AIRLINE_LUGGAGE_H
