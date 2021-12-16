#ifndef AIRLINE_HANDLING_CART_H
#define AIRLINE_HANDLING_CART_H

#include <stack>
#include <queue>
#include "luggage.h"

using LuggageStack = std::stack<Luggage*>;
using Carriage = std::queue<LuggageStack>;

class HandlingCar {
    unsigned int number_of_carriages, stacks_per_carriage, luggage_per_stack;
    std::queue<Carriage> carriages;

    Carriage *ensureFrontCarriageExists();
    LuggageStack *ensureFrontLuggageStackExists();

    Carriage *getFrontCarriage();
    Carriage const *getFrontCarriage() const;

    LuggageStack *getFrontLuggageStack();
    LuggageStack const *getFrontLuggageStack() const;

public:
    HandlingCar(const unsigned int number_of_carriages, const unsigned int stacks_per_carriage, const unsigned int luggage_per_stack);

    unsigned int getNumberOfCarriages() const;
    unsigned int getStacksPerCarriage() const;
    unsigned int getLuggagePerStack() const;

    Luggage* getNextLuggage() const;
    Luggage* unloadNextLuggage();
    bool addLuggage(Luggage* luggage);
};

#endif //AIRLINE_HANDLING_CAR_H
