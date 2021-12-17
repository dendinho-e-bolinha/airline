#include "airport.h"

using namespace std;

const set<TransportPlace> &Airport::getTransportPlaceInfo() const {
    return transportPlaceInfo;
}

void Airport::setTransportPlaceInfo(const set<TransportPlace> &transportPlaceInfo) {
    Airport::transportPlaceInfo = transportPlaceInfo;
}

Airport::Airport(const set<TransportPlace> &transportPlaceInfo) : transportPlaceInfo(transportPlaceInfo) {}

bool TransportPlace::operator<(const TransportPlace &rhs) const {
    if (this->transportType == rhs.transportType) {
        if (this->airportDistance == rhs.airportDistance) {
            return this->name < rhs.name;
        }

        return this->airportDistance < rhs.airportDistance;
    }

    return this->transportType < rhs.transportType;
}
