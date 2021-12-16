#ifndef AIRLINE_MAINTENANCE_H
#define AIRLINE_MAINTENANCE_H

#include <string>
#include "datetime.h"
#include "plane.h"

enum ServiceType {
    MAINTENANCE,
    CLEANING
};

class Service {
    ServiceType type;
    Datetime datetime;
    std::string worker;
    Plane& plane;

public:
    /**
     * @brief Creates an object of type Service with the given attributes
     *
     * @param type The service's type
     * @param date The service's scheduled date
     * @param worker The worker that will perform the service
     * @param plane The plane where the service will take place
     */
    Service(const ServiceType &type, const Datetime &datetime, const std::string &worker, Plane &plane);
    ServiceType getType() const;
    Datetime getDatetime() const;
    std::string getWorker() const;
    Plane& getPlane() const;

    /**
     * @brief Displays data from a Service instance
     * @param out Ostream
     * @param s Service instance
     * @return Returns the reference to the ostream
     */
    std::ostream &operator << (std::ostream &out, const Service &s);
};

#endif //AIRLINE_MAINTENANCE_H
