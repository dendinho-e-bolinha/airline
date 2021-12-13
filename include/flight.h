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
    /**
     * @brief Creates an object of type Flight with given attributes
     *
     * @param id The flight's id
     * @param departure The flight's departure location
     * @param duration The flight's duration
     * @param origin The flight's origin location
     * @param destination The flight's destination location
     * @param plane The plane the flight will take place on
     */
    Flight(std::string &id, Datetime &departure, Time &duration, Airport &origin, Airport &destination, Plane& plane);

    std::string getFlightId() const;
    Datetime getDepartureTime() const;
    Time getDuration() const;
    Airport& getOrigin() const;
    Airport& getDestination() const;
    std::vector<Ticket> getTickets() const;

    /**
     * @brief Adds a ticket to the flight's purchased tickets
     * @param ticket A flight's ticket
     *
     * @return True, if a ticket was successfully purchased; false, if a ticket couldn't be bought
     */
    bool addTicket(Ticket& ticket);

    /**
     * @brief Removes a ticket from the flight's purchased tickets
     * @param ticket A flight's ticket
     *
     * @return true, if the ticket was successfully removed from the flight's bought tickets; false, if the ticket couldn't be removed
     */
    bool removeTicket(const Ticket& ticket);

    /**
     * @brief Removes the first ticket already purchased for the flight that fulfills the removal condition
     * @param selector A selection method to choose the ticket to remove
     *
     * @return true, if at least one ticket was removed; false, if no tickets were removed
     */
    bool removeFirstTicket(const std::function <bool (const Ticket&)>& selector);

    /**
     * @brief Removes all the tickets already purchased for a flight that fulfill the removal condition
     * @param selector A selection method to choose the tickets to remove
     *
     * @return true, if at least one ticket was removed; false, if no tickets were removed
     */
    bool removeAllTickets(const std::function <bool (const Ticket&)>& selector);
};

#endif //AIRLINE_FLIGHT_H
