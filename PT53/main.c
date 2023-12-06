/*
Updated versoin of PT5
This version should include:
- add perimeter or zone
- ask user to specify vehicle origin
- direction rules
- ask user for direction rules
- get direction rules from binary file
- ask user for entrypoint
- multiple building entrypoint
- go to next destination
- multiple cars
- ask for numbers of cars
*/
#include "Simulation.h"

//FILE* bfd;

int main(int argc, char* argv[]) {
    int NSno, EWno;

    if (argc != 4) { // was != 3
        printf("Usage: %s <BuildingDataFile> <CustomerDataFile> <EventDataFile>\n", argv[0]);
        return 1;
    }
    

    // Open in binary mode for reading
    _set_fmode(O_BINARY);

    if ((bfd = fopen(argv[1], "r")) < 0)
    {
        printf("Can't open %s\n", argv[1]);
        (void)getchar();
        return;
    }

    read_file(&EWno, &NSno); // will get the number of buildings in the rows and coloumns from the binary inputfile

    Customer customers[MAX_CUSTOMERS];

    readprcoessCustomerF(argv[2], customers); // reads and processes Customer File

    Event events[MAX_EVENTS];
    readprocessEventF(argv[3], events);// read and process events file // make this in the run simulation?

    run_simulation(NSno, EWno, customers, events); // runs the program

    (void) getchar();
    return 0;
}
