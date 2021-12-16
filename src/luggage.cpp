#include "luggage.h"

Luggage::Luggage(Ticket &ticket, float weight) : ticket(ticket), weight(weight) {}

Ticket &Luggage::getTicket() {
    return this->ticket;
}

float const &Luggage::getWeight() const {
    return this->weight;
}
