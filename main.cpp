#include <iostream>
#include "interact.h"
#include "crud.h"
#include "input.h"
#inclufe "files.h"

using namespace std;

int main() {

    files::read();    

    string datetime = readValue<GetLine>("Date: ", "pls", [](const string &v) {
        input::readDateFromString(v);
        return true;
    });
    cout << datetime << endl;
    waitForInput();

    Menu menu("Please select an area you want to manage!");

    MenuBlock planeBlock;
    planeBlock.addOption("Planes", crud::managePlanes);
    planeBlock.addOption("Flights", crud::manageFlights);
    planeBlock.addOption("Tickets", crud::manageTickets);
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
