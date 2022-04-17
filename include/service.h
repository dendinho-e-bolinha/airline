#pragma once

class Service;
enum class ServiceType;

#include <string>
#include "datetime.h"
#include "plane.h"

enum class ServiceType {
    MAINTENANCE = 0,
    CLEANING = 1
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

    // Getters

    ServiceType getType() const;
    Datetime getDatetime() const;
    std::string getWorker() const;
    Plane& getPlane() const;

    /**
     * @overload Displays a Service instance
     */
    friend std::ostream& operator<<(std::ostream &out, const Service &service);
};