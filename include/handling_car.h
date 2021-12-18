#ifndef AIRLINE_HANDLING_CART_H
#define AIRLINE_HANDLING_CART_H

#include <stack>
#include <deque>
#include "luggage.h"

using LuggageStack = std::stack<Luggage*>;
using Carriage = std::deque<LuggageStack>;

class HandlingCar {
    unsigned int number_of_carriages, stacks_per_carriage, luggage_per_stack;
    std::deque<Carriage> carriages;

    /**
     * Create a carriage, if one doesn't already exist, and put it at the back
     * @return The carriage that was created
     */
    Carriage *ensureBackCarriageExists();

    /**
     * Create a stack of luggage at the last carriage, if one doesn't already exist, and put it at the back of the last carriage.
     * @return The stack of luggage that was created
     */
    LuggageStack *ensureBackLuggageStackExists();

    /**
     * Get the last carriage
     * @return This car's last carriage
     */
    Carriage *getBackCarriage();

    /**
     * Get the last carriage
     * @return This car's last carriage
     */
    Carriage const *getBackCarriage() const;

    /**
     * Get the last luggage stack
     * @return This car's last carriage last luggage stack
     */
    LuggageStack *getBackLuggageStack();

    /**
     * Get the last luggage stack
     * @return This car's last carriage last luggage stack
     */
    LuggageStack const *getBackLuggageStack() const;

public:
    /**
     * Creates a HandlingCar with a given capacity
     * @param number_of_carriages The car's number of carriages
     * @param stacks_per_carriage The number of luggage stacks per carriage
     * @param luggage_per_stack The number of luggage per stack
     */
    HandlingCar(const unsigned int number_of_carriages, const unsigned int stacks_per_carriage, const unsigned int luggage_per_stack);

    unsigned int getNumberOfCarriages() const;
    unsigned int getStacksPerCarriage() const;
    unsigned int getLuggagePerStack() const;

    /**
     * Get the next luggage to be unloaded
     * @return The top piece of luggage of the last stack of the last carriage
     */
    Luggage* getNextLuggage() const;

    /**
     * Remove the next luggage to be unloaded from this handling car
     * @return The piece of luggage that was removed from the car
     */
    Luggage* unloadNextLuggage();

    /**
     * Adds a piece of luggage to the last position in the car
     * @param luggage The luggage to be loaded onto the car
     * @return true if there was enough space to load the luggage, false otherwise
     */
    bool addLuggage(Luggage &luggage);
};

#endif //AIRLINE_HANDLING_CAR_H