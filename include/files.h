#ifndef AIRLINE_FILES_H
#define AIRLINE_FILES_H

#include <fstream>
#include <limits>
#include "handling_car.h"
#include "airport.h"
#include "luggage.h"
#include "state.h"
#include "ticket.h"
#include "service.h"

#include "state.h"

using namespace std;

namespace files {

    static const string PATH = "data.txt";
        
    void read() {

        for (const auto &el : data::airports) {
            delete el;
        }
        data::airports.clear();

        for (const auto &el : data::planes) {
            delete el;
        }
        data::planes.clear();
        
        for (const auto &el : data::flights) {
            delete el;
        }
        data::flights.clear();
 
        for (const auto &el : data::handlingCars) {
            delete el;
        }
        data::handlingCars.clear();

        for (const auto &el : data::airports) {
            delete el;
        }
        data::airports.clear();
 
                
        ifstream file(PATH);
        if (!file.is_open())
            return;

        unsigned int N1;
        file >> N1;

        for (unsigned int i = 0; i < N1; i++) {
            string name;
            unsigned int N2;
            
            getline(file, name);
            file >> N2;

            Airport *airport = new Airport(name);

            file.ignore(numeric_limits<streamsize>::max(), '\n');

            for (unsigned int j = 0; j < N2; j++) {
                string info_name, info_type;
                float info_lat, info_lon, info_dist;

                getline(file, info_name);
                file >> info_lat >> info_lon >> info_dist >> info_type;

                TransportType tt;
                if (info_type == "bus")
                    tt = TransportType::BUS;
                else if (info_type == "subway")
                    tt = TransportType::SUBWAY;
                else if (info_type == "train")
                    tt = TransportType::TRAIN;
                else
                    throw runtime_error("Unknown transport type");

                unsigned int N3;
                file >> N3;
                file.ignore(numeric_limits<streamsize>::max(), '\n');

                TransportPlace place = {
                    .name = info_name,
                    .latitude = info_lat,
                    .longitude = info_lon,
                    .transport_type = tt,
                    .airport_distance = info_dist
                };

                for (int k = 0; k < N3; k++) {
                    string time;
                    getline(file, time);
                    place.schedule.insert(Time::readFromString(time));
                }
                
                airport->addTransportPlaceInfo(place);
            }
            
            data::airports.push_back(airport);
        }


        unsigned int N4;
        file >> N4;

        for (unsigned int j = 0; j < N4; j++) {
            string licensePlate, type;
            unsigned int capacity;
            file >> licensePlate;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            getline(file, type);
            file >> capacity;

            Plane *plane = new Plane(licensePlate, type, capacity);

            unsigned int N5;
            file >> N5;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            for (unsigned int k = 0; k < N5; k++) {
                string worker, datetime, type;
                getline(file, worker);
                getline(file, datetime);
                getline(file, type);

                ServiceType st;
                if (type == "cleaning")
                    st = ServiceType::CLEANING;
                else if (type == "maintenance")
                    st = ServiceType::MAINTENANCE;
                else
                    throw runtime_error("Unknown service type");

                Service *service = new Service(st, Datetime::readFromString(datetime), worker, *plane);
                plane->scheduleService(*service);
                plane->completeService();
            }

            unsigned int N6;
            file >> N6;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            for (unsigned int k = 0; k < N6; k++) {
                string worker, datetime, type;
                getline(file, worker);
                getline(file, datetime);
                getline(file, type);

                ServiceType st;
                if (type == "cleaning")
                    st = ServiceType::CLEANING;
                else if (type == "maintenance")
                    st = ServiceType::MAINTENANCE;
                else
                    throw runtime_error("Unknown service type");

                Service *service = new Service(st, Datetime::readFromString(datetime), worker, *plane);
                plane->scheduleService(*service);
            }

            unsigned int N7;
            file >> N7;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            for (unsigned int k = 0; k < N7; k++) {
                string flightId, departureTime, duration, origin, destination;
                getline(file, flightId);
                getline(file, departureTime);
                getline(file, duration);
                getline(file, origin);
                getline(file, destination);

                Airport *originAirport = nullptr;
                for (const auto &airport : data::airports) {
                    if (airport->getName() == origin) {
                        originAirport = airport;
                        break;
                    }
                }

                Airport *destinationAirport = nullptr;
                for (const auto &airport : data::airports) {
                    if (airport->getName() == destination) {
                        destinationAirport = airport;
                        break;
                    }
                }

                if (originAirport == nullptr || destinationAirport == nullptr)
                    throw runtime_error("Unknown origin or destination airports");

                Flight *flight = new Flight(flightId, Datetime::readFromString(departureTime), Time::readFromString(duration), *originAirport, *destinationAirport, *plane);

                unsigned int N8;
                file >> N8;
                file.ignore(numeric_limits<streamsize>::max(), '\n');

                for (unsigned int m = 0; m < N8; m++) {
                    string customerName;
                    unsigned int customerAge, seatNumber;
                    getline(file, customerName);
                    file >> customerAge >> seatNumber;

                    Ticket *ticket = new Ticket(*flight, customerName, customerAge, seatNumber);
                    flight->addTicket(*ticket);
                }

                unsigned int N9;
                file >> N9;
                file.ignore(numeric_limits<streamsize>::max(), '\n');

                for (unsigned int m = 0; m < N9; m++) {
                    int index;
                    float weight;
                    file >> index >> weight;

                    Luggage *luggage = new Luggage(*flight->getTickets().at(index), weight);
                    flight->addLuggage(*luggage);
                }

                plane->addFlight(*flight);
                data::flights.push_back(flight);
            }

            data::planes.push_back(plane);
        }

        unsigned int N10 ;
        file >> N10;
            
        for (unsigned int j = 0; j < N10; j++) {
            unsigned int number_of_carriages, stacks_per_carriage, luggage_per_stack;
            file >> number_of_carriages >> stacks_per_carriage >> luggage_per_stack;

            HandlingCar *car = new HandlingCar(number_of_carriages, stacks_per_carriage, luggage_per_stack);

            string flightId;
            file >> flightId;
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (flightId == "none")
                continue;

            string datetime;
            getline(file, datetime);

            Datetime dt = Datetime::readFromString(datetime);

            for (const auto &flight : data::flights) {
                if (flight->getFlightId() == flightId && flight->getDepartureTime() == dt) {
                    car->setFlight(*flight);
                    break;
                }
            }

            if (car->getFlight() == nullptr)
                throw runtime_error("No flight found");

            
            unsigned int N12;
            file >> N12;

            for (unsigned int k = 0; k < N12; k++) {
                unsigned int index;
                float weight;

                file >> index >> weight;
                Luggage *luggage = new Luggage(*car->getFlight()->getTickets().at(index), weight);
                car->addLuggage(*luggage);
            }

            data::handlingCars.push_back(car);
        }
    }

