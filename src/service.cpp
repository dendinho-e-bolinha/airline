#include "service.h"

Service::Service(ServiceType type, Date date, std::string worker, Plane& plane) {
    this->type = type;
    this->date = date;
    this->worker = worker;
    this->plane = plane;
}

ServiceType Service::getType() const {
    return this->type;
}

Date Service::getDate() const {
    return this->date;
}

std::string Service::getWorker() const {
    return this->worker;
}

Plane& Service::getPlane() const {
    return this->plane;
}
