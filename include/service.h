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
    Service(ServiceType type, Date date, std::string worker, Plane& plane);
    ServiceType getType() const;
    Date getDate() const;
    std::string getWorker() const;
    Plane& getPlane();
};

#endif //AIRLINE_MAINTENANCE_H
