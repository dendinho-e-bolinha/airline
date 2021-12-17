//
// Created by lima on 12/15/21.
//

#ifndef AIRLINE_CUSTOMER_H
#define AIRLINE_CUSTOMER_H

#include <string>

class Customer {
    std::string name;
    unsigned int age;

    std::string identification;
    std::string nationality;

public:
    Customer(const std::string &name, unsigned int age, const std::string &identification, const std::string &nationality);
    Customer(const Customer &customer);

    std::string getName() const;
    unsigned int getAge() const;
    std::string getIdentification() const;
    std::string getNationality() const;
};


#endif //AIRLINE_CUSTOMER_H
