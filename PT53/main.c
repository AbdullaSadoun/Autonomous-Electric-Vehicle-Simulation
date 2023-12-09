/*
Simulation project's main
The main is responsible for:
- Taking the argc, argv file argumentrs
- Calling the appropriate functions to read and process files properly
- Calls the Run Simulation function to run the software
*/
#include "Simulation.h"

int main(int argc, char* argv[]) {
    int NSno, EWno;

    if (argc != 4) { // error if not suffiecient files or wrong input 
        printf("Usage: %s <BuildingDataFile> <CustomerDataFile> <EventDataFile>\n", argv[0]);
        return 1;
    }    
    

    _set_fmode(O_BINARY);// Open in binary mode for reading

    if ((bfd = fopen(argv[1], "r")) < 0) // error if cant open file
    {
        printf("Can't open %s\n", argv[1]);
        (void)getchar();
        return;
    }

    FILE* Delivery = fopen("Delivery.txt", "w");
    if (Delivery == NULL) {
        perror("Error opening Delivery file");
        return; // or exit with an error code
    }
    fprintf(Delivery, "Package No.\tOriginCustomer\tDeliveryCustomer\tPickupTime\tDeliveryTime\n");
    fflush(Delivery);


    FILE* Vehicle = fopen("Vehicle.txt", "w");
    if (Delivery == NULL) {
        perror("Error opening Vehicle file");
        return; // or exit with an error code
    }
    fprintf(Vehicle, "VIN\tLastStable\tBatteryCapcity\tBatteryCharge\tBatteryRechargeRate\tDrivingDischargeRate\tIdlingDischargeRate\tTotalTimeMoving\tTotalTimeWaiting\tTotalTimeCharging\n");
    fflush(Vehicle); // flushes the buffer to the file


    read_file(&EWno, &NSno); // will get the number of buildings in the rows and coloumns from the binary inputfile

    Customer customers[MAX_CUSTOMERS]; // Delcares customers array of type Customers
    readprcoessCustomerF(argv[2], customers); // reads and processes Customer File

    Event events[MAX_EVENTS]; // Declares events array of type Events
    readprocessEventF(argv[3], events);// read and process events file // make this in the run simulation?

    run_simulation(NSno, EWno, customers, events, Delivery, Vehicle); // runs the program

    // close all files
    fclose(bfd);
    fclose(Delivery); 
    fclose(Vehicle); 

    (void) getchar(); // keeps the output window open
    return 0;
}
