#include "airport.h"
#include <iostream>
#include <sstream>

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
    os << airport.str() << endl;
    
    return os;
}

string Airport::str() const {
    ostringstream os;

    auto aux = this->getTransportPlaceInfo();
    os << "Name:" << this->getName() << endl;
    os << '{' << endl;

    bool is_first = true;
    for (const auto &c : aux) {
        if (!is_first)
            cout << '\n';

        os << "\tDistance: " << c.airport_distance
             << "\tCoordinates: (" << c.latitude << ", " << c.longitude << ")" << endl
             << "\tName: " << c.name
             << "\tTransport type: " << c.transport_type << endl;

        is_first = false;
    }

    os << '}';

    return os.str();
}