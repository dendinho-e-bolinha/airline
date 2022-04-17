#pragma once

#include <functional>
#include <sstream>
#include "handling_car.h"
#include "airport.h"
#include "flight.h"
#include "airport.h"
#include "plane.h"
#include "ticket.h"
#include "service.h"

namespace files {
    void read();
    void write();
}

namespace crud {
    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of planes
    */
    void managePlanes();

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of flights
    */
    void manageFlights();
    
    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of tickets
    */
    void manageTickets(Flight &flight);

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update the airport
    */
    void manageAirports();

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of handling carts
    */
    void manageHandlingCars();

    /**
     * @brief Creates the filters for the Plane
     */
    std::function<bool(const Plane* const&)> createPlaneFilter(std::ostringstream &repr);

    /**
     * @brief Creates the filters for the Flight
     */
    std::function<bool(const Flight* const&)> createFlightFilter(std::ostringstream &repr);

    /**
     * @brief Creates the filters for the Ticket
     */
    std::function<bool(const Ticket* const&)> createTicketFilter(std::ostringstream &repr);

    /**
     * @brief Creates the filters for the Airport
     */
    std::function<bool(const Airport* const&)> createAirportFilter(std::ostringstream &repr);

    /**
     * @brief Creates the filters for the HandlingCar
     */
    std::function<bool(const HandlingCar* const&)> createCarFilter(std::ostringstream &repr);

    /**
     * @brief Creates the filters for the Service
     */
    std::function<bool(const Service* const&)> createServiceFilter(std::ostringstream &repr);
}