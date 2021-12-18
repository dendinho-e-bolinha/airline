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
        string type = readValue<GetLine>("Type: ", "Please insert a valid plane type")();
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
        for (const Plane *plane : data::planes)
            cout << *plane << endl;

        waitForInput();
    }

    /**
     * @brief Displays all planes with characteristics specified by the user
     */
    void readAllPlanesWithFilters() {

    }

    /**
     * @brief Updates one Plane instance
     */
    void updatePlane() {
        Plane &plane = findPlane();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Type", [&plane](){
            string type = readValue<GetLine>("Type: ", "Please insert a valid plane type")();
            plane.setType(type);
        });

        choice.addOption("Capacity", [&plane](){
            unsigned int capacity = readValue<unsigned int>("Type: ", "Please insert a valid plane capacity");
            plane.setCapacity(capacity);
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

    /**
     * @brief Manages all the existing operations regarding Planes, as well as the
     * operations we can do with them
     */
    void managePlanes() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new plane", createPlane);
        normal.addOption("Update plane", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one plane", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all planes", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all planes with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

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

    /*----------FLIGHTS----------*/

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
        string type = readValue<GetLine>("Type: ", "Please insert a valid plane type")();
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
        for (const Plane *plane : data::planes)
            cout << *plane << endl;

        waitForInput();
    }

    /**
     * @brief Displays all planes with characteristics specified by the user
     */
    void readAllPlanesWithFilters() {

    }

    /**
     * @brief Updates one Plane instance
     */
    void updatePlane() {
        Plane &plane = findPlane();
        Menu menu("Please select what you want to update:");

        MenuBlock choice;
        choice.addOption("Type", [&plane](){
            string type = readValue<GetLine>("Type: ", "Please insert a valid plane type")();
            plane.setType(type);
        });

        choice.addOption("Capacity", [&plane](){
            unsigned int capacity = readValue<unsigned int>("Type: ", "Please insert a valid plane capacity");
            plane.setCapacity(capacity);
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

    /**
     * @brief Manages all the existing operations regarding Planes, as well as the
     * operations we can do with them
     */
    void managePlanes() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new plane", createPlane);
        normal.addOption("Update plane", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one plane", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all planes", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all planes with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

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

    /*----------FLIGHTS----------*/

    void manageFlights() {
        Menu menu("Flights");





        MenuBlock block;
    }

    /*----------TICKETS----------*/

    void manageTickets() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new ticket", createPlane);
        normal.addOption("Update tickets", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one ticket", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all tickets", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all tickets with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one ticket", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all tickets", allowWhenPlanesExist(deleteAllPlanes));

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

    void createAirport() {
        // string name;
        // GetLine type;
        // unsigned int capacity;

        // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
        //     for (const Airport *airport: data::airports) {
        //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
        //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
        //             return airport1.getName() == name;
        //         })
        //         if (it == airps.end())
        //             throw "An airport with that name already exists";
        //     }

        //     return true;
        // });
        // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
        // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

        // Plane *plane = new Plane(license_plate, type(), capacity);
        // data::planes.push_back(plane);

        // waitForInput();
    }


    void manageAirports() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new airport", createPlane);
        normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

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


    void manageHandlingCars() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new handling car", createPlane);
        normal.addOption("Update an handling car", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one handling car", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllPlanes));

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
}
    }

    /*----------TICKETS----------*/

    void manageTickets() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new airport", createPlane);
        normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

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

    void createAirport() {
        // string name;
        // GetLine type;
        // unsigned int capacity;

        // read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
        //     for (const Airport *airport: data::airports) {
        //         std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
        //         auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
        //             return airport1.getName() == name;
        //         })
        //         if (it == airps.end())
        //             throw "An airport with that name already exists";
        //     }

        //     return true;
        // });
        // read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
        // read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

        // Plane *plane = new Plane(license_plate, type(), capacity);
        // data::planes.push_back(plane);

        // waitForInput();
    }


    void manageAirports() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new airport", createPlane);
        normal.addOption("Update airport", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one airport", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all airport", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all airports with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one airport", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all airports", allowWhenPlanesExist(deleteAllPlanes));

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


    void manageHandlingCars() {
        Menu menu("Select one of the following operations:");

        auto allowWhenPlanesExist = [](const function<void()> &f) {
            return [f]() {
                if (data::planes.empty()) {
                    cout << "There are no planes in the database" << endl;
                    waitForInput();
                    return;
                }

                f();
            };
        };

        MenuBlock normal;
        normal.addOption("Create a new handling car", createPlane);
        normal.addOption("Update an handling car", allowWhenPlanesExist(updatePlane));

        MenuBlock ohno;
        ohno.addOption("Read one handling car", allowWhenPlanesExist(readOnePlane));
        ohno.addOption("Read all handling cars", allowWhenPlanesExist(readAllPlanes));
        ohno.addOption("Read all handling cars with filters", allowWhenPlanesExist(readAllPlanesWithFilters));

        MenuBlock remove;
        remove.addOption("Delete one handling car", allowWhenPlanesExist(deleteOnePlane));
        remove.addOption("Delete all handling cars", allowWhenPlanesExist(deleteAllPlanes));

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
}