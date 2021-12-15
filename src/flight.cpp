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

void Flight::setDestiny(std::string destiny) {
    this->destiny = destiny;
}

void Flight::freeSeat() {
    this->seats++;
}

void Flight::takeSeat() {
    this->seats--;
}