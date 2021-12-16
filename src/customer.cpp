//
// Created by lima on 12/15/21.
//

#include "customer.h"

using namespace std;

Customer::Customer(const string &name, unsigned int age, const string &identification, const string &nationality)
        : name(name), age(age), identification(identification), nationality(nationality) {}

Customer::Customer(const Customer &customer)
    : Customer(
            customer.getName(),
            customer.getAge(),
            customer.getIdentification(),
            customer.getNationality()
    ) {}

string Customer::getName() const {
    return this->name;
}

string Customer::getIdentification() const {
    return this->identification;
}

unsigned int Customer::getAge() const {
    return this->age;
}

string Customer::getNationality() const {
    return this->nationality;
}
