#ifndef AIRLINE_FLIGHT_H
#define AIRLINE_FLIGHT_H

#include <string>
#include <vector>
#include "datetime.h"
#include "plane.h"
#include "ticket.h"
#include "functional"

class Flight {
    Plane& plane;
    std::string flight_id;
    Datetime departure;
    Time duration;
    Airport& origin;
    Airport& destination;
    std::vector<Ticket*> tickets;

public:
    Flight(std::string &id, Datetime &departure, Time &duration, Airport &origin, Airport &destination, Plane& plane);
    std::string getFlightId() const;
    Datetime getDepartureTime() const;
    Time getDuration() const;
    Airport& getOrigin() const;
    Airport& getDestination() const;
    std::vector<Ticket> getTickets() const;
    void addTicket(Ticket& ticket);
    bool removeTicket(const Ticket& ticket);
    bool removeFirstTicket(const std::function <bool (const Ticket&)> selector);
    bool removeAllTickets(const std::function <bool (const Ticket&)> selector);
    bool addTicket(Ticket &ticket);
};

#endif //AIRLINE_FLIGHT_H
