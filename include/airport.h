#ifndef AIRLINE_AIRPORT_H
#define AIRLINE_AIRPORT_H

#include <set>
#include <string>

#include "datetime.h"

enum TransportType {
    SUBWAY,
    BUS,
    TRAIN
};

struct TransportPlace {
    std::string name;
    float latitude, longitude;
    TransportType transportType;
    float airportDistance;
    std::set<Datetime> schedule;

    /**
     * @overload Operator <
     * @param rhs
     * @return
     */
    bool operator<(const TransportPlace &rhs) const;
};


class Airport {
    std::set<TransportPlace> transportPlaceInfo;
public:

    Airport(const std::set<TransportPlace> &transportPlaceInfo);

    /**
     * @return Returns a set containing each transport of the
     */
    const std::set<TransportPlace> &getTransportPlaceInfo() const;


    /**
     * @return Returns a set containing each transport of the
     */
    void setTransportPlaceInfo(const std::set<TransportPlace> &transportPlaceInfo);

    /**
     * @brief Adds a TransportPlace to `transportPlaceInfo`
     * @param transportPlace TransportPlace
     */
    void addTransportPlaceInfo(TransportPlace transportPlace) {
        this->transportPlaceInfo.insert(transportPlace);
    }
};


#endif //AIRLINE_AIRPORT_H
