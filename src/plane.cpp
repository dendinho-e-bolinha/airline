#include "plane.h"
#include "algorithm"

using namespace std;

Plane::Plane(string &plate, string &type, unsigned int &capacity) {
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

vector<Flight*> Plane::getFlights() const {
    return this->flights;
}

queue<Service*> Plane::getServices() const {
    return this->services;
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
    auto it = this->flights.begin();
    while (it != this->flights.end()) {
        if (selector(**it)) {
            it = this->flights.erase(it);
            return true;
        }
        it++;
    }
    return false;
}

bool Plane::removeAllFlights(const function<bool(const Flight&)>& selector) {
    auto it = this->flights.begin();
    bool removed_any= false;
    while (it != this->flights.end()) {
        if (selector(**it)) {
            it = this->flights.erase(it);
            removed_any = true;
        }
        it++;
    }
    return removed_any;
}

void Plane::scheduleService(Service& service) {
    this->services.push(&service);
}

bool Plane::completeService() {
    if (this->services.empty()) {
        return false;
    }
    this->finished_services.push_back(this->services.front());
    this->services.pop();
    return true;
}
