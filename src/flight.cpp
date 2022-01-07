#include "flight.h"
#include "utils.h"
#include <sstream>

using namespace std;

Flight::Flight(const string &id, const Datetime &departure_time, const Time &duration, Airport &origin, Airport &destination,
               Plane &plane) : plane(plane), flight_id(id), departure_time(departure_time),
                               duration(duration), origin(origin), destination(destination) {}

std::string Flight::getFlightId() const {
    return this->flight_id;
}

Datetime Flight::getDepartureTime() const {
    return this->departure_time;
}

Time Flight::getDuration() const {
    return this->duration;
}

Airport &Flight::getOrigin() const {
    return this->origin;
}

Airport &Flight::getDestination() const {
    return this->destination;
}

vector<Ticket *> Flight::getTickets() const {
    return this->tickets;
}

Plane &Flight::getPlane() const {
    return this->plane;
}

void Flight::addLuggage(Luggage &luggage) {
    this->luggage.push_back(&luggage);
}

vector<Luggage*> Flight::getLuggage() const {
    return this->luggage;
}

string Flight::str() const {
    ostringstream out;
    out << "Flight ID: " << this->getFlightId() << '\n'  
        << "At: " << this->getDepartureTime().str() << '\n'
        << "Duration: " << this->getDuration().str() << '\n'
        << "From " << this->getOrigin().getName() << " to " << this->getDestination().getName();

    return out.str();
}

ostream& operator<<(ostream &out, const Flight &flight) {
    return out << flight.str() << endl;
}


bool Flight::addTicket(Ticket &ticket) {
    if (this->tickets.size() < this->plane.getCapacity()) {
        auto it = utils::lowerBound<Ticket*, unsigned int>(this->tickets, ticket.getSeatNumber(), [](Ticket* ticket) {
            return ticket->getSeatNumber();
        });

        this->tickets.insert(it, &ticket);
        return true;
    }

    return false;
}

bool Flight::removeTicket(const Ticket &ticket) {
    auto it = find(this->tickets.begin(), this->tickets.end(), &ticket);
    if (it != this->tickets.end()) {
        this->tickets.erase(it);
        return true;
    }
    return false;
}

bool Flight::removeFirstTicket(const std::function<bool(const Ticket &)> &selector) {
    // FIXME
    for (auto it = tickets.begin(), end = tickets.end(); it != end; it++) {
        if (selector(**it)) {
            it = this->tickets.erase(it);
            return true;
        }
    }
    return false;
}


void Flight::setDepartureTime(Datetime &datetime) {
    this->departure_time = datetime;
}

void Flight::setDuration(Time &duration) {
    this->duration = duration;
}

void Flight::setOrigin(Airport &origin) {
    this->origin = origin;
}

void Flight::setDestination(Airport &destination) {
    this->destination = destination;
}

void Flight::clearTickets() {
    this->tickets.clear();
}
