#ifndef AIRLINE_CRUD_H
#define AIRLINE_CRUD_H

namespace crud {
    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of planes
    */
    void managePlanes();

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of flights
    */
    void manageFlights();
    
    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of tickets
    */
    void manageTickets();

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update the airport
    */
    void manageAirports();

    /**
    * @brief Displays a menu where the user can choose options regarding deletion, addition or update of handling carts
    */
    void manageHandlingCars();
}

#endif //AIRLINE_CRUD_H
