#include "ticket.h"

Ticket::Ticket(Flight &flight, Customer &customer, unsigned int seat_number) : flight(flight), seat_number(seat_number), customer(customer) {}

Flight &Ticket::getFlight() {
    return this->flight;
}

unsigned int Ticket::getSeatNumber() const {
    return this->seat_number;
}

//void Ticket::addLuggage(Luggage *luggage) {
//    this->luggage.push_back(luggage);
//}
