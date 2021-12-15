#ifndef AIRLINE_PLANE_H
#define AIRLINE_PLANE_H

#include <string>
#include <vector>
#include <queue>
#include "flight.h"
#include "service.h"
#include <functional>

class Plane {
    std::string plate;
    std::string type;
    unsigned int capacity;
    std::list<Flight*> flights;
    std::queue<Service*> scheduled_services;
    std::vector<Service*> finished_services;

public:

    /**
     * @brief Creates an object of type Plane with the given attributes
     * @param plate The plane's license plate
     * @param type The plane's type
     * @param capacity The plane's capacity
     */
    Plane(const std::string &plate, const std::string &type, const unsigned int capacity);

    std::string getPlate() const;
    std::string getType() const;
    unsigned int getCapacity() const;
    std::vector<Flight*> getFlights() const;
    std::queue<Service*> getScheduledServices() const;
    std::vector<Service*> getFinishedServices() const;

    /**
     * @brief Adds a flight to the planes scheduled flights
     * @param flight A flight performed by the plane
     */
    void addFlight(Flight &flight);

    /**
     * @brief Removes a flight from the planes scheduled flights
     * @param flight A flight
     *
     * @return true, if the flight was found and removed; false, if the flight wasn't removed
     */
    bool removeFlight(const Flight &flight);

    /**
     * @brief Removes the first flight scheduled for the plane that fulfills the removal condition
     * @param selector A selection method to choose the flight to remove
     *
     * @return true, if a flight was removed; false, if no flights were removed
     */
    bool removeFirstFlight(const std::function <bool (const Flight&)> &selector);

    /**
     * @brief Removes all flights scheduled for the plane that fulfill the removal condition
     * @param selector A selection method to choose the flight to remove
     *
     * @return true, if at least one flight was removed; false, if no flights were removed
     */
    bool removeAllFlights(const std::function <bool (const Flight&)> &selector);

    /**
     * @brief Schedule's a new service for the plane
     * @param service A service to schedule for the plane
     */
    void scheduleService(Service &service);

    /**
     * @brief Removes the most recently finished maintenance on the plane and adds a log to the finished maintenances
     * @return true, if a service was completed; false, if there were no services scheduled for the plane
     */
    bool completeService();
};

#endif //AIRLINE_PLANE_H
