#include "ticket.h"
#include <sstream>

using namespace std;

Ticket::Ticket(Flight &flight, const string &customer_name, unsigned int customer_age, unsigned int seat_number) : flight(flight), seat_number(seat_number), customer_name(customer_name), customer_age(customer_age) {}

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
