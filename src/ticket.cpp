class Flight;

#include "ticket.h"
#include "flight.h"
#include <sstream>

using namespace std;

Ticket::Ticket(Flight &flight, const string &customer_name, unsigned int customer_age, unsigned int seat_number) : flight(flight) {
    if (seat_number >= flight.getPlane().getCapacity()) {
        throw "Not a valid ticket number";
    } else {
        this->customer_name = customer_name;
        this->customer_age = customer_age;
        this->seat_number = seat_number;
    }
}

Flight &Ticket::getFlight() const {
    return this->flight;
}

unsigned int Ticket::getSeatNumber() const {
    return this->seat_number;
}

std::string Ticket::getCustomerName() const {
    return this->customer_name;
}

unsigned int Ticket::getCustomerAge() const {
    return this->customer_age;
}

void Ticket::setCustomerName(string name) {
    this->customer_name = name;
}

void Ticket::setCustomerAge(unsigned int age) {
    this->customer_age = age;
}

string Ticket::str() const {
    ostringstream out;

    out << "Flight ID: " << this->flight.getFlightId() << endl
        << "Flight Departure: " << this->flight.getDepartureTime() << endl
        << "Customer Name: " << this->customer_name << endl
        << "Customer Age: " << this->customer_age << endl
        << "Seat Number: " << this->seat_number << endl;

    return out.str();
}

ostream &operator<<(ostream &out, const Ticket &ticket) {
    return out << ticket.str();
}
//void Ticket::addLuggage(Luggage *luggage) {
//    this->luggage.push_back(luggage);
//}
