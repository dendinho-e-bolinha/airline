#include "files.h"
#include <limits>

using namespace std;

ifstream openFileReader(const string& file_name) {
    ifstream file;
    file.open(file_name, ios::in);
    if (!file.is_open())
        throw "The given file doesn't exist";

    return file;
}

ofstream openFileWriter(const string& file_name) {
    ofstream file;
    file.open(file_name, ios::trunc);
    if (!file.is_open())
        throw "Couldn't create the given file";

    return file;
}

vector<string> readFile(const string& file_name) {
    ifstream reader = openFileReader(file_name);
    vector<string> result;

    string line;
    while(getline(reader, line)) {
        if (line.back() == '\r')
            line.pop_back();

        result.push_back(line);
    }

    reader.close();
    return result;
}


void writePlane(Plane &p) {
    ofstream file = openFileWriter("data.txt");
    file << p << endl;
    file.close();
}

void writeTicket(Ticket &t) {  //need ticket file here
    ofstream file = openFileWriter("data.txt");
    file << t << endl;
}

void writeScheduledServices(Plane &p) {  //referencia?? ou copy?
    ofstream file = openFileWriter("data.txt");
    queue<Service> copy_services;
    while(!(p.getScheduledServices().empty())) {
        copy_services.push(*(p.getScheduledServices().front()));
        file << p.getScheduledServices().front();
        p.getScheduledServices().pop();
    }

    while(!(p.getScheduledServices().empty())) {
        p.scheduleService(copy_services.front());  //works?
        copy_services.pop();
    }
    file.close();
}

void writeFinishedServices(Plane &p) {
    ofstream file = openFileWriter("data.txt");
    file << p.getFinishedServices().size() << '\n';
    for (auto s : p.getFinishedServices()) {
        file << s << '\n';
    }
    file.close();
}

void writeFlight(Flight &f) {
    ofstream file = openFileWriter("data.txt");
    file << f << endl;
    file.close();
}

queue<Service> readScheduledServices(string &file, Plane &p) {
    queue<Service> services;
    ifstream f = openFileReader(file);
    int lines;
    f >> lines;
    f.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < lines; i++) {
        string type;
        string datetime;
        string worker;
        f >> type >> datetime >> worker;

        ServiceType t = (stoi(type) == 0) ? MAINTENANCE : CLEANING;
        f.ignore(numeric_limits<streamsize>::max(), '\n');

        Service s(t, Datetime::toDatetime(datetime), worker, p);  //falta const cast?
        services.push(s);
    }

    return services;
}

vector<Service> readFinishedServices(string &file, Plane &p) {
    vector<Service> services;
    ifstream f = openFileReader(file);
    int lines;
    f >> lines;
    f.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < lines; i++) {
        string type;
        string datetime;
        string worker;
        f >> type >> datetime >> worker;

        ServiceType t = (stoi(type) == 0) ? MAINTENANCE : CLEANING;
        f.ignore(numeric_limits<streamsize>::max(), '\n');

        Service s(t, Datetime::toDatetime(datetime), worker, p);  //falta const cast?
        services.push_back(s);
    }

    return services;
}

list<Flight*> readFlight(string &file, Plane &p) {
    list<Flight*> flights;
    ifstream f = openFileReader(file);
    int lines;
    f >> lines;
    f.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < lines; i++) {
        string departure, duration, id, origin, destination;
        f >> departure >> duration >> id >> origin >> destination;
        Flight* flight(id, Datetime::toDatetime(departure), Time::toTime(duration), Airport::toString(origin), Airport::toString(destination), p);
        flights.push_back(flight);
    }

    return flights;
}
