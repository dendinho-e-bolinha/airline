#ifndef PROJ_1_PLANE_H
#define PROJ_1_PLANE_H

#include <string>
#include <list>
#include "Flight.h"

class Plane {
    string plate;
    string type;
    unsigned int capacity;
    List<Flight> flights;

public:
        Plane();
        //plane só com 3?
        Plane(string plate, string type, unsigned int capacity, List<Flight> flights);
        string getPlate();
        string getType();
        unsigned int getCapacity();
        List<Flight> getFlights();

};

#endif //PROJ_1_PLANE_H
