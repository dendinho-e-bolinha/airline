#include "airport.h"
#include <iostream>

using namespace std;

const set<TransportPlace> &Airport::getTransportPlaceInfo() const {
    return transportPlaceInfo;
}

// void Airport::setTransportPlaceInfo(const set<TransportPlace> &transportPlaceInfo) {
    // Airport::transportPlaceInfo = transportPlaceInfo;
// }

Airport::Airport(const string &name): name(name) {}

bool TransportPlace::operator<(const TransportPlace &rhs) const {
    if (this->transportType == rhs.transportType) {
        if (this->airportDistance == rhs.airportDistance) {
            return this->name < rhs.name;
        }

        return this->airportDistance < rhs.airportDistance;
    }

    return this->transportType < rhs.transportType;
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

    for (const auto &c : airport.transportPlaceInfo) {
        os << "Distance: " << c.airportDistance
             << "Coordinates: (" << c.latitude << ", " << c.longitude << ")" << endl
             << "Name: " << c.name
             << "Transport type: " << c.transportType << endl;
    }
    
    return os;
}