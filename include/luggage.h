#ifndef AIRLINE_LUGGAGE_H
#define AIRLINE_LUGGAGE_H

class Luggage;

#include "ticket.h"

class Luggage {
    Ticket &ticket;
    float weight;

public:
    /**
     * @brief Creates a Luggage instance
     * @param ticket Ticket instance
     * @param weight The weight of the Luggage
     */
    Luggage(Ticket &ticket, float weight);

    // Getters

    Ticket &getTicket();
    float const &getWeight() const;
};

#endif // AIRLINE_LUGGAGE_H
