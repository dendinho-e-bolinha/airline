#include "airport.h"
#include <iostream>
#include <sstream>

using namespace std;

set<TransportPlace> Airport::getTransportPlaceInfo() const {
    return transport_place_info;
}

void Airport::addTransportPlaceInfo(TransportPlace transportPlace)  {
    this->transport_place_info.insert(transportPlace);
}

void Airport::removeAllTransportPlaceInfo() {
    this->transport_place_info.clear();
}

void Airport::removeTransportPlaceInfo(const string &name) {
    for (auto it = this->transport_place_info.begin(); it != this->transport_place_info.end(); it++) {
        if (it->name == name) {
            this->transport_place_info.erase(it);
            break;
        }
    }
}

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

ostream &operator<<(ostream &os, const Airport &airport) {
    os << airport.str() << endl;
    
    return os;
}

string Airport::str() const {
    ostringstream os;
    os << "Name: " << this->name;

    return os.str();
}