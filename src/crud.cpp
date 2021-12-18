#include "crud.h"
#include <algorithm>
#include <set>

using namespace std;

namespace crud {
    /*----------PLANES----------*/

    Plane* findPlane() {
        if (data::planes.empty())
            return nullptr;

        if (data::planes.size() == 1)
            return data::planes.at(0);

        string license_plate;
        read_value<string>("License plate: ", "Please insert a valid license plate", license_plate, [](string value) {
            for (const Plane *plane : data::planes) {
                if (value == plane->getLicensePlate())
                    return true;
            }

            throw "There isn't a plane with that license plate";
        });

        cout << endl;

        for (Plane *plane : data::planes) {
            if (license_plate == plane->getLicensePlate())
                return plane;
        }

        return nullptr;
    }

    /**
     * @brief Creates a Plane instance
     */
    void createPlane() {
        string license_plate;
        GetLine type;
        unsigned int capacity;

        read_value<string>("License plate: ", "Please insert a valid license plate", license_plate, [](string value) {
            for (const Plane *plane : data::planes) {
                if (plane->getLicensePlate() == value)
                    throw "A plane with that license plate already exists";
            }

            return true;
        });
        read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
        read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

        Plane *plane = new Plane(license_plate, type(), capacity);
        data::planes.push_back(plane);

        waitForInput();
    }

    /**
     * @brief Displays a plane specified by the user
     */
    void readOnePlane() {
        if (data::planes.empty()) {
            cout << "There are no planes in the database" << endl;
            waitForInput();
            return;
        }

        const Plane *plane = findPlane();
        if (plane == nullptr)
            throw logic_error("No plane found");

        cout << *findPlane() << endl;
        waitForInput();
    }

    /**
     * @brief Displays all the existing planes
     */
    void readAllPlanes() {
        if (data::planes.empty()) {
            cout << "There are no planes in the database" << endl;
            waitForInput();
            return;
        }

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
    void updateOnePlane() {
        Plane *plane = findPlane();
        if (plane) {
            Menu menu("Please select what you want to update:");
            MenuBlock choice;

            choice.addOption("[1] License Plate", [&plane]() {

            });

            choice.addOption("[2] Type", [&plane](){

            });
        }
    }

    /**
     * @brief Updates every Plane instance
     */
    void updateAllPlanes() {

    }

    /**
     * @brief Updates every Plane instance with characteristics
     * * specified  user
     */
    void updateAllPlanesWithFilter() {

    }

    /**
     * @brief Deletes one Plane instance specified by the user
     */
    void deleteOnePlane() {
        if (data::planes.empty()) {
            cout << "There are no planes in the database" << endl;
            waitForInput();
            return;
        }

        Plane const *plane = findPlane();
        if (plane == nullptr)
            throw logic_error("No plane found");

        for (auto it = data::planes.begin(), end = data::planes.end(); it != end; it++) {
            if (*it == plane) {
                data::planes.erase(it);
                delete plane;
                waitForInput();
                return;
            }
        }

        delete plane;
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
     * @brief Deletes every Plane instance  with characteristics specified by the user
     */
    void deleteAllPlanesWithFilter() {

    }

    /**
     * @brief Manages all the existing operations regarding Planes, as well as the
     * operations we can do with them
     */
    void managePlanes() {
        Menu menu("Select one of the following operations:");

        MenuBlock create;
        create.addOption("Create a new plane", createPlane);

        MenuBlock read;
        read.addOption("Read one plane", readOnePlane);
        read.addOption("Read all planes", readAllPlanes);
        read.addOption("Read all planes with filters", readAllPlanesWithFilters);

        MenuBlock update;
        update.addOption("Update one plane", updateOnePlane);
        update.addOption("Update all planes", updateAllPlanes);
        update.addOption("Update all planes with filters", updateAllPlanesWithFilter);

        MenuBlock remove;
        remove.addOption("Delete one plane", deleteOnePlane);
        remove.addOption("Delete all planes", deleteAllPlanes);
        remove.addOption("Delete all planes with filters", deleteAllPlanesWithFilter);

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(create);
        menu.addBlock(read);
        menu.addBlock(update);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show()
    }

    /*----------FLIGHTS----------*/

    void manageFlights() {
        Menu menu("Flights");





        MenuBlock block;
    }

    /*----------TICKETS----------*/

    void manageTickets() {
        Menu menu("Tickets");

        MenuBlock block;
    }

    /*----------AIRPORTS----------*/

    void createAirport() {
        string name;
        GetLine type;
        unsigned int capacity;

        read_value<string>("Name: ", "Please insert a valid license plate", name, [](string value) {
            for (const Airport *airport: data::airports) {
                std::set<TransportPlace> airps = airport->getTransportPlaceInfo();
                auto it = find_if(airps.begin(), airps.end(), [name](Airport &airport1){
                    return airport1.getName() == name;
                })
                if (it == airps.end())
                    throw "An airport with that name already exists";
            }

            return true;
        });
        read_value<GetLine>("Type: ", "Please insert a valid plane type", type);
        read_value<unsigned int>("Capacity: ", "Please insert a valid capacity", capacity);

        Plane *plane = new Plane(license_plate, type(), capacity);
        data::planes.push_back(plane);

        waitForInput();
    }

    void manageAirports() {
        Menu menu("Select one of the following operations:");

        MenuBlock create;
        create.addOption("Create a new airport", createAirport);

        MenuBlock read;
        read.addOption("Read one airport", readOnePlane);
        read.addOption("Read all airports", readAllPlanes);
        read.addOption("Read all airports with filters", readAllPlanesWithFilters);

        MenuBlock update;
        update.addOption("Update one airport", updateOnePlane);
        update.addOption("Update all airports", updateAllPlanes);
        update.addOption("Update all airports with filter", updateAllPlanesWithFilter);

        MenuBlock remove;
        remove.addOption("Delete one airport", deleteOnePlane);
        remove.addOption("Delete all airports", deleteAllPlanes);
        remove.addOption("Delete all airports with filters", deleteAllPlanesWithFilter);

        bool is_running = true;
        MenuBlock special_block;
        special_block.addOption("Go back", [&is_running]() { is_running = false; });

        menu.addBlock(create);
        menu.addBlock(read);
        menu.addBlock(update);
        menu.addBlock(remove);
        menu.setSpecialBlock(special_block);

        while (is_running)
            menu.show();
    }

    void manageHandlingCars() {

    }
}