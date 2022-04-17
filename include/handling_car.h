#pragma once

#include <stack>
#include <deque>
#include "luggage.h"

using LuggageStack = std::stack<Luggage*>;
using Carriage = std::deque<LuggageStack>;

class HandlingCar {
    unsigned int id;
    unsigned int number_of_carriages, stacks_per_carriage, luggage_per_stack;
    Flight *flight = nullptr;
    std::deque<Carriage> carriages;

    /**
     * @brief Create a carriage, if one doesn't already exist, and put it at the back
     * @return The carriage that was created
     */
    Carriage *ensureBackCarriageExists();

    /**
     * @brief Create a stack of luggage at the last carriage, if one doesn't already exist, and put it at the back of the last carriage.
     * @return The stack of luggage that was created
     */
    LuggageStack *ensureBackLuggageStackExists();

    /**
     * @brief Get the last carriage
     * @return This car's last carriage
     */
    Carriage *getBackCarriage();

    /**
     * @brief Get the last carriage
     * @return This car's last carriage
     */
    Carriage const *getBackCarriage() const;

    /**
     * @brief Get the last luggage stack
     * @return This car's last carriage last luggage stack
     */
    LuggageStack *getBackLuggageStack();

    /**
     * @brief Get the last luggage stack
     * @return This car's last carriage last luggage stack
     */
    LuggageStack const *getBackLuggageStack() const;

public:
    /**
     * @brief Creates a HandlingCar with a given capacity
     * @param number_of_carriages The car's number of carriages
     * @param stacks_per_carriage The number of luggage stacks per carriage
     * @param luggage_per_stack The number of luggage per stack
     */
    HandlingCar(const unsigned int number_of_carriages, const unsigned int stacks_per_carriage, const unsigned int luggage_per_stack);

    // Getters

    unsigned int getId();
    unsigned int getNumberOfCarriages() const;
    unsigned int getStacksPerCarriage() const;
    unsigned int getLuggagePerStack() const;

    std::string str() const;

    /**
     * @brief Get the next luggage to be unloaded
     * @return The top piece of luggage of the last stack of the last carriage
     */
    Luggage* getNextLuggage() const;

    /**
     * @brief Remove the next luggage to be unloaded from this handling car
     * @return The piece of luggage that was removed from the car
     */
    Luggage* unloadNextLuggage();

    /**
     * @brief Adds a piece of luggage to the last position in the car
     * @param luggage The luggage to be loaded onto the car
     * @return true if there was enough space to load the luggage, false otherwise
     */
    bool addLuggage(Luggage &luggage);

    std::deque<Carriage> getCarriages() const;

    Flight* getFlight() const;
    void setFlight(Flight &flight);

    friend std::ostream& operator<<(std::ostream &out, const HandlingCar &car);
};