#include "service.h"

Service::Service(const ServiceType &type, const Datetime &datetime, const std::string &worker, Plane& plane): datetime(datetime), plane(plane) {
    this->type = type;
    this->worker = worker;
}

ServiceType Service::getType() const {
    return this->type;
}

Datetime Service::getDatetime() const {
    return this->datetime;
}

std::string Service::getWorker() const {
    return this->worker;
}

Plane& Service::getPlane() const {
    return this->plane;
}


std::ostream& operator<<(std::ostream &out, const Service &service) {
    if (service.getType() == ServiceType::CLEANING)
        out << "CLEANING\n";
    else
        out << "MAINTENANCE\n";

    out << "At " << service.getDatetime().str() << '\n'
        << "By " << service.getWorker() << '\n';

    return out;
}