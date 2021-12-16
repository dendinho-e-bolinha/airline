//#include "iostream"
//#include "string"
//#include "list"
//#include "ticket.h"
//#include <vector>
//#include <stack>
#include <fstream>
#include <limits>

using namespace std;

int main() {
    ifstream file(".......");
    int lines;
    file >> lines;
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    string line;
    for (int i = 0; i < lines; i++) {
        file >> line;
    }
//    Flight flight;
//    vector<Ticket*> tickets;
//    Ticket ticket(flight, 2);
//    tickets.push_back(&ticket);
//    cout << (&ticket) << ' ' << (tickets.back()) << endl;
//
//    stack<Ticket*> t
}