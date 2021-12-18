#ifndef AIRLINE_STATE_H
#define AIRLINE_STATE_H

#include <vector>
#include "plane.h"
#include "flight.h"
#include "ticket.h"
#include "airport.h"
#include "handling_car.h"

class ComparePointers {
    
    template<class T>
    bool operator()(const T *&lhs, const T *&rhs) {
        return lhs < rhs;
    }
};

namespace data {
    static std::vector<Plane*> planes;
    static std::vector<Flight*> flights;
    static std::vector<Ticket*> tickets;
    static std::vector<HandlingCar*> handlingCars;
    static std::vector<Airport*> airports;
}

#endif //AIRLINE_STATE_H