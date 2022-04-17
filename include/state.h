#pragma once

#include <vector>
#include "plane.h"
#include "flight.h"
#include "ticket.h"
#include "airport.h"
#include "handling_car.h"

/** Data that is stored during the runtime of the application */
namespace data {
    static std::vector<Plane*> planes;
    static std::vector<Flight*> flights;
    static std::vector<HandlingCar*> handlingCars;
    static std::vector<Airport*> airports;
}