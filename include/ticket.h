#ifndef AIRLINE_TICKET_H
#define AIRLINE_TICKET_H

class Ticket;

#include <list>
#include "flight.h"
#include "luggage.h"
#include "customer.h"

class Ticket {
private:
    Flight &flight;
    Customer &customer;
    unsigned int seat_number;

public:
    /**
     * Creates a ticket for a given flight
     * @param flight The flight this ticket has been issued for
     * @param customer The customer that has bought this ticket
     * @param seat_number The seat number for this ticket
     */
    Ticket(Flight &flight, Customer &customer, unsigned int seat_number);
    unsigned int getSeatNumber() const;
    Flight &getFlight();
};

#endif // AIRLINE_TICKET_H
