#pragma once

class Ticket;

#include <list>
#include <ostream>
#include <string>

#include "flight.h"
#include "luggage.h"

class Ticket {
private:
    Flight &flight;
    std::string customer_name;
    unsigned int customer_age;
    unsigned int seat_number;

public:
    /**
     * Creates a ticket for a given flight
     * @param flight The flight this ticket has been issued for
     * @param customer_name The name of the customer that bought the ticket
     * @param customer_age The age of the customer that bought the ticket
     * @param seat_number The seat number for this ticket
     */
    Ticket(Flight &flight, const std::string &customer_name, unsigned int customer_age, unsigned int seat_number);

    // Getters

    Flight &getFlight() const;
    std::string getCustomerName() const;
    unsigned int getCustomerAge() const;
    unsigned int getSeatNumber() const;

    // Setters

    void setCustomerName(std::string name);
    void setCustomerAge(unsigned int age);

    /**
     * @brief Converts a Ticket instance into a string
     */
    std::string str() const;

    /**
     * @overload Displays a Ticket instance
     */
    friend std::ostream& operator<<(std::ostream &out, const Ticket &ticket);
};