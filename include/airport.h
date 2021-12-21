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
    std::set<Time> schedule;

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

    /**
     * @brief Creates an Airport instance
     * @param name Airport name
     */
    Airport(const std::string &name);

    // Getters

    const std::set<TransportPlace> &getTransportPlaceInfo() const;
    const std::string &getName() const;

    // Setters

    void setName(std::string name);

    /**
     * @brief Adds the given TransportPlace to `transport_place_info`
     * @param transportPlace A TransportPlac instance
     */
    void addTransportPlaceInfo(TransportPlace transportPlace) {
        this->transport_place_info.insert(transportPlace);
    }

    /**
     * @overload Displays a plane
     */
    friend std::ostream &operator<<(std::ostream &os, const Airport &airport);
    std::string str() const;
};


#endif //AIRLINE_AIRPORT_H
