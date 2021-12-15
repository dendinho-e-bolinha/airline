#include "flight.h"

using namespace std;

Flight::Flight(string &id, Datetime &departure_time, Time &duration, Airport &origin, Airport &destination, Plane &plane) {
    this->flight_id = id;
    this->departure_time = departure_time;
    this->duration = duration;
    this->origin = origin;
    this->destination = destination;
    this->plane = plane;
}

std::string Flight::getFlightId() const {
    return this->flight_id;
}

Datetime Flight::getDepartureTime() const {
    return this->departure_time;
}

Time Flight::getDuration() const {
    return this->duration;
}

Airport& Flight::getOrigin() const {
    return this->origin;
}

Airport& Flight::getDestination() const {
    return this->destination;
}

vector<Ticket*> Flight::getTickets() const {
    return this->tickets;
}

Plane& Flight::getPlane() const {
    return this->plane;
}

bool Flight::addTicket(int &ticket) {
    if (this->tickets.size() < this->plane.getCapacity()) {
        this->tickets.push_back(ticket);
        return true;
    }
    return false;
}

bool Flight::removeTicket(const int &ticket) {
    auto it = find(this->tickets.begin(), this->tickets.end(), &ticket);
    if (it != this->tickets.end()) {
        this->tickets.erase(it);
        return true;
    }
    return false;
}

bool Flight::removeFirstTicket(const std::function<bool(const int &)> &selector) {
    for (auto it = tickets.begin(), end = tickets.end(); it != end; it++) {
        if (selector(**it)) {
            it = this->tickets.erase(it);
            return true;
        }
    }
    return false;
}

bool Flight::removeAllTickets(const std::function<bool(const int &)> &selector) {
    bool removed_any = false;
    for (auto it = tickets.begin(), end = tickets.end(); it != end; it++) {
        if (selector(**it)) {
            it = this->tickets.erase(it);
            removed_any = true;
        }
    }
    return removed_any;
}
