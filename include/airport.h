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
    std::string name;
    std::set<TransportPlace> transportPlaceInfo;
public:

    Airport(const std::string &name);


    /**
     * @return Returns a set containing each transport of the
     */
    const std::set<TransportPlace> &getTransportPlaceInfo() const;

    const std::string &getName() const;

    void setName(std::string name);

    /**
     * @brief Adds a TransportPlace to `transportPlaceInfo`
     * @param transportPlace TransportPlace
     */
    void addTransportPlaceInfo(TransportPlace transportPlace) {
        this->transportPlaceInfo.insert(transportPlace);
    }

    /**
     * @overload Displays a plane
     * @param os
     * @param airport
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, const Airport &airport);

};


#endif //AIRLINE_AIRPORT_H
