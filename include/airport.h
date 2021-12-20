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
    TransportType transport_type;
    float airport_distance;
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
    std::set<TransportPlace> transport_place_info;
public:

    Airport(const std::string &name);


    /**
     * @return Returns a set containing each means of transport on the airport
     */
    const std::set<TransportPlace> &getTransportPlaceInfo() const;

    const std::string &getName() const;

    void setName(std::string name);

    /**
     * @brief Adds the given TransportPlace to `transport_place_info`
     * @param transportPlace A TransportPlac instance
     */
    void addTransportPlaceInfo(TransportPlace transportPlace) {
        this->transport_place_info.insert(transportPlace);
    }

    std::string str() const;

    /**
     * @overload Displays a plane
     * @param os
     * @param airport
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, const Airport &airport);

};


#endif //AIRLINE_AIRPORT_H
