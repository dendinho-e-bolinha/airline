#include "service.h"

Service::Service(const ServiceType &type, const Date &date, const std::string &worker, Plane& plane): date(date), plane(plane) {
    this->type = type;
    this->worker = worker;
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
