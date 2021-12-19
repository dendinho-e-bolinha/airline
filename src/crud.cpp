#include "crud.h"
#include "utils.h"
#include "interact.h"
#include "state.h"
#include <set>
#include <algorithm>
#include <sstream>

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

    /**
     * @brief Displays all the existing planes
     */
    void readAllPlanes() {
        for (const Plane *plane : data::planes) {
            cout << "-----------------------" << endl;
            cout << *plane << endl;
        }
        cout << "-----------------------" << endl;

        waitForInput();
    }


    template <typename T, typename V, typename P>
    function<bool(const T&)> createFilter(ostringstream &repr, const function<const V(const T&)> mapper, const function<bool(const V&, const V&)> filter, const function<bool(const P&)> validator = [](const P&) { return true; }) {
        // cout << "\x1B[2J\x1B[;H" << flush;

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

        block.addOption(repr.str() + "!=", [&filter, &repr, mapper, validator]() {
            repr << "!= ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs != rhs;
            }, validator);
        });

        block.addOption(repr.str() + "<", [&filter, &repr, mapper, validator]() {
            repr << "< ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs < rhs;
            }, validator);
        });

        block.addOption(repr.str() + ">", [&filter, &repr, mapper, validator]() {
            repr << "> ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs > rhs;
            }, validator);
        });

        block.addOption(repr.str() + "<=", [&filter, &repr, mapper, validator]() {
            repr << "<= ";
            filter = createFilter<T, V, P>(repr, mapper, [](const V& lhs, const V& rhs) {
                return lhs <= rhs;
            }, validator);
        });

        block.addOption(repr.str() + ">=", [&filter, &repr, mapper, validator]() {
            repr << ">= ";
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

        MenuBlock flights;
        flights.addOption(repr.str() + "if all flights'", [&filter, &repr]() {
            repr << "if all flights' ";

            auto flightFilter = createFlightFilter();
            filter = [flightFilter](const Plane* const &plane) {
                for (const Flight* const &flight : plane->getFlights()) {
                    if (!flightFilter(flight))
                        return false;
                }

                return true;
            };
        });

        flights.addOption(repr.str() + "if any flights'", [&filter, &repr]() {
            repr << "if any flights' ";

            auto flightFilter = createFlightFilter();
            filter = [flightFilter](const Plane* const &plane) {
                for (const Flight* const &flight : plane->getFlights()) {
                    if (flightFilter(flight))
                        return true;
                }

                return false;
            };
        });

        MenuBlock booleanLogic;
        booleanLogic.addOption("or", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createPlaneFilter();
            repr << ") or (";
            auto filter2 = createPlaneFilter();
            repr >> ')';

            filter = [filter1, filter2](const Plane* const &plane) {
                return filter1(plane) || filter2(plane);
            };
        });

        booleanLogic.addOption("and", [&filter, &repr]() {
            repr << '(';
            auto filter1 = createPlaneFilter();
            repr << ") and (";
            auto filter2 = createPlaneFilter();
            repr >> ')';

            filter = [filter1, filter2](const Plane* const &plane) {
                return filter1(plane) && filter2(plane);
            };
        });

        booleanLogic.addOption("not", [&filter, &repr]() {
            repr << 'not (';
            auto filter1 = createPlaneFilter();
            repr << ")";

            filter = [filter1](const Plane* const &plane) {
                return !filter1(plane);
            };
        });

        menu.addBlock(directAttributes);
        menu.addBlock(booleanLogic);
        menu.show();

        return filter;
    }
    

    /**
     * @brief Displays all planes with characteristics specified by the user
     */
    void readPlanesWithFilters(vector<Plane*> &pool) {
        ostringstream repr;

        function<bool(const Plane* const&)> filter = createPlaneFilter(repr);
        for (const Plane* const &plane: pool) {
            if (filter(plane))
                cout << *plane << endl;
        }

        waitForInput();
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

    /**
     * @brief Manages all the existing operations regarding Planes, as well as the
     * operations we can do with them
     */
    void managePlanes() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &func) {
            return [func]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
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
        ohno.addOption("Read all planes with filters", allowWhenPlanesExist(readAllPlanesWithUserInput));

        MenuBlock remove;
        remove.addOption("Delete one plane", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all planes", allowWhenPlanesExist(deleteAllPlanes));

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


























//     /*----------FLIGHTS----------*/


    Flight* findFlightbyFlightKey(const string &flight_id, const Datetime &datetime) {
        return utils::binarySearch<Flight*, string>(data::flights, flight_id, datetime, [](Flight *flight) {
            return flight->getFlightId() && flight->getDepartureTime();
        });
    }

    string askUnusedFlightKey() {
        return readValue<string>("Flight ID: ", "Please insert a valid flight id", [](const string &value) {
            Flight *flight = findPlaneByFlightId(value);
            if (flight != nullptr)
                throw "That flight id already belongs to a plane";

            return true;
        });
    }
    
    string askUsedFlightId() {
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

        string flight_id = askUsedFlightId();

        cout << endl;

        Flight *flight = findFlightByFlightId(flight_id);
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

    /**
     * @brief Displays all the existing planes
     */
    void readAllPlanes() {
        for (const Plane *plane : data::planes) {
            cout << "-----------------------" << endl;
            cout << *plane << endl;
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
        for (const Plane* const &plane: pool) {
            if (filter(plane))
                cout << *plane << endl;
        }

        waitForInput();
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











//     /*----------FLIGHTS----------*/

    void manageFlights() {
//         Menu menu("Flights");






//         MenuBlock block;
    }


















//     /*----------TICKETS----------*/

    void manageTickets() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new ticket", createPlane);
//         normal.addOption("Update tickets", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one ticket", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all tickets", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all tickets with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one ticket", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all tickets", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
    }

//     /*----------AIRPORTS----------*/

//     void createAirport() {
//         // string name;
//         // GetLine type;
//         // unsigned int capacity;

//         // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
//         //     for (const Airport *airport: data::airports) {
//         //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
//         //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
//         //             return airport1.getName() == name;
//         //         })
//         //         if (it == airps.end())
//         //             throw "An airport with that name already exists";
//         //     }

//         //     return true;
//         // });
//         // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
//         // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

//         // Plane *plane = new Plane(license_plate, type(), capacity);
//         // data::planes.push_back(plane);

//         // waitForInput();
//     }


    void manageAirports() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new airport", createPlane);
//         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
    }


    void manageHandlingCars() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new handling car", createPlane);
//         normal.addOption("Update an handling car", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one handling car", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
//     }
// }
    }

//     /*----------TICKETS----------*/

//     void manageTickets() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new airport", createPlane);
//         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
//     }

//     /*----------AIRPORTS----------*/

//     void createAirport() {
//         // string name;
//         // GetLine type;
//         // unsigned int capacity;

//         // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
//         //     for (const Airport *airport: data::airports) {
//         //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
//         //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
//         //             return airport1.getName() == name;
//         //         })
//         //         if (it == airps.end())
//         //             throw "An airport with that name already exists";
//         //     }

//         //     return true;
//         // });
//         // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
//         // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

//         // Plane *plane = new Plane(license_plate, type(), capacity);
//         // data::planes.push_back(plane);

//         // waitForInput();
//     }


//     void manageAirports() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new airport", createPlane);
//         normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
//     }


//     void manageHandlingCars() {
//         Menu menu("Select one of the following operations:");

//         auto allowWhenPlanesExist = [](const function<void()> &f) {
//             return [f]() {
//                 if (data::planes.empty()) {
//                     cout << "There are no planes in the database" << endl;
//                     waitForInput();
//                     return;
//                 }

//                 f();
//             };
//         };

//         MenuBlock normal;
//         normal.addOption("Create a new handling car", createPlane);
//         normal.addOption("Update an handling car", allowWhenPlanesExist(updatePlane));

//         MenuBlock ohno;
//         ohno.addOption("Read one handling car", allowWhenPlanesExist(readOnePlane));
//         ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllPlanes));
//         ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

//         MenuBlock remove;
//         remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOnePlane));
//         remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllPlanes));

//         bool is_running = true;
//         MenuBlock special_block;
//         special_block.addOption("Go back", [&is_running]() { is_running = false; });

//         menu.addBlock(normal);
//         menu.addBlock(ohno);
//         menu.addBlock(remove);
//         menu.setSpecialBlock(special_block);

//         while (is_running)
//             menu.show();
//     }
}