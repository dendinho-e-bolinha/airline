#include "crud.h"
#include "utils.h"
#include "interact.h"
#include "state.h"
#include <set>
#include <algorithm>
#include <sstream>

#define EVERYTHING

using namespace std;

namespace crud {
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
                throw "That license plate already belongs to a plane";

            return true;
        });
    }
    
    string askUsedLicensePlate() {
        return readValue<string>("License plate: ", "Please insert a valid license plate", [](const string &value) {
            Plane *plane = findPlaneByLicensePlate(value);
            if (plane == nullptr)
                throw "That license plate doesn't belong to any plane";

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
        directAttributes.addOption(repr.str() + "licensePlate", [&filter, &repr]() {
            repr << "licensePlate ";
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

        // MenuBlock flights;
        // flights.addOption(repr.str() + "if all flights have", [&filter, &repr]() {
        //     repr << "if all flights have ";

        //     auto flightFilter = createFlightFilter(repr);
        //     filter = [flightFilter](const Plane* const &plane) {
        //         for (const Flight* const &flight : plane->getFlights()) {
        //             if (!flightFilter(flight))
        //                 return false;
        //         }

        //         return true;
        //     };
        // });

        // flights.addOption(repr.str() + "if any flights have", [&filter, &repr]() {
        //     repr << "if any flights have ";

        //     auto flightFilter = createFlightFilter(repr);
        //     filter = [flightFilter](const Plane* const &plane) {
        //         for (const Flight* const &flight : plane->getFlights()) {
        //             if (flightFilter(flight))
        //                 return true;
        //         }

        //         return false;
        //     };
        // });

        // MenuBlock services;
        // services.addOption(repr.str() + "if all finished services have", [&filter, &repr]() {
        //     repr << "if all finished services have ";

        //     auto serviceFilter = createServiceFilter(repr);
        //     filter = [serviceFilter](const Plane* const &plane) {
        //         for (const Service* const &flight : plane->getFinishedServices()) {
        //             if (!serviceFilter(flight))
        //                 return false;
        //         }

        //         return true;
        //     };
        // });

        // services.addOption(repr.str() + "if any finished services have", [&filter, &repr]() {
        //     repr << "if any finished services have ";

        //     auto serviceFilter = createServiceFilter(repr);
        //     filter = [serviceFilter](const Plane* const &plane) {
        //         for (const Service* const &service : plane->getFinishedServices()) {
        //             if (serviceFilter(service))
        //                 return true;
        //         }

        //         return false;
        //     };
        // });

        // services.addOption(repr.str() + "if the next scheduled service has", [&filter, &repr]() {
        //     repr << "if the next scheduled service has ";

        //     auto serviceFilter = createServiceFilter(repr);
        //     filter = [serviceFilter](const Plane* const &plane) {
        //         return serviceFilter(plane->getScheduledServices().front());
        //     };
        // });

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
        // menu.addBlock(flights);
        // menu.addBlock(services);
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

    void slicePlaneVectorFromBeginWithUserInput(vector<Plane*> &pool) {
        size_t number = readValue<size_t>("#: ", "Please provide a valid number of elements");
        auto last = pool.begin() + number - 1;
        for (auto it = pool.end() - 1; it > last ; it--)
            pool.erase(it);
    }

    void slicePlaneVectorFromEndWithUserInput(vector<Plane*> &pool) {
        size_t number = readValue<size_t>("#: ", "Please provide a valid number of elements");
        auto first = pool.end() - number;
        for (auto it = pool.begin(), cursor = first; it < first; it++, cursor++)
            *it = *cursor;

        pool.resize(number);
    }

    void readAllPlanesWithUserInput() {
        vector<Plane*> pool = data::planes;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterPlanesWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderPlanesWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # planes", [&pool]() { slicePlaneVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # planes", [&pool]() { slicePlaneVectorFromEndWithUserInput(pool); });

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
        choice.addOption("Type", [&plane](){
            string type = readValue<GetLine>("Type: ", "Please insert a valid plane type");
            plane.setType(type);
        });

        choice.addOption("Capacity", [&plane](){
            unsigned int capacity = readValue<unsigned int>("Type: ", "Please insert a valid plane capacity");
            plane.setCapacity(capacity);
        });

        // MenuBlock flights;
        // flights.addOption("Add a flight to this plane")

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

    void deleteAllPlanesWithFilters() {
        vector<Plane*> pool = data::planes;

        MenuBlock ops;
        ops.addOption("Filter", [&pool]() { filterPlanesWithUserInput(pool); });
        ops.addOption("Sort", [&pool]() { orderPlanesWithUserInput(pool); });

        MenuBlock other_ops;
        other_ops.addOption("Keep the first # planes", [&pool]() { slicePlaneVectorFromBeginWithUserInput(pool); });
        other_ops.addOption("Keep the last # planes", [&pool]() { slicePlaneVectorFromEndWithUserInput(pool); });

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
                }

                if (was_selected)
                    delete plane1;
                else
                    new_planes.push_back(plane1);
            }

            data::planes = pool = vector<Plane*>(new_planes);
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
        ohno.addOption("Read all planes with filter and sort", allowWhenPlanesExist(readAllPlanesWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one plane", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all planes", allowWhenPlanesExist(deleteAllPlanes));
        remove.addOption("Delete all planes with filters and sort", allowWhenPlanesExist(deleteAllPlanesWithFilters));

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

#ifndef EVERYTHING

    void manageAirports() {

    }

    void manageFlights() {

    }

    void manageHandlingCars() {

    }

    void manageTickets() {

    }

#else

// //     /*----------FLIGHTS----------*/

    Flight* findFlightById(const string &flight_id) {
        return utils::binarySearch<Flight*, string>(data::flights, flight_id, [](Flight *flight){
            return flight->getFlightId();
        });
    }

    Flight* findFlightByFlightkey(const string &flight_id, const Datetime &departure) {
        auto it = utils::lowerBound<Flight*, string>(data::flights, flight_id, [](Flight *flight) {
            return flight->getFlightId();
        });

        while (!((*it)->getDepartureTime() == departure)) {
            if (it == data::flights.end())
                return nullptr;

            it++;
        }

        return *it;
    }

    std::pair<string, Datetime> askUnusedFlightKey() {
        string id = readValue<string>("Flight ID: ", "Please insert a valid flight id");
        
        Flight *flight = findFlightById(id);
        

        
        /*
        [](const string &value) {
            //se existir um plane com o id, devolver as cenas do plane
            // se existir mais do que um plane com o id, pedir um datetime que não exista com esse id
            // se não existir nenhum plane com o id, pedir uma datetime qualquer
            Flight *flight = findFlightByFlightkey(valu, datetimee);
            if (flight != nullptr)
                throw "That flight id already belongs to a plane";

            return true;*/
    }
    
    string askUsedFlightKey() {
        return readValue<string>("Flight ID: ", "Please insert a valid flight id", [](const string &value) {
            Flight *flight = findFlightByFlightId(value);
            if (flight == nullptr)
                throw "That flight id doesn't belong to any plane";

            return true;
        });
    }

    Flight &findFlight() {
        if (data::planes.empty())
            throw logic_error("No flight found");

        if (data::flights.size() == 1)
            return *data::flights.at(0);

        string flight_id = askUsedFlightKey();
        Datetime departure_time = as

        cout << endl;

        Flight *flight = findFlightByFlightkey(flight_id);
        if (flight == nullptr)
            throw logic_error("No flight found");

        return *flight;
    }

    /**
     * @brief Creates a Plane instance
     */
    void createFlight() {
        string flight_id = askUnusedFlightId();
        string type = readValue<GetLine>("Type: ", "Please insert a valid flight id");
        unsigned int capacity = readValue<unsigned int>("Capacity: ", "Please insert a valid capacity");

        Plane *plane = new Plane(license_plate, type, capacity);
        auto 





pos = utils::lowerBound<Plane*, string>(data::planes, license_plate, [](Plane* plane) {
            return plane->getLicensePlate();
        });

        data::planes.insert(pos, plane);
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

    /**
     * @brief Displays all the existing planes
     */
    void readAllFlights() {
        for (const Flight *flight : data::flights) {
            cout << "-----------------------" << endl;
            cout << *flight << endl;
        }
        cout << "-----------------------" << endl;

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
        block.addOption(repr.str() + "=", [&filter, &repr, mapper, validator]() {
            repr << "= ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs == rhs;
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
        directAttributes.addOption("type", [&filter, &repr]() {
            repr << "type ";
            filter = createFilter<const Plane*, string, GetLine>(repr, [](const Plane* const &value) {
                return value->getType();
            });
        });

        menu.addBlock(directAttributes);
        menu.show();

        return filter;
    }
    

    /**
     * @brief Displays all planes with characteristics specified by the user
     */
    void readPlanesWithFilters(vector<Plane*> &pool) {
        ostringstream repr;

        function<bool(const Plane* const&)> filter = createPlaneFilter(repr);
        remove_if(pool.begin(), pool.end(), [&filter](const Plane* plane) {
            return !filter(plane);
        });
    }

    function<void()> readPlanesOrdered(vector<Plane*> &pool) {
        
    }

    void readAllPlanesWithUserInput() {
        vector<Plane*> pool = data::planes;
        readPlanesWithFilters(pool);
    }

    /**
     * @brief Updates one Plane instance
     */
    void updateFlight() {
        Flight &flight = findFlight();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Flight ID", [&flight]() {
            string flight_id = readValue<string>("Flight ID: ", "Please insert a valid flight ID");
            Flight *new_flight = new Flight(flight_id, flight.getDepartureTime(), flight.getDuration(), flight.getOrigin(), flight.getDestination(), flight.getTickets(), flight.getPlane());
        });

        choice.addOption("Departure time", [&flight](){
            string departure = readValue<GetLine>("Departure time(YYYY-MM-DD-HH-MM-SS): ", "Please insert a valid departure time");
            departure = Datetime::toDatetime(departure);
            Flight *new_flight = new Flight(flight.getFlightID(), departure, flight.getDuration(), flight.getOrigin(), flight.getDestination(), flight.getTickets(), flight.getplane());
        });

        choice.addOption("Duration", [&plane](){
            string duration = readValue<string>("Duration(HH:MM:SS): ", "Please insert a valid duration");
            duration = T
        });

        // MenuBlock flights;
        // flights.addOption("Add a flight to this plane")

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

    void manageFlights() {
        Menu menu("Flights");


        Menu menu("Select one of the following operations:");

        auto allowWhenFlightExists = [](const function<void()> &func) {
            return [func]() {
                if (data::flights.empty()) {
                    cout << "There are no flights in the database" << endl;
                    waitForInput();
                    return;
                }

                func();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new flight", createPlane);
        normal.addOption("Update flight", allowWhenFlightExists(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one flight", allowWhenFlightExists(readOnePlane));
        ohno.addOption("Read all flights", allowWhenFlightExists(readAllPlanes));
        ohno.addOption("Read all flights with filters", allowWhenFlightExists(readAllPlanesWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one flight", allowWhenFlightExists(deleteOnePlane));
        remove.addOption("Delete all flights", allowWhenFlightExists(deleteAllPlanes));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();



        MenuBlock block;
    }











// //     /*----------FLIGHTS----------*/

    void manageFlights() {
// //         Menu menu("Flights");






// //         MenuBlock block;
    }


















// //     /*----------TICKETS----------*/

    Ticket* findTicketsBySeatNumber(Flight* flight, const unsigned int seat_number) {
        return utils::binarySearch<Ticket*, unsigned int>(flight->getTickets(), seat_number, [](Ticket* ticket) {
            return ticket->getSeatNumber();
        });
    }

    Ticket* findTicketByFlightAndCustomer(Flight* flight, const string &name) {
        for (Ticket* ticket : flight->getTickets()) {
            if (ticket->getCustomer().getName() == name) {
                return ticket;
            }
        }

        return nullptr;
    }

    unsigned int askUnusedSeatNumber(Flight* flight) {
        return readValue<unsigned int>("Seat number: ", "Please insert a valid seat number", [&flight](const unsigned int seat_number) {
            return findTicketsBySeatNumber(flight, seat_number) == nullptr;
        });
    }

    unsigned int askUsedSeatNumber(Flight* flight) {
        return readValue<unsigned int>("Seat number: ", "Please insert a valid seat number", [&flight](const unsigned int &seat_number) {
            return findTicketsBySeatNumber(flight, seat_number) != nullptr;
        });
    }

    Ticket &findTicket(Flight* flight) {
        if (flight->getTickets().empty()) {
            throw logic_error("No tickets found");
        }

        if (flight->getTickets().size() == 1) {
            return *flight->getTickets().at(0);
        }

        unsigned int seat_number = askUsedSeatNumber(flight);
        cout << endl;

        Ticket *ticket = findTicketsBySeatNumber(flight, seat_number);
        if (ticket == nullptr)
            throw logic_error("No ticket found");

        return *ticket;
    }

    void createTicket(Flight* flight) {
        string name = readValue<GetLine>("Name: ","Please insert a valid name");
        unsigned int age = readValue<unsigned int>("Age: ", "Please insert a valid age");
        unsigned int seat_number = askUnusedSeatNumber(flight);
        
        Ticket *ticket = new Ticket(*flight, name, age, seat_number);
        flight->addTicket(*ticket);
        
        waitForInput();
    }
    
    void readOneTicket(Flight *flight) {
        const Ticket &ticket = findTicket(flight);
        cout << ticket << endl;
        waitForInput();
    }

    string getTicketRepresentation(Flight* flight) {
        ostringstream repr;

        for (const Ticket *ticket : flight->getTickets()) {
            repr << "-----------------------\n\n"
                 << *ticket << "\n\n";
        }
        repr << "-----------------------";

        return repr.str();
    }

    void readAllTickets(Flight *flight) {
        cout << getTicketRepresentation(flight->getTickets()) << endl;
        waitForInput();
    }

    function<bool(const Ticket* const&)> createTicketFilter(ostringstream &repr) {

    }

    void updateTicket(Flight *flight) {
        Ticket &ticket = findTicket(flight);
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Seat mumber", [&ticket, &flight]() {
            unsigned int seat = askUsedSeatNumber(flight);
            Ticket* new_ticket = new Ticket(*flight, ticket.getCustomerName(), ticket.getCustomerAge(), seat);

            flight->removeTicket(ticket);
            delete &ticket;

            flight->addTicket(*new_ticket);
        });

        bool is_running = true;

        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() {is_running = false});

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);

        while (is_running) {
            menu.show(ticket.str());
        }
    }

    void deteleOneTicket(Flight *flight) {
        Ticket const &ticket = findTicket(flight);
        flight->removeTicket(ticket);
        delete &ticket;
        waitForInput();
        return;
    }

    void deleteAllTickets(Flight *flight) {
        for (const Ticket *ticket : flight.getTickets()) {
            delete ticket;
        }

        flight.getTickets().clear();
    }

    void manageTickets() {
        
// //         Menu menu("Select one of the following operations:");

// //         auto allowWhenPlanesExist = [](const function<void()> &f) {
// //             return [f]() {
// //                 if (data::planes.empty()) {
// //                     cout << "There are no planes in the database" << endl;
// //                     waitForInput();
// //                     return;
// //                 }

// //                 f();
// //             };
// //         };

// //         MenuBlock normal;
// //         normal.addOption("Create a new ticket", createPlane);
// //         normal.addOption("Update tickets", allowWhenPlanesExist(updatePlane));

// //         MenuBlock ohno;
// //         ohno.addOption("Read one ticket", allowWhenPlanesExist(readOnePlane));
// //         ohno.addOption("Read all tickets", allowWhenPlanesExist(readAllPlanes));
// //         ohno.addOption("Read all tickets with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

// //         MenuBlock remove;
// //         remove.addOption("Delete one ticket", allowWhenPlanesExist(deleteOnePlane));
// //         remove.addOption("Delete all tickets", allowWhenPlanesExist(deleteAllPlanes));

// //         bool is_running = true;
// //         MenuBlock special_block;
// //         special_block.addOption("Go back", [&is_running]() { is_running = false; });

// //         menu.addBlock(normal);
// //         menu.addBlock(ohno);
// //         menu.addBlock(remove);
// //         menu.setSpecialBlock(special_block);

// //         while (is_running)
// //             menu.show();
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
            if (airport != nullptr) {
                throw "That name is already being used"
            }
            return true;
        });
    }
    
    string askUsedName() {
        return readValue<GetLine>("Airport name: ", "Please insert a valid airport name", [](const string &value) {
            if (airport == nullptr) {
                throw "That name isn't being used"
            }
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
        if (name == nullptr)
            throw logic_error("No airport found");

        return *airport;
    }

    void createAirport() {
        string name = askUnusedName();
        
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
        waitForInput();
    }

    string getAirportRepresentation(const vector<Airport*> &vec) {
        ostringstream repr;

        for (const Airport *airport : vec) {
            repr << "-----------------------\n\n"
                 << *airport << "\n\n";
        }
        repr << "-----------------------";
    
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
            repr << name;
            filter = createFilter<const Airport*, string>(repr, [](const Airport* const &value) {
                return value->getName();
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

        menu.addBlock(directAttributes);
        menu.addBlock(booleanLogic);
        menu.show();

        return filter;
    }

    void readAirportsWithFilters(vector<Airport*> &pool) {

    }

    void readAirportsOrdered(vector<Airport*> &pool) {

    }

    void readAllAirportsWithUserInput(){

    }

    void updateAirport() {
        Airport &airport = findAirport();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Name", [&airport](){
            string name = readValue<GetLine>("Name: ", "Please inserta valid name");
            airport.setName(name);
        });

        choice.addOption("Add transportation stop", [&airport]() {
            string name = readValue<GetLine>("Name: ", "Please insert a valid name");
            float longitude = readValue<float>("Longitude: ", "Please insert a valid longitude");
            float latitude = readValue<float>("Latitude", "Please insert a valid latitude");
            TransportType transport_type = readValue<unsigned int>("Transport Type (0-subway, 1-bus, 2-train): ", "Please insert a valid trasnportation method");
            float airport_distance = readValue<float>("Airport distance: ", "Please insert a valid distance");
            unsigned schedule_number = readValue<unsigned>("How many times it passes by: ", "Please insert a valid number");
            set<Datetime> schedule;
            for (int i = 0; i < schedule_number; i++) {
                string datetime = readValue<string>("Time " + to_string(i) + "(YYYY-MM-DD-HH-MM-SS): ", "Please insert a valid date");
                schedule.insert(Datetime::toString(datetime));
            }

            airport.addTransportPlaceInfo(new TransportPlace(name, latitude, longitude, transport_type, airport_distance, schedule));
        });

        choice.addOption("Remove stop", [&airport]() {
            string name = readValue<GetLine>("Name: ", "Please insert a valid name");
            for (auto it : airport.getTransportPlaceInfo()) {
                if (*it.name == name) {
                    airport.getTransportPlaceInfo().delete(it);
                }
            }
        });
        
        choice.addOption("Remove all stops", [&airport]() {
            for (auto it : airport.getTransportPlaceInfo()) {
                delete(it);
            }
        });

        bool is_running = true;

        MenuBlock special_block;
        special_block.addOption("Go Back", [&is_running]() {is_running = false});

        menu.addBlock(choice);
        menu.setSpecialBlock(special_block);

        while (is_running) {
            menu.show(airport.str());
        }
    }

    void deleteAllStops() {
        Airport const &airport = findAirport();
        for (const TransportPlace *transport : data::airports) {
            delete transport;
        }
    }

    void manageAirports() {
        Menu menu("Airports");

        Menu menu("Select one of the following operations:");

        auto allowWhenAirportExists = [](const function<void()> &func) {
            return [func]() {
                if (data::airports.empty()) {
                    cout<< "There are no airports in the database";
                    waitForInput();
                    return;
                }

                func();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new airport", createAirport);
        normal.addOption("Update airport", allowWhenAirportExists(updateAirport));

        MenuBlock ohno;
        ohno.addOption("Read one airport", allowWhenAirportExists(readOneAirport()));
        ohno.addOption("Read all flights", allowWhenAirportExists(readAllAirports()));
        ohno.addOption("Read all airports with filters", allowWhenAirportExists(readAllAirportsWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one airport", allowWhenAirportExists(deleteOneAirport));
        remove.addOption("Delete all airports", allowWhenAirportExists(deleteAllAirport));

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();

        MenuBlock block;
    }

// //     void createAirport() {
// //         // string name;
// //         // GetLine type;
// //         // unsigned int capacity;

// //         // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
// //         //     for (const Airport *airport: data::airports) {
// //         //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
// //         //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
// //         //             return airport1.getName() == name;
// //         //         })
// //         //         if (it == airps.end())
// //         //             throw "An airport with that name already exists";
// //         //     }

// //         //     return true;
// //         // });
// //         // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
// //         // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

// //         // Plane *plane = new Plane(license_plate, type(), capacity);
// //         // data::planes.push_back(plane);

// //         // waitForInput();
// //     }


//     void manageAirports() {
// //         Menu menu("Select one of the following operations:");

// //         auto allowWhenPlanesExist = [](const function<void()> &f) {
// //             return [f]() {
// //                 if (data::planes.empty()) {
// //                     cout << "There are no planes in the database" << endl;
// //                     waitForInput();
// //                     return;
// //                 }

// //                 f();
// //             };
// //         };

// //         MenuBlock normal;
// //         normal.addOption("Create a new airport", createPlane);
// //         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

// //         MenuBlock ohno;
// //         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
// //         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
// //         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

// //         MenuBlock remove;
// //         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
// //         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

// //         bool is_running = true;
// //         MenuBlock special_block;
// //         special_block.addOption("Go back", [&is_running]() { is_running = false; });

// //         menu.addBlock(normal);
// //         menu.addBlock(ohno);
// //         menu.addBlock(remove);
// //         menu.setSpecialBlock(special_block);

// //         while (is_running)
// //             menu.show();
//     }


//     void manageHandlingCars() {
// //         Menu menu("Select one of the following operations:");

// //         auto allowWhenPlanesExist = [](const function<void()> &f) {
// //             return [f]() {
// //                 if (data::planes.empty()) {
// //                     cout << "There are no planes in the database" << endl;
// //                     waitForInput();
// //                     return;
// //                 }

// //                 f();
// //             };
// //         };

// //         MenuBlock normal;
// //         normal.addOption("Create a new handling car", createPlane);
// //         normal.addOption("Update an handling car", allowWhenPlanesExist(updatePlane));

// //         MenuBlock ohno;
// //         ohno.addOption("Read one handling car", allowWhenPlanesExist(readOnePlane));
// //         ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllPlanes));
// //         ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

// //         MenuBlock remove;
// //         remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOnePlane));
// //         remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllPlanes));

// //         bool is_running = true;
// //         MenuBlock special_block;
// //         special_block.addOption("Go back", [&is_running]() { is_running = false; });

// //         menu.addBlock(normal);
// //         menu.addBlock(ohno);
// //         menu.addBlock(remove);
// //         menu.setSpecialBlock(special_block);

// //         while (is_running)
// //             menu.show();
// //     }
// // }
//     }

// //     /*----------TICKETS----------*/

    // void manageTickets() {
// //         Menu menu("Select one of the following operations:");

// //         auto allowWhenPlanesExist = [](const function<void()> &f) {
// //             return [f]() {
// //                 if (data::planes.empty()) {
// //                     cout << "There are no planes in the database" << endl;
// //                     waitForInput();
// //                     return;
// //                 }

// //                 f();
// //             };
// //         };

// //         MenuBlock normal;
// //         normal.addOption("Create a new airport", createPlane);
// //         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

// //         MenuBlock ohno;
// //         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
// //         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
// //         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

// //         MenuBlock remove;
// //         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
// //         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

// //         bool is_running = true;
// //         MenuBlock special_block;
// //         special_block.addOption("Go back", [&is_running]() { is_running = false; });

// //         menu.addBlock(normal);
// //         menu.addBlock(ohno);
// //         menu.addBlock(remove);
// //         menu.setSpecialBlock(special_block);

// //         while (is_running)
// //             menu.show();
    // }

// //     /*----------AIRPORTS----------*/

// //     void createAirport() {
// //         // string name;
// //         // GetLine type;
// //         // unsigned int capacity;

// //         // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
// //         //     for (const Airport *airport: data::airports) {
// //         //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
// //         //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
// //         //             return airport1.getName() == name;
// //         //         })
// //         //         if (it == airps.end())
// //         //             throw "An airport with that name already exists";
// //         //     }

// //         //     return true;
// //         // });
// //         // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
// //         // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

// //         // Plane *plane = new Plane(license_plate, type(), capacity);
// //         // data::planes.push_back(plane);

// //         // waitForInput();
// //     }


    void manageAirports() {
// //         Menu menu("Select one of the following operations:");

// //         auto allowWhenPlanesExist = [](const function<void()> &f) {
// //             return [f]() {
// //                 if (data::planes.empty()) {
// //                     cout << "There are no planes in the database" << endl;
// //                     waitForInput();
// //                     return;
// //                 }

// //                 f();
// //             };
// //         };

// //         MenuBlock normal;
// //         normal.addOption("Create a new airport", createPlane);
// //         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

// //         MenuBlock ohno;
// //         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
// //         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
// //         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

// //         MenuBlock remove;
// //         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
// //         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

// //         bool is_running = true;
// //         MenuBlock special_block;
// //         special_block.addOption("Go back", [&is_running]() { is_running = false; });

// //         menu.addBlock(normal);
// //         menu.addBlock(ohno);
// //         menu.addBlock(remove);
// //         menu.setSpecialBlock(special_block);

// //         while (is_running)
// //             menu.show();
    }

    HandlingCar* findCarByID(const unsigned &id) {
        return utils::binarySearch<HandlingCar*, unsigned>(data::handlingCars, id, [](HandlingCar *car) {
            return car->getID();
        });
    }

    unsigned askUnusedID() {
        return readValue<unsigned>("ID: ", "Please insert a valid number", [](const unsigned &value) {
            HandlingCar *car = findCarByID(value);
            if (car != nullptr)
                throw "That ID is already being used";

            return true;
        });
    }

    unsigned askUnusedID() {
        return readValue<unsigned>("ID: ", "Please input a valid ID", [](const unsigned &value) {
            HandlingCar *car = findCarByID(value);
            if (car == nullptr) {
                throw "That ID is not being used";
            }

            return true;
        });
    
    HandlingCar &findCar() {
        if (data::handlingCars.empty())
            throw logic_error("No handling car found");

        if (data::handlingCars.size() == 1) {
            return data::handlingCars.at(0);
        }

        unsigned int id = askusedID();

        cout << endl;

        HandlingCar *car = findCarByID(id);
        if (car == nullptr)
            throw logic_error("No handling car found");
    
        return *car;
    }

    void createHandlingCar() {
        unsigned int number_of_carriages = readValue<unsigned>("Number of carriages: ", "Please input a valid number");
        unsigned int stacks_per_carriage = readValue<unsigned>("Number of stacks in each carriage: ", "Please input a valid number");
        unsigned int luggage_per_stack = readValue<unsigned>("Luggage per stack: ", "Please insert a valid number");
        HandlingCar *car = new HandlingCar(number_of_carriages, stacks_per_carriaidck);
        auto pos = utilsgetIDgCar*, unsigned>(data::handlingCars, number_of_carriages, [](HandlingCar *car) {
            return car->getNumberOfCarriages();
        });

        data::handlingCars.insert(pos, car);
        waitForInput();
    }

        HandlingCar &car = findCar();
        Menu menu("Please select what you want to update:");

        Menublock choice;
        choice.addOption("Number of carriages", [&car]() {
            unsigned int number_of_carriages = readValue<unsigned>("Number of carriages: ", "Please input a valid number");
            car.setNumber ?
        })    void updateHandlingCar() {



    void readOneCar() {
        const HandlingCar &car = findCar();
        cout << car << endl;
        waitForInput();
    }

    string getHandlingCarRepresentation(const vector<HandlingCar*> &vec) {
        ostringstream out;

        for (const HandlingCar *car : vec) {
            repr <<  "-----------------------\n\n"
                 << *car << "\n\n";
        }
        repr << "-----------------------";

        return repr.str();
    }

    void readAllCars() {
        cout << getHandlingCarRepresentation(data::handlingCars) << endl;
        waitForInput();
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
            delete plane;

        data::handlingCars.clear();
    }    }
    
    void manageHandlingCars() {
        Menu menu("Select one of the following operations:");

        auto allowWhenCartsExist = [](const function<void()> &f) {
            return [f]() {
                if (data::handlingCars.empty()) {
                    cout << "There are no hadling cars in the database" << endl;
                    waitForInput();
                    return;
                 }
                 f();
             };
         };

        MenuBlock normal;
        normal.addOption("Create a new handling car", createHandlingCar);
        normal.addOption("Update an handling car", allowWhenPlanesExist(updateHandlingCar));

        ohno.addOption("Read one handling car", allowWhenPlanesExist(readOneHandlingCar));
        ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllHandlingCars));
        ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllHandlingCarsWithFilters));

        remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOneHandlingCar));
        remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllHandlingCars));

        bool is_running = true;
        MenuBlock special_block;
        menu.addBlock(normal);
        menu.addBlock(ohno);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
 
 
  }


  #endif
}