#include "plane.h"
#include <algorithm>

using namespace std;

Plane::Plane(const string &plate, const string &type, const unsigned int capacity) {
    this->plate = plate;
    this->type= type;
    this->capacity = capacity;
}

string Plane::getPlate() const {
    return this->plate;
}

string Plane::getType() const {
    return this->type;
}

unsigned int Plane::getCapacity() const {
    return this->capacity;
}

list<Flight*> Plane::getFlights() const {
    return this->flights;
}

queue<Service*> Plane::getScheduledServices() const {
    return this->scheduled_services;
}

vector<Service*> Plane::getFinishedServices() const {
    return this->finished_services;
}

void Plane::addFlight(Flight &flight) {
    this->flights.push_back(&flight);
}

bool Plane::removeFlight(const Flight &flight) {
    auto it = find(this->flights.begin(), this->flights.end(), &flight);
    if (it != this->flights.end()) {
        this->flights.erase(it);
        return true;
    }
    return false;
}

bool Plane::removeFirstFlight(const std::function<bool(const Flight &)>& selector) {
    // FIXME
    for (auto it = flights.begin(), end = flights.end(); it != end; it++) {
        if (selector(**it)) {
            it = this->flights.erase(it);
            return true;
        }
    }
    return false;
}

bool Plane::removeAllFlights(const function<bool(const Flight&)>& selector) {
    // FIXME
    bool removed_any = false;
    for (auto it = flights.begin(), end = flights.end(); it != end; it++) {
        if (selector(**it)) {
            it = this->flights.erase(it);
            removed_any = true;
        }
        return removed_any;
    }
}

void Plane::scheduleService(Service& service) {
    this->scheduled_services.push(&service);
}

bool Plane::completeService() {
    if (this->scheduled_services.empty()) {
        return false;
    }
    this->finished_services.push_back(this->scheduled_services.front());
    this->scheduled_services.pop();
    return true;
}
