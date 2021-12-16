#ifndef AIRLINE_FILES_H
#define AIRLINE_FILES_H

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <fstream>
#include "plane.h"
#include "ticket.h"

/**
 * @brief Opens the given file for reading
 * If the file doesn't exist, an error is thrown
 * @param file_name The name of the file
 *
 * @return an open stream to the given file
 */
std::ifstream openFileReader(const std::string &file_name);

/**
 * @brief Opens the given file for writing
 * If the file doesn't exist and it couldn't be created, an error is thrown
 * @param file_name The name of the file
 *
 * @return an open stream to the given file
 */
std::ofstream openFileWriter(const std::string &file_name);

/**
 * @brief Reads a file line by line
 * If the file doesn't exist, an error is thrown
 * @param file_name The name of the file
 *
 * @return a vector filled with the information on the file
 */
std::vector<std::string> readFile(std::string file_name);

/**
 * @brief Writes all the data about a plane's scheduled services
 * @param plane A plane instance
 */
void writeScheduledServices(Plane &plane);

/**
 * @brief Writes all the data about a plane's finished services
 * @param plane A plane instance
 */
void writeFinishedServices(Plane &plane);

/**
 * @brief Writes data about a given plane to a .txt file
 * @param f A flight instance
 */
void writeFlight(Flight &f);

void writeClient(Client &c);
void writeTicket(Ticket &t);
void writeLuggage(Luggage &l);
void writeCart(Cart &c);
Plane readPlane(); // ref?

/**
 * @brief Reads from the given file all the data relative to the plane's scheduled services
 * @param file A file to read from
 * @param p A plane instance
 * @return A queue with all the scheduled services for the given plane
 */
std::queue<Service> readScheduledServices(std::string &file, Plane &p);

/**
 * @brief Reads from the given file all the data relative to the plane's finished services
 * @param file A file to read from
 * @param p A plane instance
 * @return A vector with all the finished services for the given plane
 */
std::vector<Service> readFinishedServices(std::string &file, Plane &p);

/**
 * @brief Reads from the given file all the data relative to the plane's flights
 * @param p A plane instance
 * @param file A file to read from
 * @return A list with all the flight information ofthe given plane
 */
std::list<Flight*> readFlight(std::string &file, Plane &p);

#endif //AIRLINE_FILES_H
