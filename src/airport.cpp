#include "airport.h"
#include <iostream>

using namespace std;

const set<TransportPlace> &Airport::getTransportPlaceInfo() const {
    return transport_place_info;
}

// void Airport::settransport_place_info(const set<TransportPlace> &transport_place_info) {
    // Airport::transport_place_info = transport_place_info;
// }

Airport::Airport(const string &name): name(name) {}

bool TransportPlace::operator<(const TransportPlace &rhs) const {
    if (this->transport_type == rhs.transport_type) {
        if (this->airport_distance == rhs.airport_distance) {
            return this->name < rhs.name;
        }

        return this->airport_distance < rhs.airport_distance;
    }

    return this->transport_type < rhs.transport_type;
}

const std::string &Airport::getName() const {
    return name;
}

void Airport::setName(std::string name) {
   this->name = name;
}


ostream &operator<<(ostream &os, const Airport &airport) {
    auto aux = airport.getTransportPlaceInfo();
    os << "Name:" << airport.getName() << endl;

    for (const auto &c : airport.transport_place_info) {
        os << "Distance: " << c.airport_distance
             << "Coordinates: (" << c.latitude << ", " << c.longitude << ")" << endl
             << "Name: " << c.name
             << "Transport type: " << c.transport_type << endl;
    }
    
    return os;
}