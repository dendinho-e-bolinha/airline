#include "service.h"
#include <iomanip>
#include <iostream>

using namespace std;

Service::Service(const ServiceType &type, const Datetime &datetime, const string &worker, Plane& plane) {
    this->type = type;
    this->datetime = datetime;
    this->worker = worker;
    this->plane = plane;
}

ServiceType Service::getType() const {
    return this->type;
}

Datetime Service::getDatetime() const {
    return this->datetime;
}

string Service::getWorker() const {
    return this->worker;
}

Plane& Service::getPlane() const {
    return this->plane;
}

ostream &Service::operator << (std::ostream &out, const Service &s) {  //meti o 3 porque se quiserem fazeruma funcao que leia tudo e precise do numero de linhas que vai ter a seguir
    out << '3' << setw(5) << setfill(' ')
        << to_string(s.getType()) << ' '
        << Datetime::toString(s.getDatetime()) << ' '
        << s.getWorker() << endl;
}

istream
