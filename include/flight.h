#ifndef AIRLINE_FLIGHT_H
#define AIRLINE_FLIGHT_H

class Flight;

#include <string>
#include <vector>
#include "datetime.h"
#include "plane.h"
#include "ticket.h"
#include "airport.h"
#include "luggage.h"
#include <functional>
#include <algorithm>

class Flight {
    Plane& plane;
    std::string flight_id;
    Datetime departure_time;
    Time duration;
    Airport& origin;
    Airport& destination;
    std::vector<Ticket*> tickets;
    std::vector<Luggage*> luggage;

public:
    /**
     * @brief Creates an object of type Flight with given attributes
     *
     * @param id The flight's id
     * @param departure_time The flight's departure timestamp
     * @param duration The flight's duration
     * @param origin The flight's origin
     * @param destination The flight's destination
     * @param plane A flight performed by the plane
     */
    Flight(const std::string &id, const Datetime &departure_time, const Time &duration, Airport &origin, Airport &destination, Plane& plane);

    std::string getFlightId() const;
    Datetime getDepartureTime() const;
    Time getDuration() const;
    Airport& getOrigin() const;
    Airport& getDestination() const;
    std::vector<Ticket*>& getTickets();
    Plane& getPlane() const;

    std::string str() const;

    /**
     * @brief Adds a ticket to the flight's purchased tickets
     * @param ticket A flight's ticket
     *
     * @return true, if there was room on the flight for another passenger; false, otherwise
     */
    bool addTicket(Ticket &ticket);

    /**
     * @brief Removes a ticket from the flight's purchased tickets
     * @param ticket A flight's ticket
     *
     * @return true, if the ticket was on this flight's purchased tickets; false otherwise
     */
    bool removeTicket(const Ticket &ticket);

    /**
     * @brief Removes the first ticket already purchased for the flight that fulfills the removal condition
     * @param selector A selection method to choose the ticket to remove
     *
     * @return true, if at least one ticket was removed; false, if no tickets were removed
     */
    bool removeFirstTicket(const std::function <bool (const Ticket&)> &selector);

    /**
     * @brief Removes all the tickets already purchased for a flight that fulfill the removal condition
     * @param selector A selection method to choose the tickets to remove
     *
     * @return true, if at least one ticket was removed; false, if no tickets were removed
     */
    bool removeAllTickets(const std::function <bool (const Ticket&)> &selector);

    friend std::ostream& operator<<(std::ostream &out, const Flight &flight);
};

#endif //AIRLINE_FLIGHT_H
