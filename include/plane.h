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
    std::vector<Flight*> flights;
    std::queue<Service*> services;
    std::vector<Service*> finished_services;

public:
        Plane(std::string &plate, std::string &type, unsigned int &capacity);
        std::string getPlate() const;
        std::string getType() const;
        unsigned int getCapacity() const;
        std::vector<Flight*> getFlights() const;
        std::queue<Service*> getServices() const;
        std::vector<Service*> getFinishedServices() const;
        void emptyMaintenance();
        void popFrontMaintenance();  //bool??
        void addFlight(Flight &flight);
        bool removeFlight(const Flight &flight);
        bool removeFirstFlight(const std::function <bool (const Flight&)> selector);
        bool removeAllFlights(const std::function <bool (const Flight&)> selector);
        void scheduleService(Service& service);
        bool completeService();
};

#endif //AIRLINE_PLANE_H
