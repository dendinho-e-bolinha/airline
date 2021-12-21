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

    std::set<TransportPlace> getTransportPlaceInfo() const;
    const std::string &getName() const;

    // Setters

    /**
     * @brief Adds the given TransportPlace to `transport_place_info`
     * @param transportPlace A TransportPlace instance
     */
    void addTransportPlaceInfo(TransportPlace transportPlace);


    /**
     * @brief Removes all elements from the transport_place_info
     */
    void removeAllTransportPlaceInfo();

    /**
     * @brief Removes all elements with a certain name from transport_place_info
     * 
     * @param name The name of the transport place infos to be removed
     */
    void removeTransportPlaceInfo(const std::string &name);

    /**
     * @overload Displays a plane
     */
    friend std::ostream &operator<<(std::ostream &os, const Airport &airport);
    std::string str() const;
};


#endif //AIRLINE_AIRPORT_H
