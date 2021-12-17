#include "airport.h"

const std::set<TransportPlace> &Airport::getTransportPlaceInfo() const {
    return transportPlaceInfo;
}

void Airport::setTransportPlaceInfo(const std::set<TransportPlace> &transportPlaceInfo) {
    Airport::transportPlaceInfo = transportPlaceInfo;
}

Airport::Airport(const std::set<TransportPlace> &transportPlaceInfo) : transportPlaceInfo(transportPlaceInfo) {}

bool TransportPlace::operator<(const TransportPlace &rhs) const {
    if (this->transportType == rhs.transportType) {
        if (this->airportDistance == rhs.airportDistance) {
            return this->name < rhs.name;
        }

        return this->airportDistance < rhs.airportDistance;
    }

    return this->transportType < rhs.transportType;
}
