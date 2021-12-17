#include "handling_car.h"
#include <stdexcept>

using namespace std;

HandlingCar::HandlingCar(const unsigned int number_of_carriages, const unsigned int stacks_per_carriage, const unsigned int luggage_per_stack)
        : number_of_carriages(number_of_carriages), stacks_per_carriage(stacks_per_carriage),
          luggage_per_stack(luggage_per_stack) {

    if (number_of_carriages == 0)
        throw invalid_argument("Number of carriages must be greater than 0");

    if (stacks_per_carriage == 0)
        throw invalid_argument("Number of stacks per carriage must be greater than 0");

    if (luggage_per_stack == 0)
        throw invalid_argument("Number of luggage per stack must be greater than 0");
}

unsigned int HandlingCar::getNumberOfCarriages() const {
    return this->number_of_carriages;
}

unsigned int HandlingCar::getStacksPerCarriage() const {
    return this->stacks_per_carriage;
}

unsigned int HandlingCar::getLuggagePerStack() const {
    return this->luggage_per_stack;
}

Carriage *HandlingCar::getBackCarriage() {
    if (this->carriages.empty())
        return nullptr;

    return &this->carriages.back();
}

Carriage const *HandlingCar::getBackCarriage() const {
    if (this->carriages.empty())
        return nullptr;

    return &this->carriages.back();
}

LuggageStack *HandlingCar::getBackLuggageStack() {
    Carriage *backCarriage = this->getBackCarriage();
    if (backCarriage == nullptr || backCarriage->empty())
        return nullptr;

    return &backCarriage->front();
}

LuggageStack const *HandlingCar::getBackLuggageStack() const {
    Carriage const *backCarriage = this->getBackCarriage();
    if (backCarriage == nullptr || backCarriage->empty())
        return nullptr;

    return &backCarriage->front();
}

Luggage *HandlingCar::getNextLuggage() const {
    LuggageStack const *backLuggageStack = this->getBackLuggageStack();

    if (backLuggageStack == nullptr || backLuggageStack->empty())
        return nullptr;

    return backLuggageStack->top();
}

Luggage *HandlingCar::unloadNextLuggage() {
    LuggageStack *backLuggageStack = this->getBackLuggageStack();

    if (backLuggageStack == nullptr || backLuggageStack->empty())
        return nullptr;

    Luggage &luggage = *backLuggageStack->top();
    backLuggageStack->pop();

    Carriage *backCarriage = this->getBackCarriage();
    if (backLuggageStack->empty())
        backCarriage->pop_back();

    if (backCarriage->empty())
        this->carriages.pop_back();

    return &luggage;
}

Carriage *HandlingCar::ensureBackCarriageExists() {
    Carriage *backCarriage = this->getBackCarriage();

    // If there is no front carriage, we need to create one
    if (backCarriage == nullptr) {
        Carriage carriage;
        this->carriages.push_back(carriage);
    }

    backCarriage = this->getBackCarriage();
    if (backCarriage == nullptr)
        throw logic_error("There is no front carriage");

    return backCarriage;
}

LuggageStack *HandlingCar::ensureBackLuggageStackExists() {
    Carriage *backCarriage = ensureBackCarriageExists();
    LuggageStack *backLuggageStack = this->getBackLuggageStack();

    if (backLuggageStack == nullptr) {
        LuggageStack luggageStack;
        backCarriage->push_back(luggageStack);
    }

    backLuggageStack = this->getBackLuggageStack();
    if (backLuggageStack == nullptr)
        throw logic_error("There is no front luggage stack");

    return backLuggageStack;
}

bool HandlingCar::addLuggage(Luggage &luggage) {
    // Now, we just need to verify if the limits aren't exceeded
    // Limits:
    //      - A carriage cannot have more than stacks_per_carriage LuggageStacks
    //      - A stack cannot have more than luggage_per_stack Luggage*

    LuggageStack *backLuggageStack = this->ensureBackLuggageStackExists();
    if (backLuggageStack->size() < this->luggage_per_stack) {
        backLuggageStack->push(&luggage);
        return true;
    }

    // We need to create a new stack and add it to a carriage
    LuggageStack new_stack;
    new_stack.push(&luggage);

    Carriage *backCarriage = this->ensureBackCarriageExists();
    if (backCarriage->size() < this->stacks_per_carriage) {
        backCarriage->push_back(new_stack);
        return true;
    }

    // We need to create a new carriage and add it to the car
    Carriage new_carriage;
    new_carriage.push_back(new_stack);

    if (this->carriages.size() >= this->number_of_carriages)
        return false;

    this->carriages.push_back(new_carriage);
    return true;
}
