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
    Date date;
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
    Service(const ServiceType &type, const Date &date, const std::string &worker, Plane &plane);
    ServiceType getType() const;
    Date getDate() const;
    std::string getWorker() const;
    Plane& getPlane() const;
};

#endif //AIRLINE_MAINTENANCE_H
