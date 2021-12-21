#include "crud.h"
#include "utils.h"
#include "interact.h"
#include "state.h"
#include <set>
#include <algorithm>

using namespace std;

namespace crud {

    Airport* findAirportByName(const string name);
    string askUnusedName();
    string askUsedName();
    /*----------PLANES----------*/

    Plane* findPlaneByLicensePlate(const string &license_plate) {
        return utils::binarySearch<Plane*, string>(data::planes, license_plate, [](Plane *plane) {
            return plane->getLicensePlate();
        });
    }

    string askUnusedLicensePlate() {
        return readValue<string>("License plate: ", "Please insert a valid license plate", [](const string &value) {
            Plane *plane = findPlaneByLicensePlate(value);
            if (plane != nullptr)
                throw validation_error("That license plate already belongs to a plane");

            return true;
        });
    }
    
    string askUsedLicensePlate() {
        return readValue<string>("License plate: ", "Please insert a valid license plate", [](const string &value) {
            Plane *plane = findPlaneByLicensePlate(value);
            if (plane == nullptr)
                throw validation_error("That license plate doesn't belong to any plane");

            return true;
        });
    }

    Plane &findPlane() {
        if (data::planes.empty())
            throw logic_error("No plane found");

        if (data::planes.size() == 1)
            return *data::planes.at(0);

        string license_plate = askUsedLicensePlate();
        cout << endl;

        Plane *plane = findPlaneByLicensePlate(license_plate);
        if (plane == nullptr)
            throw logic_error("No plane found");

        return *plane;
    }

    /**
     * @brief Creates a Plane instance
     */
    void createPlane() {
        string license_plate = askUnusedLicensePlate(); 
        string type = readValue<GetLine>("Type: ", "Please insert a valid plane type");
        unsigned int capacity = readValue<unsigned int>("Capacity: ", "Please insert a valid capacity");
        cout << endl;

        Plane *plane = new Plane(license_plate, type, capacity);
        auto pos = utils::lowerBound<Plane*, string>(data::planes, license_plate, [](Plane* plane) {
            return plane->getLicensePlate();
        });

        data::planes.insert(pos, plane);
        waitForInput();
    }

    /**
     * @brief Displays a plane specified by the user
     */
    void readOnePlane() {
        const Plane &plane = findPlane();
        cout << plane << endl;

        queue<Service*> scheduled_services = plane.getScheduledServices();

        if (!scheduled_services.empty()) {
            cout << "\nScheduled services:\n";
            while(!scheduled_services.empty()) {
                cout << '\n' << *scheduled_services.front() << '\n';
                scheduled_services.pop();
            }
        }
        
        if (!plane.getFinishedServices().empty()) {
            cout << "\nFinished services:\n";
            for (const auto &service: plane.getFinishedServices()) {
                cout << '\n' << *service << '\n';
            }
        }

        cout << flush;
        waitForInput();
    }

    string getPlanesRepresentation(const vector<Plane*> &vec) {
        ostringstream repr;
        
        bool is_empty = true;
        for (const Plane *plane : vec) {
            if (!is_empty)
                repr << '\n';

            repr << *plane;
            is_empty = false;
        }

        return repr.str();
    }

    /**
     * @brief Displays all the existing planes
     */
    void readAllPlanes() {
        cout << getPlanesRepresentation(data::planes) << endl;
        waitForInput();
    }


    template <typename T, typename V, typename P>
    function<bool(const T&)> createFilter(ostringstream &repr, const function<const V(const T&)> mapper, const function<bool(const V&, const V&)> filter, const function<bool(const P&)> validator = [](const P&) { return true; }) {
        P operand2 = readValue<P>(repr.str(), "Please specify a valid value", validator);
        cout << endl;

        repr << operand2;

        return [operand2, mapper, filter](const T& operand1) { 
            const V mapped = mapper(operand1);
            return filter(mapped, operand2);
        };
    }

