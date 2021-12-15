#include "ticket.h"

Ticket::Ticket(Flight &flight, unsigned int seat_number) : flight(flight), seat_number(seat_number) {}

Flight &Ticket::get_flight() const {
    return this->flight;
}

unsigned int Ticket::get_seat_number() const {
    return this->seat_number;
}
