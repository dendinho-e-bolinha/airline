#include "flight.h"

using namespace std;

Flight::Flight(string &id, Datetime &departure, Time &duration, Airport &origin, Airport &destination, Plane &plane) {
    this->flight_id =id;
    this->departure = departure;
    this->duration = duration;
    this->origin = origin;
    this->destination = destination;
    this->plane = plane;
}

std::string Flight::getFlightId() const {
    return this->flight_id;
}

Datetime Flight::getDepartureTime() const {
    return this->departure;
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

bool Flight::removeFirstTicket(const std::function<bool(const int &)> selector) {
    vector<Ticket*>::iterator it = this->tickets.begin();
    while (it != this->tickets.end()) {
        if (selector(**it)) {
            it = this->tickets.erase(it);
            return true;
        }
        it++;
    }
    return false;
}

bool Flight::removeAllTickets(const std::function<bool(const int &)> selector) {
    vector<Ticket*>::iterator it = this->tickets.begin();
    bool removed_any;
    while (it != this->tickets.end()) {
        if (selector(**it)) {
            it = this->tickets.erase(it);
            removed_any = true;
        }
        it++;
    }
    return removed_any;
}