    template <typename T, typename V, typename P = V>
    function<bool(const T&)> createFilter(ostringstream &repr, const function<const V(const T&)> mapper, const function<bool(const P&)> validator = [](const P&) { return true; }) {
        Menu menu("Please specify an operator to use in the filter:");
        function<bool(const T&)> filter;

        MenuBlock block;
        block.addOption(repr.str() + "equal to", [&filter, &repr, mapper, validator]() {
            repr << "equal to ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs == rhs;
            }, validator);
        });

        block.addOption(repr.str() + "not equal to", [&filter, &repr, mapper, validator]() {
            repr << "not equal to ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs != rhs;
            }, validator);
        });

        block.addOption(repr.str() + "less than", [&filter, &repr, mapper, validator]() {
            repr << "less than ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs < rhs;
            }, validator);
        });

        block.addOption(repr.str() + "greater than", [&filter, &repr, mapper, validator]() {
            repr << "greater than ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs > rhs;
            }, validator);
        });

        block.addOption(repr.str() + "less than or equal to", [&filter, &repr, mapper, validator]() {
            repr << "less than or equal to ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs <= rhs;
            }, validator);
        });

        block.addOption(repr.str() + "greater than or equal to", [&filter, &repr, mapper, validator]() {
            repr << "greater than or equal to ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs >= rhs;
            }, validator);
        });
        

        menu.addBlock(block);
        menu.show();

        return filter;
    }

    function<bool(const Plane* const&)> createPlaneFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const Plane* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "license plate", [&filter, &repr]() {
            repr << "license plate ";
            filter = createFilter<const Plane*, string>(repr, [](const Plane* const &value) {
                return value->getLicensePlate();
            });
        });

        directAttributes.addOption(repr.str() + "type", [&filter, &repr]() {
            repr << "type ";
            filter = createFilter<const Plane*, string, GetLine>(repr, [](const Plane* const &value) {
                return value->getType();
            });
        });

        directAttributes.addOption(repr.str() + "capacity", [&filter, &repr]() {
            repr << "capacity ";
            filter = createFilter<const Plane*, unsigned int>(repr, [](const Plane* const &value) {
                return value->getCapacity();
            });
        });

        directAttributes.addOption(repr.str() + "number of flights", [&filter, &repr]() {
            repr << "number of flights ";
            filter = createFilter<const Plane*, unsigned int>(repr, [](const Plane* const &value) {
                return value->getFlights().size();
            });
        });

        directAttributes.addOption(repr.str() + "number of scheduled services", [&filter, &repr]() {
            repr << "number of schedules services ";
            filter = createFilter<const Plane*, unsigned int>(repr, [](const Plane* const &value) {
                return value->getScheduledServices().size();
            });
        });

        directAttributes.addOption(repr.str() + "number of finished services", [&filter, &repr]() {
            repr << "number of flights ";
            filter = createFilter<const Plane*, unsigned int>(repr, [](const Plane* const &value) {
                return value->getFinishedServices().size();
            });
        });

        MenuBlock flights;
        flights.addOption(repr.str() + "all flights have", [&filter, &repr]() {
            repr << "all flights have ";

            auto flightFilter = createFlightFilter(repr);
            filter = [flightFilter](const Plane* const &plane) {
                for (const Flight* const &flight : plane->getFlights()) {
                    if (!flightFilter(flight))
                        return false;
                }

                return true;
            };
        });

        flights.addOption(repr.str() + "any flights have", [&filter, &repr]() {
            repr << "any flights have ";

            auto flightFilter = createFlightFilter(repr);
            filter = [flightFilter](const Plane* const &plane) {
                for (const Flight* const &flight : plane->getFlights()) {
                    if (flightFilter(flight))
                        return true;
                }

                return false;
            };
        });

        MenuBlock services;
        services.addOption(repr.str() + "all finished services have", [&filter, &repr]() {
            repr << "all finished services have ";

            auto serviceFilter = createServiceFilter(repr);
            filter = [serviceFilter](const Plane* const &plane) {
                for (const Service* const &flight : plane->getFinishedServices()) {
                    if (!serviceFilter(flight))
                        return false;
                }

                return true;
            };
        });

        services.addOption(repr.str() + "any finished services have", [&filter, &repr]() {
            repr << "any finished services have ";

            auto serviceFilter = createServiceFilter(repr);
            filter = [serviceFilter](const Plane* const &plane) {
                for (const Service* const &service : plane->getFinishedServices()) {
                    if (serviceFilter(service))
                        return true;
                }

                return false;
            };
        });

        services.addOption(repr.str() + "the next scheduled service has", [&filter, &repr]() {
            repr << "the next scheduled service has ";

            auto serviceFilter = createServiceFilter(repr);
            filter = [serviceFilter](const Plane* const &plane) {
                if (plane->getScheduledServices().empty())
                    return false;

                return serviceFilter(plane->getScheduledServices().front());
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createPlaneFilter(repr);
            repr << ')';

            filter = [filter1](const Plane* const &plane) {
                return !filter1(plane);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createPlaneFilter(repr);
            repr << ") or (";
            auto filter2 = createPlaneFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Plane* const &plane) {
                return filter1(plane) || filter2(plane);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createPlaneFilter(repr);
            repr << ") and (";
            auto filter2 = createPlaneFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Plane* const &plane) {
                return filter1(plane) && filter2(plane);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(flights);
        menu.addBlock(services);
        menu.addBlock(booleanLogic);
        menu.show();

        return filter;
    }
    
    /**
     * @brief Displays all planes with characteristics specified by the user
     */
    void filterPlanesWithUserInput(vector<Plane*> &pool) {
        ostringstream filter_repr;
        function<bool(const Plane* const&)> filter = createPlaneFilter(filter_repr);

        cout << "\x1B[2J\x1B[;H\x1B[32m✓\x1B[0m " << "Your filter: " << filter_repr.str() << '\n' << endl;
        waitForInput();

        for (auto it = pool.begin(); it != pool.end();) {
            if (!filter(*it))
                it = pool.erase(it);
            else
                it++;
        }
    }

    void orderPlanesWithUserInput(vector<Plane*> &pool) {
        Menu menu("Please select an attribute to sort the selected planes:");

        MenuBlock block;
        block.addOption("License Plate", [&pool]() {
            utils::sort<Plane*, string>(pool, [](Plane* const &plane) {
                return plane->getLicensePlate();
            });
        });

        block.addOption("Type", [&pool]() {
            utils::sort<Plane*, string>(pool, [](Plane* const &plane) {
                return plane->getType();
            });
        });

        block.addOption("Capacity", [&pool]() {
            utils::sort<Plane*, unsigned int>(pool, [](Plane* const &plane) {
                return plane->getCapacity();
            });
        });

        block.addOption("Number of flights", [&pool]() {
            utils::sort<Plane*, unsigned int>(pool, [](Plane* const &plane) {
                return plane->getFlights().size();
            });
        });

        block.addOption("Number of scheduled services", [&pool]() {
            utils::sort<Plane*, unsigned int>(pool, [](Plane* const &plane) {
                return plane->getScheduledServices().size();
            });
        });

        block.addOption("Number of finished services", [&pool]() {
            utils::sort<Plane*, unsigned int>(pool, [](Plane* const &plane) {
                return plane->getFinishedServices().size();
            });
        });

        menu.addBlock(block);
        menu.show();
    }

    void readAllPlanesWithUserInput() {
        vector<Plane*> pool = data::planes;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterPlanesWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderPlanesWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # planes", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # planes", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);   
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no planes left!\n");
            else
                menu.show(getPlanesRepresentation(pool));
        }
    }

    /**
     * @brief Updates one Plane instance
     */
    void updatePlane() {
        Plane &plane = findPlane();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Type", [&plane]() {
            string type = readValue<GetLine>("Type: ", "Please insert a valid plane type");
            plane.setType(type);
        });

        choice.addOption("Capacity", [&plane]() {
            unsigned int capacity = readValue<unsigned int>("Capacity: ", "Please insert a valid plane capacity");
            plane.setCapacity(capacity);
        });

        choice.addOption("Schedule a new service", [&plane]() {
            Datetime datetime = Datetime::readFromString(
                readValue<GetLine>("Date and time of service: ", "Please insert a valid date and time", [](const string &value) {
                    Datetime::readFromString(value);
                    return true;
                })
            );
            
            string worker = readValue<GetLine>("Worker's name: ", "Please insert a valid name");

            Menu serviceType("Please choose a type of service:");
            
            MenuBlock services;
            services.addOption("Maintenance", [&plane, &datetime, &worker]() {
                Service *service = new Service(ServiceType::MAINTENANCE, datetime, worker, plane);
                plane.scheduleService(*service);
            });

            services.addOption("Cleaning", [&plane, &datetime, &worker]() {
                Service *service = new Service(ServiceType::CLEANING, datetime, worker, plane);
                plane.scheduleService(*service);
            });

            serviceType.addBlock(services);
            serviceType.show();
        });

        choice.addOption("Complete the next service", [&plane] {
            if (plane.completeService()) {
                cout << "The next service was marked as completed!\n" << endl;
            } else {    
                cout << "There are no scheduled services!\n" << endl;
            }

            waitForInput();
        });

        bool is_running = true;
        
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);
    
        while (is_running) {
            menu.show(plane.str());
        }
    }

    /**
     * @brief Deletes one Plane instance specified by the user
     */
    void deleteOnePlane() {
        Plane const &plane = findPlane();
        for (auto it = data::planes.begin(), end = data::planes.end(); it != end; it++) {
            if (*it == &plane) {
                data::planes.erase(it);
                delete &plane;

                waitForInput();
                return;
            }
        }

        delete &plane;
        throw logic_error("No planes were deleted");
    }

    /**
     * @brief Deletes every Plane instance
     */
    void deleteAllPlanes() {
        for (const Plane *plane : data::planes)
            delete plane;

        data::planes.clear();
    }

    void deleteAllPlanesWithUserInput() {
        vector<Plane*> pool = data::planes;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterPlanesWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderPlanesWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # planes", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # planes", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        MenuBlock erase;
        erase.addOption("Delete all planes in this selection", [&pool]() {
            vector<Plane*> new_planes;
            
            for (Plane *plane1 : data::planes) {
                bool was_selected = false;

                for (Plane *plane2 : pool) {
                    if (plane1 == plane2)
                        was_selected = true;
                        break;
                }

                if (was_selected)
                    delete plane1;
                else
                    new_planes.push_back(plane1);
            }

            data::planes = pool = new_planes;
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
                menu.addBlock(erase);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no planes left!\n");
            else
                menu.show(getPlanesRepresentation(pool));
        }
    }

    /**
     * @brief Manages all the existing operations regarding Planes, as well as the
     * operations we can do with them
     */
    void managePlanes() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &func) {
            return [func]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database\n" << endl;
                    waitForInput();
                    return;
                }

                func();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new plane", createPlane);
        normal.addOption("Update plane", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one plane", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all planes", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all planes with filters and sort", allowWhenPlanesExist(readAllPlanesWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one plane", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all planes", allowWhenPlanesExist(deleteAllPlanes));
        remove.addOption("Delete all planes with filters and sort", allowWhenPlanesExist(deleteAllPlanesWithUserInput));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    /*----------FLIGHTS----------*/

    vector<Flight*> findFlightsByFlightId(const string &flight_id) {
        auto it = utils::lowerBound<Flight*, string>(data::flights, flight_id, [](Flight *flight) {
            return flight->getFlightId();
        });

        vector<Flight*> result;
        if (it == data::flights.end())
            return result;

        while ((*it)->getFlightId() == flight_id && it != data::flights.end())
            result.push_back(*(it++));

        return result;
    }

    Flight* findFlightByKey(const string &flight_id, const Datetime &departure) {
        auto flights = findFlightsByFlightId(flight_id);

        for (Flight *flight : flights)
            if (flight->getDepartureTime() == departure)
                return flight;

        return nullptr;
    }

    pair<string, Datetime> askUnusedFlightKey() {
        string id = readValue<string>("Flight ID: ", "Please insert a valid flight ID");
        Datetime datetime = Datetime::readFromString(
            readValue<GetLine>("Date and time of flight: ", "That date and time are already being used by another flight with the same id", [&id](const string &value) {
                Datetime datetime = Datetime::readFromString(value);
                return findFlightByKey(id, datetime) == nullptr;
            })
        );

        return make_pair(id, datetime);
    }
    
    pair<string, Datetime> askUsedFlightKey() {
        string id = readValue<string>("Flight ID: ", "Please insert a valid flight ID", [](const string &value) {
            if (findFlightsByFlightId(value).size() == 0)
                throw validation_error("There are no flights with that ID");

            return true;
        });
        
        vector<Flight*> flights = findFlightsByFlightId(id);
        if (flights.size() == 1)
            return make_pair(flights.at(0)->getFlightId(), flights.at(0)->getDepartureTime());
        
        Datetime datetime = Datetime::readFromString(
            readValue<GetLine>("Date and time of flight: ", "That date and time are not being used by any flight with the given id", [&id](const string &value) {
                Datetime datetime = Datetime::readFromString(value);
                return findFlightByKey(id, datetime) != nullptr;
            })
        );

        return make_pair(id, datetime);
    }

    Flight &findFlight() {
        if (data::planes.empty())
            throw logic_error("No flight found");

        if (data::flights.size() == 1)
            return *data::flights.at(0);

        pair<string, Datetime> flight_key = askUsedFlightKey();
        cout << endl;

        Flight *flight = findFlightByKey(flight_key.first, flight_key.second);
        if (flight == nullptr)
            throw logic_error("No flight found");

        return *flight;
    }

    /**
     * @brief Creates a Plane instance
     */
    void createFlight() {
        if (data::airports.size() < 2) {
            cout << "There must be at least 2 airports to create a new flight\n" << endl;
            waitForInput();
            return;
        }

        pair<string, Datetime> flight_key = askUnusedFlightKey();
        Plane &plane = findPlane();

        Time duration = Time::readFromString(
            readValue<string>("Flight duration: ", "Please insert a valid flight duration", [](const string &value) {
                Time::readFromString(value);
                return true;
            })
        );

        Airport &origin = *findAirportByName(
            readValue<GetLine>("Origin airport name: ", "Please provid a valid airport name", [](const string &value) {
                Airport *airport = findAirportByName(value);
                return airport != nullptr;
            })
        );

        Airport &destination = *findAirportByName(
            readValue<GetLine>("Destination airport name: ", "Please provid a valid airport name", [&origin](const string &value) {
                if (value == origin.getName())
                    throw validation_error("Destination airport must not be the same as the origin airport");

                Airport *airport = findAirportByName(value);
                return airport != nullptr;
            })
        );

        Flight *flight = new Flight(flight_key.first, flight_key.second, duration, origin, destination, plane);
        plane.addFlight(*flight);

        auto pos = utils::lowerBound<Flight*, string>(data::flights, flight->getFlightId(), [](const Flight* flight) {
            return flight->getFlightId();
        });

        data::flights.insert(pos, flight);

        cout << endl;
        waitForInput();
    }

    /**
     * @brief Displays a plane specified by the user
     */
    void readOneFlight() {
        const Flight &flight = findFlight();
        cout << flight << endl;
        waitForInput();
    }

    string getFlightRepresentation(const vector<Flight*> &vec) {
        ostringstream repr;
        
        bool is_empty = true;
        for (const Flight *flight : vec) {
            if (!is_empty)
                repr << '\n';

            repr << *flight;
            is_empty = false;
        }

        return repr.str();
    }

    /**
     * @brief Displays all the existing planes
     */
    void readAllFlights() {
        cout << getFlightRepresentation(data::flights) << endl;
        waitForInput();
    }

    function<bool(const Flight* const&)> createFlightFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const Flight* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "ID", [&filter, &repr]() {
            repr << "ID ";
            filter = createFilter<const Flight*, string>(repr, [](const Flight* const &value) {
                return value->getFlightId();
            });
        });

        directAttributes.addOption(repr.str() + "departure time", [&filter, &repr]() {
            repr << "departure time ";
            filter = createFilter<const Flight*, string>(repr, [](const Flight* const &value) {
                return value->getDepartureTime().str();
            });
        });

        directAttributes.addOption(repr.str() + "duration", [&filter, &repr]() {
            repr << "duration ";
            filter = createFilter<const Flight*, string>(repr, [](const Flight* const &value) {
                return value->getDuration().str();
            });
        });

        directAttributes.addOption(repr.str() + "number of tickets", [&filter, &repr]() {
            repr << "number of tickets ";
            filter = createFilter<const Flight*, size_t>(repr, [](const Flight* const &value) {
                return value->getTickets().size();
            });
        });

        MenuBlock plane;
        plane.addOption(repr.str() + "plane has", [&filter, &repr]() {
            repr << "plane has ";
            auto planeFilter = createPlaneFilter(repr);
            filter = [planeFilter](const Flight* const &flight) {
                return planeFilter(&flight->getPlane());
            };
        });

        MenuBlock airports;
        airports.addOption(repr.str() + "origin airport has ", [&filter, &repr]() {
            repr << "origin airport has ";
            auto airportFilter = createAirportFilter(repr);
            filter = [airportFilter](const Flight* const &flight) {
                return airportFilter(&flight->getOrigin());
            };
        });

        airports.addOption(repr.str() + "destination airport has", [&filter, &repr]() {
            repr << "destination airport has ";
            auto airportFilter = createAirportFilter(repr);
            filter = [airportFilter](const Flight* const &flight) {
                return airportFilter(&flight->getDestination());
            };
        });

        MenuBlock tickets;
        tickets.addOption(repr.str() + "all tickets have ", [&filter, &repr]() {
            repr << "all tickets have ";
            auto ticketFilter = createTicketFilter(repr);
            filter = [ticketFilter](const Flight* const &flight) {
                for (const Ticket* const &ticket : flight->getTickets()) {
                    if (!ticketFilter(ticket))
                        return false;
                }

                return true;
            };
        });

        tickets.addOption(repr.str() + "any tickets have ", [&filter, &repr]() {
            repr << "any tickets have ";
            auto ticketFilter = createTicketFilter(repr);
            filter = [ticketFilter](const Flight* const &flight) {
                for (const Ticket* const &ticket : flight->getTickets()) {
                    if (ticketFilter(ticket))
                        return true;
                }

                return false;
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createFlightFilter(repr);
            repr << ')';

            filter = [filter1](const Flight* const &flight) {
                return !filter1(flight);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createFlightFilter(repr);
            repr << ") or (";
            auto filter2 = createFlightFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Flight* const &flight) {
                return filter1(flight) || filter2(flight);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createFlightFilter(repr);
            repr << ") and (";
            auto filter2 = createFlightFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Flight* const &flight) {
                return filter1(flight) && filter2(flight);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(plane);
        menu.addBlock(airports);
        menu.addBlock(tickets);
        menu.addBlock(booleanLogic);

        menu.show();
        return filter;
    }
    
    void filterFlightsWithUserInput(vector<Flight*> &pool) {
        ostringstream filter_repr;
        function<bool(const Flight* const&)> filter = createFlightFilter(filter_repr);

        cout << "\x1B[2J\x1B[;H\x1B[32m✓\x1B[0m " << "Your filter: " << filter_repr.str() << '\n' << endl;
        waitForInput();

        for (auto it = pool.begin(); it != pool.end();) {
            if (!filter(*it))
                it = pool.erase(it);
            else
                it++;
        }
    }

    void orderFlightsWithUserInput(vector<Flight*> &pool) {
        Menu menu("Please select an attribute to sort the selected flights:");

        MenuBlock block;
        block.addOption("Flight ID", [&pool]() {
            utils::sort<Flight*, string>(pool, [](Flight* const &flight) {
                return flight->getFlightId();
            });
        });

        block.addOption("Departure time", [&pool]() {
            utils::sort<Flight*, Datetime>(pool, [](Flight* const &flight) {
                return flight->getDepartureTime();
            });
        });
        
        block.addOption("Duration", [&pool]() {
            utils::sort<Flight*, Time>(pool, [](Flight* const &flight) {
                return flight->getDuration();
            });
        });

        block.addOption("Number of tickets", [&pool]() {
            utils::sort<Flight*, size_t>(pool, [](Flight* const &flight) {
                return flight->getTickets().size();
            });
        });

        block.addOption("Origin airport name", [&pool]() {
            utils::sort<Flight*, string>(pool, [](Flight* const &flight) {
                return flight->getOrigin().getName();
            });
        });

        block.addOption("Destination airport name", [&pool]() {
            utils::sort<Flight*, string>(pool, [](Flight* const &flight) {
                return flight->getDestination().getName();
            });
        });
    }

    void readAllFlightsWithUserInput() {
        vector<Flight*> pool = data::flights;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterFlightsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderFlightsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # flights", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # flights", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);   
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no flights left!\n");
            else
                menu.show(getFlightRepresentation(pool));
        }
    }

    /**
     * @brief Updates one Plane instance
     */
    void updateFlight() {
        Flight &flight = findFlight();

        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Departure time", [&flight]() {
            Datetime datetime = Datetime::readFromString(
                readValue<GetLine>("Date and time of departure: ", "Please insert a valid date and time", [&flight](const string &value) {
                    Datetime datetime = Datetime::readFromString(value);
                    Flight *flight2 = findFlightByKey(flight.getFlightId(), datetime);

                    if (flight2 != nullptr)
                        throw validation_error("There is already a plane with that flight ID scheduled to depart at that time");

                    return true;
                })
            );
            
            flight.setDepartureTime(datetime);
        });

        choice.addOption("Duration", [&flight]() {
            Time duration = Time::readFromString(
                readValue<GetLine>("Duration of the flight: ", "Please insert a valid time", [](const string &value) {
                    Time::readFromString(value);
                    return true;  
                })
            );

            flight.setDuration(duration);
        });

        choice.addOption("Origin airport", [&flight]() {
            if (data::airports.size() <= 2) {
                cout << "You must have 3 or more airports to change this flight's destination airport\n" << endl;
                waitForInput();
                return;
            }

            Airport &airport = *findAirportByName(
                readValue<GetLine>("Airport name: ", "Please provide a valid airport name", [&flight](const string &value) {
                    Airport *airport = findAirportByName(value);
                    if (airport == nullptr)
                        return false;

                    if (value == flight.getDestination().getName())
                        throw validation_error("You can't choose the destination airport for origin airport");
                })
            );

            flight.setOrigin(airport);
        });
        
        choice.addOption("Destination airport", [&flight]() {
            if (data::airports.size() <= 2) {
                cout << "You must have 3 or more airports to change this flight's destination airport\n" << endl;
                waitForInput();
                return;
            }

            Airport &airport = *findAirportByName(
                readValue<GetLine>("Airport name: ", "Please provide a valid airport name", [&flight](const string &value) {
                    Airport *airport = findAirportByName(value);
                    if (airport == nullptr)
                        return false;
                        
                    if(value == flight.getOrigin().getName())
                        throw validation_error("You can't choose the origin airport for destination airport");

                    return true;
                })
            );

            flight.setDestination(airport);
        });

        MenuBlock tickets;
        tickets.addOption("Manage tickets", [&flight]() {
            manageTickets(flight);
        });

        bool is_running = true;
        
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        menu.addBlock(choice);
        menu.addBlock(tickets);
        menu.setSpecialBlock(special_block);
    
        while (is_running) {
            menu.show(flight.str() + '\n');
        }
    }

    
    /**
     * @brief Deletes one Plane instance specified by the user
     */
    void deleteOneFlight() {
        Flight const &flight = findFlight();
        for (auto it = data::flights.begin(), end = data::flights.end(); it != end; it++) {
            if (*it == &flight) {
                data::flights.erase(it);
                delete &flight;

                waitForInput();
                return;
            }
        }

        delete &flight;
        throw logic_error("No flights were deleted");
    }

    /**
     * @brief Deletes every Plane instance
     */
    void deleteAllFlights() {
        for (const Flight *flight : data::flights)
            delete flight;

        data::flights.clear();
    }

    void deleteAllFlightsWithUserInput() {
        vector<Flight*> pool = data::flights;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterFlightsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderFlightsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # flights", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # flights", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        MenuBlock erase;
        erase.addOption("Delete all flights in this selection", [&pool]() {
            vector<Flight*> new_flights;
            
            for (Flight *flight1 : data::flights) {
                bool was_selected = false;

                for (Flight *flight2 : pool) {
                    if (flight1 == flight2)
                        was_selected = true;
                        break;
                }

                if (was_selected)
                    delete flight1;
                else
                    new_flights.push_back(flight1);
            }

            data::flights = pool = new_flights;
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
                menu.addBlock(erase);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no flights left!\n");
            else
                menu.show(getFlightRepresentation(pool));
        }
    }

    void manageFlights() {
        Menu menu("Select one of the following operations:");

        auto allowWhenFlightsExist = [](const function<void()> &func) {
            return [func]() {
                if (data::flights.empty()) {
                    cout << "There are no flights in the database\n" << endl;
                    waitForInput();
                    return;
                }

                func();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new flight", createFlight);
        normal.addOption("Update flight", allowWhenFlightsExist(updateFlight));

        MenuBlock ohno;
        ohno.addOption("Read one flight", allowWhenFlightsExist(readOneFlight));
        ohno.addOption("Read all flights", allowWhenFlightsExist(readAllFlights));
        ohno.addOption("Read all flights with filters and sort", allowWhenFlightsExist(readAllFlightsWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one flight", allowWhenFlightsExist(deleteOneFlight));
        remove.addOption("Delete all flights", allowWhenFlightsExist(deleteAllFlights));
        remove.addOption("Delete all flights with filters and sort", allowWhenFlightsExist(deleteAllFlightsWithUserInput));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    /*----------TICKETS----------*/

    Ticket* findTicketsBySeatNumber(Flight &flight, unsigned int seat_number) {
        if (seat_number >= flight.getPlane().getCapacity())
            throw validation_error("Seat number must be smaller than the plane's capacity");

        vector<Ticket*> tickets = flight.getTickets();
        return utils::binarySearch<Ticket*, unsigned int>(tickets, seat_number, [](Ticket* ticket) {
            return ticket->getSeatNumber();
        });
    }

    Ticket* findTicketByFlightAndCustomer(Flight &flight, const string &name) {
        for (Ticket* ticket : flight.getTickets()) {
            if (ticket->getCustomerName() == name) {
                return ticket;
            }
        }

        return nullptr;
    }

    unsigned int askUnusedSeatNumber(Flight &flight) {
        return readValue<unsigned int>("Seat number: ", "Please insert a valid seat number", [&flight](const unsigned int seat_number) {
            return findTicketsBySeatNumber(flight, seat_number) == nullptr;
        });
    }

    unsigned int askUsedSeatNumber(Flight &flight) {
        return readValue<unsigned int>("Seat number: ", "Please insert a valid seat number", [&flight](const unsigned int &seat_number) {
            return findTicketsBySeatNumber(flight, seat_number) != nullptr;
        });
    }

    Ticket &findTicket(Flight &flight) {
        if (flight.getTickets().empty()) {
            throw logic_error("No tickets found");
        }

        if (flight.getTickets().size() == 1) {
            return *flight.getTickets().at(0);
        }

        unsigned int seat_number = askUsedSeatNumber(flight);
        cout << endl;

        Ticket *ticket = findTicketsBySeatNumber(flight, seat_number);
        if (ticket == nullptr)
            throw logic_error("No ticket found");

        return *ticket;
    }

    void createTicket(Flight &flight) {
        string name = readValue<GetLine>("Name: ","Please insert a valid name");
        unsigned int age = readValue<unsigned int>("Age: ", "Please insert a valid age");
        unsigned int seat_number = askUnusedSeatNumber(flight);
        cout << endl;
        
        Ticket *ticket = new Ticket(flight, name, age, seat_number);

        unsigned int number_luggage = readValue<unsigned int>("Number of luggage pieces: ", "Please provide a valid number of luggage pieces");
        cout << endl;

        for (unsigned int i = 0; i < number_luggage; i++) {
            float weight = readValue<float>("Weight of piece of luggage #" + to_string(i + 1) + ": ", "Please provide a valid weight", [](const float &value) {
                return value > 0;
            });

            Menu aproval("Submit luggage to automatic check-in?");
            
            MenuBlock choice;
            choice.addOption("Yes", [&flight, &ticket, &weight]() {
                Luggage *luggage = new Luggage(*ticket, weight);
                for (const auto &car : data::handlingCars) {
                    if (car->getFlight() == &flight) {
                        if (car->addLuggage(*luggage)) {
                            cout << "The luggage was sucessfuly loaded into Car #" << car->getId() << '\n' << endl;
                            waitForInput();
                            return;
                        }
                    }
                }

                cout << "There are no cars with space for the luggage piece!\n" << endl;
                waitForInput();

                delete luggage;
            });

            choice.addOption("No", []() {});

            aproval.addBlock(choice);
            aproval.show();
        }

        flight.addTicket(*ticket);
        waitForInput();
    }
    
    void readOneTicket(Flight &flight) {
        const Ticket &ticket = findTicket(flight);
        cout << ticket << endl;
        waitForInput();
    }

    string getTicketRepresentation(const vector<Ticket*> &vec) {
        ostringstream repr;
        
        bool is_empty = true;
        for (const Ticket *ticket : vec) {
            if (!is_empty)
                repr << '\n';

            repr << *ticket;
            is_empty = false;
        }

        return repr.str();
    }

    void readAllTickets(Flight &flight) {
        cout << getTicketRepresentation(flight.getTickets()) << endl;
        waitForInput();
    }

    function<bool(const Ticket* const&)> createTicketFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const Ticket* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "customer name", [&filter, &repr]() {
            repr << "customer name ";
            filter = createFilter<const Ticket*, string>(repr, [](const Ticket* const &value) {
                return value->getCustomerName();
            });
        });

        directAttributes.addOption(repr.str() + "customer age", [&filter, &repr]() {
            repr << "customer age ";
            filter = createFilter<const Ticket*, unsigned int>(repr, [](const Ticket* const &value) {
                return value->getCustomerAge();
            });
        });

        directAttributes.addOption(repr.str() + "seat number", [&filter, &repr]() {
            repr << "seat number ";
            filter = createFilter<const Ticket*, unsigned int>(repr, [](const Ticket* const &value) {
                return value->getSeatNumber();
            });
        });

        MenuBlock flight;
        flight.addOption(repr.str() + "flight has", [&filter, &repr]() {
            repr << "flight has ";
            auto flightFilter = createFlightFilter(repr);
            filter = [flightFilter](const Ticket* const &ticket) {
                return flightFilter(&ticket->getFlight());
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createTicketFilter(repr);
            repr << ')';

            filter = [filter1](const Ticket* const &ticket) {
                return !filter1(ticket);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createTicketFilter(repr);
            repr << ") or (";
            auto filter2 = createTicketFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Ticket* const &ticket) {
                return filter1(ticket) || filter2(ticket);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createTicketFilter(repr);
            repr << ") and (";
            auto filter2 = createTicketFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Ticket* const &ticket) {
                return filter1(ticket) && filter2(ticket);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(flight);
        menu.addBlock(booleanLogic);

        menu.show();
        return filter;
    }

    void filterTicketsWithUserInput(vector<Ticket*> &pool) {
        ostringstream filter_repr;
        function<bool(const Ticket* const&)> filter = createTicketFilter(filter_repr);

        cout << "\x1B[2J\x1B[;H\x1B[32m✓\x1B[0m " << "Your filter: " << filter_repr.str() << '\n' << endl;
        waitForInput();

        for (auto it = pool.begin(); it != pool.end();) {
            if (!filter(*it))
                it = pool.erase(it);
            else
                it++;
        }
    }

    void orderTicketsWithUserInput(vector<Ticket*> &pool) {
        Menu menu("Please select an attribute to sort the selected tickets:");

        MenuBlock block;
        block.addOption("Customer Name", [&pool]() {
            utils::sort<Ticket*, string>(pool, [](Ticket* const &ticket) {
                return ticket->getCustomerName();
            });
        });

        block.addOption("Customer Age", [&pool]() {
            utils::sort<Ticket*, unsigned int>(pool, [](Ticket* const &ticket) {
                return ticket->getCustomerAge();
            });
        });

        block.addOption("Seat Number", [&pool]() {
            utils::sort<Ticket*, unsigned int>(pool, [](Ticket* const &ticket) {
                return ticket->getSeatNumber();
            });
        });

        menu.addBlock(block);
        menu.show();
    }

    void readAllTicketsWithUserInput(Flight &flight) {
        vector<Ticket*> pool = flight.getTickets();

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterTicketsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderTicketsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # tickets", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # tickets", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no tickets left!\n");
            else
                menu.show(getTicketRepresentation(pool));
        }
    }

    void updateTicket(Flight &flight) {
        Ticket &ticket = findTicket(flight);

        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Seat number", [&ticket, &flight]() {
            unsigned int seat_number = readValue<unsigned int>("Seat number: ", "Please insert a valid seat number", [&flight](const unsigned int &number) {
                return number < flight.getPlane().getCapacity();
            });

            Ticket* new_ticket = new Ticket(ticket.getFlight(), ticket.getCustomerName(), ticket.getCustomerAge(), seat_number);

            if (!flight.removeTicket(ticket)) {
                delete &ticket;
                throw logic_error("No ticket was removed");
            }

            delete &ticket;

            if (!flight.addTicket(*new_ticket)) {
                delete new_ticket;
                throw logic_error("No ticket was added");
            }
        });

        choice.addOption("Customer name", [&ticket, &flight](){
            string name = readValue<GetLine>("Customer name: ", "Please insert a valid customer name");
            ticket.setCustomerName(name);
        });

        choice.addOption("Customer age", [&ticket, &flight]() {
            unsigned int age = readValue<unsigned int>("Customer age: ", "Please insert a valid customer age");
            ticket.setCustomerAge(age);
        });

        bool is_running = true;

        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);

        while (is_running) {
            menu.show(ticket.str());
        }
    }

    void deleteOneTicket(Flight &flight) {
        Ticket const &ticket = findTicket(flight);

        if (flight.removeTicket(ticket)) {
            delete &ticket;

            waitForInput();
            return;
        }

        delete &ticket;
        throw logic_error("No tickets were deleted");
    }

    void deleteAllTickets(Flight &flight) {
        for (const Ticket *ticket : flight.getTickets()) {
            delete ticket;
        }

        flight.clearTickets();
    }

    void deleteAllTicketsWithUserInput(Flight &flight) {
        vector<Ticket*> pool = flight.getTickets();

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterTicketsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderTicketsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # tickets", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # tickets", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        MenuBlock erase;
        erase.addOption("Delete all tickets in this selection", [&pool, &flight]() {
            vector<Ticket*> new_tickets;
            
            for (Ticket *ticket1 : flight.getTickets()) {
                bool was_selected = false;

                for (Ticket *ticket2 : pool) {
                    if (ticket1 == ticket2) {
                        was_selected = true;
                        break;
                    }
                }

                if (was_selected)
                    delete ticket1;
                else
                    new_tickets.push_back(ticket1);
            }

            pool = new_tickets;
            flight.clearTickets();
            
            for (Ticket* ticket : new_tickets) {
                if (!flight.addTicket(*ticket))
                    throw logic_error("No ticket was added");
            }
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
                menu.addBlock(erase);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no tickets left!\n");
            else
                menu.show(getTicketRepresentation(pool));
        }
    }

    void manageTickets(Flight &flight) {
        Menu menu("Select one of the following operations:");

        auto allowWhenTicketsExist = [&flight](const function<void()> &func) {
            return [&flight, func]() {
                if (flight.getTickets().empty()) {
                    cout << "There are no tickets for this flight\n" << endl;
                    waitForInput();
                    return;
                }

                func();
            };
        };

        auto callWithFlight = [](Flight &flight, const function<void(Flight&)> &func) {
            return [&flight, func]() {
                func(flight);
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new ticket", callWithFlight(flight, createTicket));
        normal.addOption("Update ticket", allowWhenTicketsExist(callWithFlight(flight, updateTicket)));

        MenuBlock ohno;
        ohno.addOption("Read one ticket", allowWhenTicketsExist(callWithFlight(flight, readOneTicket)));
        ohno.addOption("Read all tickets", allowWhenTicketsExist(callWithFlight(flight, readAllTickets)));
        ohno.addOption("Read all tickets with filters and sort", allowWhenTicketsExist(callWithFlight(flight, readAllTicketsWithUserInput)));

        MenuBlock remove;
        remove.addOption("Delete one ticket", allowWhenTicketsExist(callWithFlight(flight, deleteOneTicket)));
        remove.addOption("Delete all tickets", allowWhenTicketsExist(callWithFlight(flight, deleteAllTickets)));
        remove.addOption("Delete all tickets with filters and sort", allowWhenTicketsExist(callWithFlight(flight, deleteAllTicketsWithUserInput)));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    /*----------AIRPORTS----------*/

    Airport* findAirportByName(const string name) {
        return utils::binarySearch<Airport*, string>(data::airports, name, [](Airport *airport) {
            return airport->getName();
        });
    }

    string askUnusedName() {
        return readValue<GetLine>("Airport name: ", "Please insert a valid airport name",[](const string &value) {
            Airport *airport = findAirportByName(value);
            if (airport != nullptr)
                throw validation_error("There is already an airport with that name");
                
            return true;
        });
    }
    
    string askUsedName() {
        return readValue<GetLine>("Airport name: ", "Please insert a valid airport name", [](const string &value) {
            Airport *airport = findAirportByName(value);
            if (airport == nullptr)
                throw validation_error("There is no airport with that name");

            return true;
        });
    }

    Airport &findAirport() {
        if (data::airports.empty())
            throw logic_error("No airport found");

        if (data::airports.size() == 1) {
            return *data::airports.at(0);
        }

        string name = askUsedName();
        cout << endl;

        Airport *airport = findAirportByName(name);
        if (airport == nullptr)
            throw logic_error("No airport found");

        return *airport;
    }

    void createAirport() {
        string name = askUnusedName();
        cout << endl;
        
        Airport *airport = new Airport(name);
        auto pos = utils::lowerBound<Airport*, string>(data::airports, name, [](Airport* airport) {
            return airport->getName();
        });

        data::airports.insert(pos, airport);
        waitForInput();
    }

    void readOneAirport() {
        const Airport &airport = findAirport();
        cout << airport << endl;

        set<TransportPlace> transportinfo = airport.getTransportPlaceInfo();

        if (!transportinfo.empty()) {
            cout << "\nTransport Info:\n";
            for (const auto &info : transportinfo) {
                switch (info.transport_type) {
                    case TransportType::SUBWAY:
                        cout << '\n' << "Type: Subway" << '\n';
                        break;
                    
                    case TransportType::BUS:
                        cout << '\n' << "Type: Bus" << '\n';
                        break;
                        
                    case TransportType::TRAIN:
                        cout << '\n' << "Type: Train" << '\n'; 
                        break;
                    
                    default:
                        throw runtime_error("Unknown transportation type");
                }

                cout << "Name: " << info.name << '\n'
                     << "Airport distance: " << info.airport_distance << " km\n"
                     << "Location: " << info.latitude << ", " << info.longitude << '\n';
                             
                cout << "Schedule:\n";
                for (auto time : info.schedule) {
                    cout << "  " << time.str() << '\n';
                }
                        
                cout << endl;
            }
        }

        cout << flush;
        waitForInput();
    }

    string getAirportRepresentation(const vector<Airport*> &vec) {
        ostringstream repr;
        
        bool is_empty = true;
        for (const Airport *airport : vec) {
            if (!is_empty)
                repr << '\n';

            repr << *airport;
            is_empty = false;
        }

        return repr.str();
    }

    void readAllAirports() {
        cout << getAirportRepresentation(data::airports) << endl;
        waitForInput();
    }

    function<bool(const Airport* const&)> createAirportFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const Airport* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "name", [&filter, &repr]() {
            repr << "name ";
            filter = createFilter<const Airport*, string>(repr, [](const Airport* const &value) {
                return value->getName();
            });
        });

        directAttributes.addOption(repr.str() + "number of stops", [&filter, &repr]() {
            repr << "number of stops ";
            filter = createFilter<const Airport*, unsigned int>(repr, [](const Airport* const &value) {
                return value->getTransportPlaceInfo().size();
            });
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createAirportFilter(repr);
            repr << ')';

            filter = [filter1](const Airport* const &airport) {
                return !filter1(airport);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createAirportFilter(repr);
            repr << ") or (";
            auto filter2 = createAirportFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Airport* const &airport) {
                return filter1(airport) || filter2(airport);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createAirportFilter(repr);
            repr << ") and (";
            auto filter2 = createAirportFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Airport* const &airport) {
                return filter1(airport) && filter2(airport);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(booleanLogic);

        menu.show();
        return filter;
    }

    void filterAirportsWithUserInput(vector<Airport*> &pool) {
        ostringstream filter_repr;
        function<bool(const Airport* const&)> filter = createAirportFilter(filter_repr);

        cout << "\x1B[2J\x1B[;H\x1B[32m✓\x1B[0m " << "Your filter: " << filter_repr.str() << '\n' << endl;
        waitForInput();

        for (auto it = pool.begin(); it != pool.end();) {
            if (!filter(*it))
                it = pool.erase(it);
            else
                it++;
        }
    }

    void orderAirportsWithUserInput(vector<Airport*> &pool) {
        Menu menu("Please select an attribute to sort the selected airports:");

        MenuBlock block;
        block.addOption("Name", [&pool]() {
            utils::sort<Airport*, string>(pool, [](Airport* const &airport) {
                return airport->getName();
            });
        });

        block.addOption("Number of stops", [&pool]() {
            utils::sort<Airport*, unsigned int>(pool, [](Airport* const &airport) {
                return airport->getTransportPlaceInfo().size();
            });
        });

        menu.addBlock(block);
        menu.show();
    }

    void readAllAirportsWithUserInput(){
        vector<Airport*> pool = data::airports;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterAirportsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderAirportsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # airports", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # airports", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no airports left!\n");
            else
                menu.show(getAirportRepresentation(pool));
        }
    }

    void updateAirport() {
        Airport &airport = findAirport();
        
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Add transportation stop", [&airport]() {
            string name = readValue<GetLine>("Name: ", "Please insert a valid name");
            
            float latitude = readValue<float>("Latitude: ", "Please insert a valid latitude", [](const float &value) {
                return value >= -90 && value <= 90;
            });

            
            float longitude = readValue<float>("Longitude: ", "Please insert a valid longitude", [](const float &value) {
                return value >= -180 && value <= 180;
            });
            
            float airport_distance = readValue<float>("Distance to airport (in km): ", "Please insert a valid distance", [](const float &value)  {
                return value > 0;
            });
            
            set<Time> schedule;

            unsigned schedule_size = readValue<unsigned int>("How many entries you want to add to the schedule: ", "Please insert a valid number");
            for (int i = 0; i < schedule_size; i++) {
                Time time = Time::readFromString(
                    readValue<string>("Time " + to_string(i + 1) + ": ", "Please insert a valid time", [&schedule](const string &value) {
                        Time time = Time::readFromString(value);
                        if (schedule.find(time) != schedule.end())
                            throw validation_error("An entry with that timestamp already exists");

                        return true;
                    })
                );
                
                schedule.insert(time);
            }

            TransportType type;

            Menu transportTypeMenu("Please select a transportation type:");

            MenuBlock block;
            block.addOption("Bus", [&type]() { type = TransportType::BUS; });
            block.addOption("Subway", [&type]() { type = TransportType::SUBWAY; });
            block.addOption("Train", [&type]() { type = TransportType::TRAIN; });

            transportTypeMenu.addBlock(block);
            transportTypeMenu.show();

            TransportPlace place = { name, latitude, longitude, type, airport_distance, schedule };
            airport.addTransportPlaceInfo(place);
        });

        choice.addOption("Remove stop", [&airport]() {
            if (airport.getTransportPlaceInfo().size() == 0) {
                cout << "There are no stops to remove\n" << endl;
                waitForInput();
                return;
            }

            if (airport.getTransportPlaceInfo().size() == 1) {
                airport.removeAllTransportPlaceInfo();
                return;
            }

            string name = readValue<GetLine>("Name: ", "Please insert a valid name", [&airport](const string &value) {
                for (const auto &info : airport.getTransportPlaceInfo()) {
                    if (info.name == value)
                        return true;
                }

                throw validation_error("There are no stops with that name");
            });

            airport.removeTransportPlaceInfo(name);
        });
        
        choice.addOption("Remove all stops", [&airport]() { airport.removeAllTransportPlaceInfo(); });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() {is_running = false;});

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);

        while (is_running) {
            menu.show(airport.str() + '\n');
        }
    }

    void deleteOneAirport() {
        Airport const &airport = findAirport();
        for (auto it = data::airports.begin(), end = data::airports.end(); it != end; it++) {
            if (*it == &airport) {
                data::airports.erase(it);
                delete &airport;

                waitForInput();
                return;
            }
        }

        delete &airport;
        throw logic_error("No airports were deleted");
    }

    void deleteAllAirports() {
        for (const Airport *airport : data::airports)
            delete airport;

        data::airports.clear();
    }

    void deleteAllAirportsWithUserInput(){
        vector<Airport*> pool = data::airports;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterAirportsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderAirportsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # airports", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # airports", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        MenuBlock erase;
        erase.addOption("Delete all planes in this selection", [&pool]() {
            vector<Airport*> new_airports;
            
            for (Airport *airport1 : data::airports) {
                bool was_selected = false;

                for (Airport *airport2 : pool) {
                    if (airport1 == airport2)
                        was_selected = true;
                        break;
                }

                if (was_selected)
                    delete airport1;
                else
                    new_airports.push_back(airport1);
            }

            data::airports = pool = new_airports;
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);
                menu.addBlock(erase);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no airports left!\n");
            else
                menu.show(getAirportRepresentation(pool));
        }
    }

    void manageAirports() {
        Menu menu("Select one of the following operations:");

        auto allowWhenAirportsExist = [](const function<void()> &func) {
            return [func]() {
                if (data::airports.empty()) {
                    cout << "There are no airports in the database\n" << endl;
                    waitForInput();
                    return;
                }

                func();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new airport", createAirport);
        normal.addOption("Update airport", allowWhenAirportsExist(updateAirport));

        MenuBlock ohno;
        ohno.addOption("Read one airport", allowWhenAirportsExist(readOneAirport));
        ohno.addOption("Read all airports", allowWhenAirportsExist(readAllAirports));
        ohno.addOption("Read all airports with filters and sort", allowWhenAirportsExist(readAllAirportsWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one airport", allowWhenAirportsExist(deleteOneAirport));
        remove.addOption("Delete all airports", allowWhenAirportsExist(deleteAllAirports));
        remove.addOption("Delete all airports with filters and sort", allowWhenAirportsExist(deleteAllAirportsWithUserInput));


        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    /* HANDLING CARS */

    HandlingCar* findCarById(const unsigned &id) {
        return utils::binarySearch<HandlingCar*, unsigned>(data::handlingCars, id, [](HandlingCar *car) {
            return car->getId();
        });
    }

    unsigned int askUsedId() {
        return readValue<unsigned int>("ID: ", "Please input a valid ID", [](const unsigned int &value) {
            HandlingCar *car = findCarById(value);
            if (car == nullptr)
                throw validation_error("That ID does not belong to any handling car");

            return true;
        });
    }
    
    HandlingCar &findCar() {
        if (data::handlingCars.empty())
            throw logic_error("No handling car found");

        if (data::handlingCars.size() == 1) {
            return *data::handlingCars.at(0);
        }

        unsigned int id = askUsedId();
        cout << endl;

        HandlingCar *car = findCarById(id);
        if (car == nullptr)
            throw logic_error("No handling car found");
    
        return *car;
    }

    void createHandlingCar() {
        unsigned int number_of_carriages = readValue<unsigned>("Number of carriages: ", "Please input a valid number", [](const unsigned int &value) { return value > 0; });
        unsigned int stacks_per_carriage = readValue<unsigned>("Number of stacks in each carriage: ", "Please input a valid number", [](const unsigned int &value) { return value > 0; });
        unsigned int luggage_per_stack = readValue<unsigned>("Number of luggage per stack: ", "Please insert a valid number", [](const unsigned int &value) { return value > 0; });
        cout << endl;

        HandlingCar *car = new HandlingCar(number_of_carriages, stacks_per_carriage, luggage_per_stack);
        auto pos = utils::lowerBound<HandlingCar*, unsigned int>(data::handlingCars, car->getId(), [](HandlingCar *car) {
            return car->getId();
        });

        data::handlingCars.insert(pos, car);
        waitForInput();
    }

    void updateHandlingCar() {
        HandlingCar &car = findCar();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Change flight to load luggage into", [&car]() {
            Flight &flight = findFlight();
            car.setFlight(flight);

            cout << "Successfully set the flight to load luggage into\n" << endl;
            waitForInput();
        });

        choice.addOption("Unload handling car", [&car]() {
            if (car.getFlight() == nullptr) {
                cout << "This handling car does not have a flight to load luggage into\n" << endl;
                waitForInput();
                return;
            }

            Luggage *luggage;
            while((luggage = car.unloadNextLuggage()) != nullptr)
                car.getFlight()->addLuggage(*luggage);

            cout << "Every piece of luggage was unloaded into the plane\n" << endl;
            waitForInput();
        });
        
        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);

        while (is_running) {
            menu.show(car.str() + '\n');
        }
    }

    void readOneCar() {
        const HandlingCar &car = findCar();
        cout << car << endl;
        waitForInput();
    }

    string getCarsRepresentation(const vector<HandlingCar*> &vec) {
        ostringstream repr;
        
        bool is_empty = true;
        for (const HandlingCar *car : vec) {
            if (!is_empty)
                repr << '\n';

            repr << *car;
            is_empty = false;
        }

        return repr.str();
    }

    void readAllCars() {
        cout << getCarsRepresentation(data::handlingCars) << endl;
        waitForInput();
    }

    function<bool(const HandlingCar* const&)> createCarFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const HandlingCar* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "number of carriages", [&filter, &repr]() {
            repr << "number of carriages ";
            filter = createFilter<const HandlingCar*, unsigned int>(repr, [](const HandlingCar* const &value) {
                return value->getNumberOfCarriages();
            });
        });

        directAttributes.addOption(repr.str() + "number of stacks per carriage", [&filter, &repr]() {
            repr << "number of stacks per carriage ";
            filter = createFilter<const HandlingCar*, unsigned int>(repr, [](const HandlingCar* const &value) {
                return value->getStacksPerCarriage();
            });
        });

        directAttributes.addOption(repr.str() + "number of luggage per carriage", [&filter, &repr]() {
            repr << "number of luggage per carriage ";
            filter = createFilter<const HandlingCar*, unsigned int>(repr, [](const HandlingCar* const &value) {
                return value->getLuggagePerStack();
            });
        });

        MenuBlock flight;
        flight.addOption(repr.str() + "flight has", [&filter, &repr]() {
            repr << "flight has ";

            auto flightFilter = createFlightFilter(repr);
            filter = [flightFilter](const HandlingCar* const& car) {
                if (car->getFlight() == nullptr)
                    return false;

                return flightFilter(car->getFlight());
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createCarFilter(repr);
            repr << ')';

            filter = [filter1](const HandlingCar* const &car) {
                return !filter1(car);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createCarFilter(repr);
            repr << ") or (";
            auto filter2 = createCarFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const HandlingCar* const &car) {
                return filter1(car) || filter2(car);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createCarFilter(repr);
            repr << ") and (";
            auto filter2 = createCarFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const HandlingCar* const &car) {
                return filter1(car) && filter2(car);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(flight);
        menu.addBlock(booleanLogic);
        menu.show();

        return filter;
    }

    void filterCarsWithUserInput(vector<HandlingCar*> &pool) {
        ostringstream filter_repr;
        function<bool(const HandlingCar* const&)> filter = createCarFilter(filter_repr);

        cout << "\x1B[2J\x1B[;H\x1B[32m✓\x1B[0m " << "Your filter: " << filter_repr.str() << '\n' << endl;
        waitForInput();

        for (auto it = pool.begin(); it != pool.end();) {
            if (!filter(*it))
                it = pool.erase(it);
            else
                it++;
        }
    }

    void orderCarsWithUserInput(vector<HandlingCar*> &pool) {
        Menu menu("Please select an attribute to sort the selected planes:");

        MenuBlock block;
        block.addOption("Number of carriages", [&pool]() {
            utils::sort<HandlingCar*, unsigned int>(pool, [](HandlingCar* const &car) {
                return car->getNumberOfCarriages();
            });
        });

        block.addOption("Number of stacks per carriage", [&pool]() {
            utils::sort<HandlingCar*, unsigned int>(pool, [](HandlingCar* const &car) {
                return car->getStacksPerCarriage();
            });
        });

        block.addOption("Number of luggage per stack", [&pool]() {
            utils::sort<HandlingCar*, unsigned int>(pool, [](HandlingCar* const &car) {
                return car->getLuggagePerStack();
            });
        });

        menu.addBlock(block);
        menu.show();
    }

    void readAllCarsWithUserInput() {
        vector<HandlingCar*> pool = data::handlingCars;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterCarsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderCarsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # handling cars", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # handling cars", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);   
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no handling cars left!\n");
            else
                menu.show(getCarsRepresentation(pool));
        }
    }

    void deleteOneCar() {
        HandlingCar const &car = findCar();
        for (auto it = data::handlingCars.begin(), end = data::handlingCars.end(); it != end; it++) {
            if (*it == &car) {
                data::handlingCars.erase(it);
                delete &car;
                waitForInput();
                return;
            }
        }

        delete &car;
        throw logic_error("No cars were deleted");
    }

    void deleteAllCars() {
        for (const HandlingCar *car : data::handlingCars)
            delete car;

        data::handlingCars.clear();
    }

    void deleteAllCarsWithUserInput() {
        vector<HandlingCar*> pool = data::handlingCars;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterCarsWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderCarsWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # handling cars", [&pool]() { utils::sliceVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # handling cars", [&pool]() { utils::sliceVectorFromEndWithUserInput(pool); });

        other_ops.addOption("Reverse the current order", [&pool]() {
            utils::reverse(pool);
        });

        MenuBlock erase;
        erase.addOption("Delete all planes in this selection", [&pool]() {
            vector<HandlingCar*> new_cars;
            
            for (HandlingCar *car1 : data::handlingCars) {
                bool was_selected = false;

                for (HandlingCar *car2 : pool) {
                    if (car1 == car2)
                        was_selected = true;
                        break;
                }

                if (was_selected)
                    delete car1;
                else
                    new_cars.push_back(car1);
            }

            data::handlingCars = pool = new_cars;
        });

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() { is_running = false; });

        while (is_running) {
            Menu menu("Your current selection:");

            if (!pool.empty()) {
                menu.addBlock(ops);
                menu.addBlock(other_ops);   
                menu.addBlock(erase);
            }

            menu.setSpecialBlock(special_block);

            if (pool.empty())
                menu.show("\x1B[31m>>\x1B[0m There are no handling cars left!\n");
            else
                menu.show(getCarsRepresentation(pool));
        }
    }

    
    void manageHandlingCars() {
        Menu menu("Select one of the following operations:");

        auto allowWhenCarsExist = [](const function<void()> &f) {
            return [f]() {
                if (data::handlingCars.empty()) {
                    cout << "There are no handling cars in the database\n" << endl;
                    waitForInput();
                    return;
                }
                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new handling car", createHandlingCar);
        normal.addOption("Update an handling car", allowWhenCarsExist(updateHandlingCar));

        MenuBlock ohno;
        ohno.addOption("Read one handling car", allowWhenCarsExist(readOneCar));
        ohno.addOption("Read all handling cars", allowWhenCarsExist(readAllCars));
        ohno.addOption("Read all handling cars with filters and sort", allowWhenCarsExist(readAllCarsWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one handling car", allowWhenCarsExist(deleteOneCar));
        remove.addOption("Delete all handling cars", allowWhenCarsExist(deleteAllCars));
        remove.addOption("Delete all handling cars with filters and sort", allowWhenCarsExist(deleteAllCarsWithUserInput));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });
        
        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    function<bool(const Service* const&)> createServiceFilter(ostringstream &repr) {
        Menu menu("Please specify a value to use as a filter:");
        function<bool(const Service* const&)> filter;

        MenuBlock directAttributes;
        directAttributes.addOption(repr.str() + "type", [&filter, &repr]() {
            repr << "type ";
            filter = createFilter<const Service*, string>(repr, [](const Service* const &value) {
                switch (value->getType()) {
                    case ServiceType::MAINTENANCE:
                        return "maintenance";
                    case ServiceType::CLEANING:
                        return "cleaning";
                };

                return "unknown";
            });
        });

        directAttributes.addOption(repr.str() + "worker", [&filter, &repr]() {
            repr << "worker ";
            filter = createFilter<const Service*, string, GetLine>(repr, [](const Service* const &value) {
                return value->getWorker();
            });
        });

        directAttributes.addOption(repr.str() + "date", [&filter, &repr]() {
            repr << "date ";
            filter = createFilter<const Service*, string>(repr, [](const Service* const &value) {
                return value->getDatetime().str();
            });
        });

        MenuBlock plane;
        plane.addOption(repr.str() + "the plane has", [&filter, &repr]() {
            repr << "the plane has ";

            auto planeFilter = createPlaneFilter(repr);
            filter = [planeFilter](const Service* const &service) {
                return planeFilter(&service->getPlane());
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << "not (";
            auto filter1 = createServiceFilter(repr);
            repr << ')';

            filter = [filter1](const Service* const &service) {
                return !filter1(service);
            };
        });

        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createServiceFilter(repr);
            repr << ") or (";
            auto filter2 = createServiceFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Service* const &service) {
                return filter1(service) || filter2(service);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createServiceFilter(repr);
            repr << ") and (";
            auto filter2 = createServiceFilter(repr);
            repr << ')';

            filter = [filter1, filter2](const Service* const &service) {
                return filter1(service) && filter2(service);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(plane);
        menu.addBlock(booleanLogic);
        menu.show();

        return filter;
    }
}