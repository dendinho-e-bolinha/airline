#include "flight.h"

using namespace std;

Flight::Flight() {}

Flight::Flight(int id, unsigned int date, unsigned int duration, unsigned int seats, string origin, string destiny) {
    this->flight_id =id;
    this->date = date;
    this->duration = duration;
    this->seats = seats;
    this->origin= origin;
    this->destiny= destiny;
}

int Flight::getFlightId() {
    return this->flight_id;
}

unsigned int Flight::getDate() {
    return this->date;
}

unsigned int Flight::getDuration() {
    return this->duration;
}

string Flight::getOrigin() {
    return this->origin;
}

string Flight::getDestiny() {
    return this->destiny;
}

void Flight::setDate(unsigned int date) {
    this->date = date;
}

void Flight::setDuration(unsigned int duration) {
    this->duration = duration;
}

void Flight::setSeats(unsigned int seats) {
    this->seats = seats;
}

void Flight::setOrigin(std::string origin) {
    this->origin = origin;
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