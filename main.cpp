#include "iostream"
#include "string"
#include "list"
#include "ticket.h"
#include <vector>


using namespace std;

int main() {
    Flight flight;
    vector<Ticket*> tickets;
    Ticket ticket(flight, 2);
    tickets.push_back(&ticket);
    cout << (&ticket) << ' ' << (tickets.back()) << endl;
}