    void write() {
        
        ofstream file(PATH);
        if (!file.is_open())
            throw runtime_error("Could not open file");


        // Writes airport data
        file << data::airports.size() << '\n';
        for (Airport *airport : data::airports) {
            file << airport->getName() << '\n'
                 << airport->getTransportPlaceInfo().size() << '\n';

            for (const auto &info : airport->getTransportPlaceInfo()) {
                file << info.name << '\n'
                     << info.latitude << ' ' << info.longitude << ' ' << info.airport_distance << '\n';

                switch (info.transport_type) {
                    case TransportType::BUS:
                        file << "bus";
                        break;
                    
                    case TransportType::SUBWAY:
                        file << "subway";
                        break;

                    case TransportType::TRAIN:
                        file << "train";
                        break;

                    default:
                        throw runtime_error("Transport type not present in enum");
                }

                file << '\n'
                     << info.schedule.size() << '\n';

                for (const auto &time : info.schedule) {
                    file << time.str() << '\n';
                }
            }
        }
         

        // Writes plane data 
        file << data::planes.size() << '\n';
        for (Plane *plane : data::planes) {
            file << plane->getLicensePlate() << '\n'
                 << plane->getType() << '\n'
                 << plane->getCapacity() << '\n';

            file << plane->getFinishedServices().size() << '\n';
            for (const auto &service : plane->getFinishedServices()) {
                file << service->getWorker() << '\n'
                     << service->getDatetime().str() << '\n';

                switch (service->getType()) {
                    case ServiceType::CLEANING:
                        file << "cleaning";
                    case ServiceType::MAINTENANCE:
                        file << "maintenance";
                    default:
                        throw runtime_error("Service type not in enum");

                }
                
                file << '\n';
            }

            queue<Service*> scheduled_services = plane->getScheduledServices();

            file << scheduled_services.size() << '\n';
            while (!scheduled_services.empty()) {
                Service *service = scheduled_services.front();

                file << service->getWorker() << '\n'
                     << service->getDatetime().str() << '\n';

                switch (service->getType()) {
                    case ServiceType::CLEANING:
                        file << "cleaning";
                    case ServiceType::MAINTENANCE:
                        file << "maintenance";
                    default:
                        throw runtime_error("Service type not in enum");
                }

                file << '\n';
                scheduled_services.pop();
            }

            file << plane->getFlights().size() << '\n';
            for (const auto &flight : plane->getFlights()) {
                file << flight->getFlightId() << '\n'
                     << flight->getDepartureTime().str() << '\n'
                     << flight->getDuration().str() << '\n'
                     << flight->getOrigin().getName() << '\n'
                     << flight->getDestination().getName() << '\n';

                file << flight->getTickets().size() << '\n';
                for (const auto &ticket : flight->getTickets()) {
                    file << ticket->getCustomerName() << '\n'
                         << ticket->getCustomerAge() << '\n'
                         << ticket->getSeatNumber() << '\n';
                }

                vector<Ticket*> tickets = flight->getTickets();
                
                file << flight->getLuggage().size() << '\n';
                for (const auto &luggage: flight->getLuggage()) {
                    int index = -1;
                    for (index = 0; index < tickets.size(); index++) {
                        if (tickets.at(index) == &luggage->getTicket()) 
                            break;
                    }
                    
                    if (index == -1)
                        throw runtime_error("Owner of luggage doesn't have a ticket");

                    file << index << '\n'
                         << luggage->getWeight() << '\n';
                }
            }

        }

        // Writes HandlingCar data
        file << data::handlingCars.size() << '\n';
        for (HandlingCar *handlingCar : data::handlingCars) {
            file << handlingCar->getNumberOfCarriages() << '\n'
                 << handlingCar->getStacksPerCarriage() << '\n'
                 << handlingCar->getLuggagePerStack() << '\n';

            if (handlingCar->getFlight() == nullptr) {
                file << "none" << '\n';
                continue;
            }

            file << handlingCar->getFlight()->getFlightId() << '\n' << handlingCar->getFlight()->getDepartureTime() << '\n';

            size_t occ = handlingCar->getCarriages().size() - 1;
            size_t ocs = handlingCar->getCarriages().back().size() - 1;
            size_t ocl = handlingCar->getCarriages().back().back().size();

            size_t numLuggage = occ * handlingCar->getStacksPerCarriage() * handlingCar->getLuggagePerStack()
                + ocs * handlingCar->getLuggagePerStack() + ocl;

            file << numLuggage << '\n';

            vector<Ticket*> tickets = handlingCar->getFlight()->getTickets();

            deque<Carriage> carriages = handlingCar->getCarriages();
            while (!carriages.empty()) {
                Carriage carriage = carriages.front();
                while (!carriage.empty()) {
                    LuggageStack luggageStack = carriage.front();

                    stack<Luggage*> reversed;

                    while (!luggageStack.empty()) {
                        reversed.push(luggageStack.top());
                        luggageStack.pop();
                    }

                    while (!reversed.empty()) {
                        Luggage *luggage = reversed.top();

                        int index = -1;
                        for (index = 0; index < tickets.size(); index++) {
                            if (tickets.at(index) == &luggage->getTicket()) 
                                break;
                        }
                        
                        if (index == -1)
                            throw runtime_error("Owner of luggage doesn't have a ticket");

                        file << index << '\n'
                            << luggage->getWeight() << '\n';

                        reversed.pop();
                    }

                    carriage.pop_front();
                }
                
                carriages.pop_front();
            }
        }
    }
}

#endif //AIRLINE_FILES_H