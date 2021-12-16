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

Carriage *HandlingCar::getFrontCarriage() {
    if (this->carriages.empty())
        return nullptr;

    return &this->carriages.front();
}

Carriage const *HandlingCar::getFrontCarriage() const {
    if (this->carriages.empty())
        return nullptr;

    return &this->carriages.front();
}

LuggageStack *HandlingCar::getFrontLuggageStack() {
    Carriage *frontCarriage = this->getFrontCarriage();
    if (frontCarriage == nullptr || frontCarriage->empty())
        return nullptr;

    return &frontCarriage->front();
}

LuggageStack const *HandlingCar::getFrontLuggageStack() const {
    Carriage const *frontCarriage = this->getFrontCarriage();
    if (frontCarriage == nullptr || frontCarriage->empty())
        return nullptr;

    return &frontCarriage->front();
}

Luggage *HandlingCar::getNextLuggage() const {
    LuggageStack const *frontLuggageStack = this->getFrontLuggageStack();

    if (frontLuggageStack == nullptr || frontLuggageStack->empty())
        return nullptr;

    return frontLuggageStack->top();
}

Luggage *HandlingCar::unloadNextLuggage() {
    LuggageStack *frontLuggageStack = this->getFrontLuggageStack();

    if (frontLuggageStack == nullptr || frontLuggageStack->empty())
        return nullptr;

    Luggage &luggage = *frontLuggageStack->top();
    frontLuggageStack->pop();

    return &luggage;
}

Carriage *HandlingCar::ensureFrontCarriageExists() {
    Carriage *frontCarriage = this->getFrontCarriage();

    // If there is no front carriage, we need to create one
    if (frontCarriage == nullptr) {
        Carriage carriage;
        this->carriages.push(carriage);
    }

    frontCarriage = this->getFrontCarriage();
    if (frontCarriage == nullptr)
        throw logic_error("There is no front carriage");

    return frontCarriage;
}

LuggageStack *HandlingCar::ensureFrontLuggageStackExists() {
    Carriage *frontCarriage = ensureFrontCarriageExists();
    LuggageStack *frontLuggageStack = this->getFrontLuggageStack();

    if (frontLuggageStack == nullptr) {
        LuggageStack luggageStack;
        frontCarriage->push(luggageStack);
    }

    frontLuggageStack = this->getFrontLuggageStack();
    if (frontLuggageStack == nullptr)
        throw logic_error("There is no front luggage stack");

    return frontLuggageStack;
}

bool HandlingCar::addLuggage(Luggage *luggage) {
    // Now, we just need to verify if the limits aren't exceeded
    // Limits:
    //      - A carriage cannot have more than stacks_per_carriage LuggageStacks
    //      - A stack cannot have more than luggage_per_stack Luggage*

    LuggageStack *frontLuggageStack = this->ensureFrontLuggageStackExists();
    if (frontLuggageStack->size() < this->luggage_per_stack) {
        frontLuggageStack->push(luggage);
        return true;
    }

    // We need to create a new stack and add it to a carriage
    LuggageStack new_stack;
    new_stack.push(luggage);

    Carriage *frontCarriage = this->ensureFrontCarriageExists();
    if (frontCarriage->size() < this->stacks_per_carriage) {
        frontCarriage->push(new_stack);
        return true;
    }

    // We need to create a new carriage and add it to the car
    Carriage new_carriage;
    new_carriage.push(new_stack);

    if (this->carriages.size() >= this->number_of_carriages)
        return false;

    this->carriages.push(new_carriage);
    return true;
}
