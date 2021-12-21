#include <iostream>
#include "interact.h"
#include "crud.h"
#include "files.h"

using namespace std;

int main() {

    files::read();

    Menu menu("Please select an area you want to manage!");

    MenuBlock planeBlock;
    planeBlock.addOption("Planes", crud::managePlanes);
    planeBlock.addOption("Flights", crud::manageFlights);
    planeBlock.addOption("Airports", crud::manageAirports);
    planeBlock.addOption("Handling Cars", crud::manageHandlingCars);

    bool is_running = true;
    MenuBlock exitBlock;
    exitBlock.addOption("Exit", [&is_running]() { is_running = false; });

    menu.addBlock(planeBlock);
    menu.setSpecialBlock(exitBlock);

    while (is_running)
        menu.show();

    return 0;
}
