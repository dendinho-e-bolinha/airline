#include "crud.h"

using namespace std;

namespace crud {
    /*----------PLANES----------*/

    void createPlane() {
        string license_plate;
        read_value<string>("License plate: ", "Please insert a valid license plate", license_plate);
    }

    void readOnePlane() {


    }

    void readAllPlanes() {

    }

    void readAllPlanesWithFilters() {

    }

    void updateOnePlane() {

    }

    void updateAllPlanes() {

    }

    void updateAllPlanesWithFilter() {

    }

    void deleteOnePlane() {

    }

    void deleteAllPlanes() {

    }

    void deleteAllPlanesWithFilter() {

    }

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
        update.addOption("Update all planes with filter", updateAllPlanesWithFilter);

        MenuBlock remove;
        remove.addOption("Delete one plane", deleteOnePlane);
        remove.addOption("Delete all planes", deleteAllPlanes);
        remove.addOption("Delete all planes with filter", deleteAllPlanesWithFilter);

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
    //    std::string name;
    //    float latitude, longitude;
    //    TransportType transportType;
    //    float airportDistance;
    //    std::set<Datetime> schedule;
        //read_value<string>("License plate: ", "Please insert a valid license plate", license_plate);
    }

    void manageAirports() {
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
        update.addOption("Update all planes with filter", updateAllPlanesWithFilter);

        MenuBlock remove;
        remove.addOption("Delete one plane", deleteOnePlane);
        remove.addOption("Delete all planes", deleteAllPlanes);
        remove.addOption("Delete all planes with filter", deleteAllPlanesWithFilter);

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