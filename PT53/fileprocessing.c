#include Simulation.h"

// Dr. Hughes' functions used: //
struct prompt bldg_t[] = { {"Charge",CHG}, {"Stable",STB}, {"Both", BOTH} };
struct prompt ew[] = { {"East", East}, {"West", West} };
struct prompt ns[] = { {"North", North}, {"South", South} };
struct prompt bldg_q[] = {
    {"NE",NE}, {"N",N}, {"NW",NW},
    {"E",E}, {"LBL",-1}, {"W",W},
    {"SE",SE}, {"S",S}, {"SW",SW} };


void read_file(unsigned int* EWno, unsigned int* NSno)
{
    // - Read and display
    //#x #y - int int - buildings in row
    //1Str 1Ave - int int - (EW 01) and (NS 01)
    //1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
    // - Stop when #S = 0

    struct bldg_data bd;
    unsigned int xbldg;
    unsigned int ybldg;
    enum ST_DIR s1dir;
    enum AV_DIR a1dir;

    // #x #y int int buildings in row
    fread(&xbldg, sizeof(int), 1, bfd);
    fread(&ybldg, sizeof(int), 1, bfd);
    //printf("# X bldgs %d # Y bldgs %d\n", xbldg, ybldg);

    // Set the values of xbldg and ybldg to EWno and NSno respectively
    *EWno = xbldg;
    *NSno = ybldg;

    // 1Str 1Ave int int (EW 01) and (NS 01)
    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);
    //printf("First Street dir %s First Avenue dir %s\n", ew[s1dir].name, ns[a1dir].name);

    // - Read and display
    //	1 { #S #A Type Quad - Bldg XY + [Chg|Stab|Both] + [NE..SW] } Many
    // - Stop when #S=0

    // Read first record
    fread(&bd, sizeof(struct bldg_data), 1, bfd);
    while (bd.x > 0)
    {
        printf("Bldg XY: %d %d Type: %s Quad: %s\n", bd.x, bd.y,
            bldg_t[bd.bt].name, bldg_q[bd.qd].name);
        // Read next record
        fread(&bd, sizeof(struct bldg_data), 1, bfd);
    }

    fclose(bfd);
}

void readprcoessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]) {
    FILE* file = fopen(filename, "r");

    FILE* customertest = fopen("CUSTOMER_RESULTS.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[100]; // Adjust size as needed
    int customerIndex = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove potential newline character
        line[strcspn(line, "\n")] = 0;

        // Pass the line into the Customer struct
        //if (sscanf(line, "%d\t%10s\t%10s\t%2s\t%2s\t%d",&customers[0].CustNum, customer.FirstName, customer.LastName, customer.Building, customer.Entrance, &customer.Floor) == 6) {
        if (sscanf(line, "%d\t%49s\t%49s\t%2s\t%2s\t%d", &customers[customerIndex].CustNum, customers[customerIndex].FirstName, customers[customerIndex].LastName, customers[customerIndex].Building, customers[customerIndex].Entrance, &customers[customerIndex].Floor) == 6) {
        }

        int dest_building_row = customers[customerIndex].Building[0] - 'A';
        int dest_building_col = customers[customerIndex].Building[1] - 'A';
        int dest_y = originy + streeth * (dest_building_row + 1) + buildingh * dest_building_row + buildingh / 2;
        int dest_x = originx + avenuew * (dest_building_col + 1) + buildingw * dest_building_col + buildingw / 2;

        if (customers[customerIndex].Entrance[0] == 'N') {
            dest_y = dest_y - 1;
            if (customers[customerIndex].Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customers[customerIndex].Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customers[customerIndex].Entrance[0] == 'S') {
            dest_y = dest_y + 1;
            if (customers[customerIndex].Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customers[customerIndex].Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customers[customerIndex].Entrance[0] == 'E') {
            dest_x = dest_x - 1;
        }
        else if (customers[customerIndex].Entrance[0] == 'W') {
            dest_x = dest_x + 1;
        }
        else {
            printf("invalid input");
        }

        customers[customerIndex].locationx = dest_x;
        customers[customerIndex].locationy = dest_y;

        fprintf(customertest, "CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, x:%d, y:%d\n", customers[customerIndex].CustNum, customers[customerIndex].FirstName, customers[customerIndex].LastName, customers[customerIndex].Building, customers[customerIndex].Entrance, customers[customerIndex].Floor, customers[customerIndex].locationx, customers[customerIndex].locationy);
        customerIndex++;
    }

    fclose(file);

    //new test
    /*for (int i = 0; i < customerIndex; i++) {
        printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, Coords: %d %d\n", customers[i].CustNum, customers[i].FirstName, customers[i].LastName, customers[i].Building, customers[i].Entrance, customers[i].Floor, customers[i].locationx, customers[i].locationy);
    }*/
}

//event file function
void readprocessEventF(char* filename, Event events[MAX_EVENTS]) {
    FILE* file = fopen(filename, "r");

    FILE* eventtest = fopen("EVENT_RESULTS.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    //const int MAX_CUSTOMERS = 200;
    //Customer customers[MAX_CUSTOMERS];
    char line[100]; // Adjust size as needed
    int EventIndex = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove potential newline character
        line[strcspn(line, "\n")] = 0;

        sscanf(line, "%d\t%2s\t%d\t%d\t%d", &events[EventIndex].time, events[EventIndex].type, &events[EventIndex].OriginCust, &events[EventIndex].DestinationCust, &events[EventIndex].Weight);
        //my test
        fprintf(eventtest, "time is %d. origincustomer is %d. destinationcustomer is %d.\n", events[EventIndex].time, events[EventIndex].OriginCust, events[EventIndex].DestinationCust);
        EventIndex++;
    }

    fclose(file);
    fclose(eventtest);
}












/*
int readprcoessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    //const int MAX_CUSTOMERS = 200;
    Customer customers[MAX_CUSTOMERS];
    char line[100]; // Adjust size as needed
    int customerIndex = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove potential newline character
        line[strcspn(line, "\n")] = 0;

        // Pass the line into the Customer struct
        //if (sscanf(line, "%d\t%10s\t%10s\t%2s\t%2s\t%d",&customers[0].CustNum, customer.FirstName, customer.LastName, customer.Building, customer.Entrance, &customer.Floor) == 6) {
        if (sscanf(line, "%d\t%49s\t%49s\t%2s\t%2s\t%d", &customers[customerIndex].CustNum, customers[customerIndex].FirstName, customers[customerIndex].LastName, customers[customerIndex].Building, customers[customerIndex].Entrance, &customers[customerIndex].Floor) == 6) {
            //test
            //printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d\n", customer.CustNum, customer.FirstName, customer.LastName,customer.Building, customer.Entrance, customer.Floor);
        }
        // integrate this /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*
        int dest_building_row = customer.Building[0] - 'A'; // this finds the row of the destination building (y) based on the letter
        int dest_building_col = customer.Building[1] - 'A'; // this find the coloumn of the destination building (x) based on the letter
        int dest_y = originy + streeth * (dest_building_row + 1) + buildingh * dest_building_row + buildingh / 2; // sets dest coordinates to be later used in functions
        int dest_x = originx + avenuew * (dest_building_col + 1) + buildingw * dest_building_col + buildingw / 2;

        if (customer.Entrance[0] == 'N') {
            dest_y = dest_y - 1;
            if (customer.Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customer.Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customer.Entrance[0] == 'S') {
            dest_y = dest_y + 1;
            if (customer.Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customer.Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customer.Entrance[0] == 'E') {
            dest_x = dest_x - 1;
        }
        else if (customer.Entrance[0] == 'W') {
            dest_x = dest_x + 1;
        }
        else {
            printf("invalid input");
        }

        customer.locationx = dest_x;
        customer.locationy = dest_y;

        //test
        //printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, coords: %d %d\n", customer.CustNum, customer.FirstName, customer.LastName,customer.Building, customer.Entrance, customer.Floor, customer.locationx, customer.locationy);
        *

        int dest_building_row = customers[customerIndex].Building[0] - 'A';
        int dest_building_col = customers[customerIndex].Building[1] - 'A';
        int dest_y = originy + streeth * (dest_building_row + 1) + buildingh * dest_building_row + buildingh / 2;
        int dest_x = originx + avenuew * (dest_building_col + 1) + buildingw * dest_building_col + buildingw / 2;

        if (customers[customerIndex].Entrance[0] == 'N') {
            dest_y = dest_y - 1;
            if (customers[customerIndex].Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customers[customerIndex].Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customers[customerIndex].Entrance[0] == 'S') {
            dest_y = dest_y + 1;
            if (customers[customerIndex].Entrance[1] == 'E') {
                dest_x = dest_x - 1;
            }
            else if (customers[customerIndex].Entrance[1] == 'W') {
                dest_x = dest_x + 1;
            }
        }
        else if (customers[customerIndex].Entrance[0] == 'E') {
            dest_x = dest_x - 1;
        }
        else if (customers[customerIndex].Entrance[0] == 'W') {
            dest_x = dest_x + 1;
        }
        else {
            printf("invalid input");
        }

        customers[customerIndex].locationx = dest_x;
        customers[customerIndex].locationy = dest_y;
    }

    fclose(file);

    //new test
    for (int i = 0; i < customerIndex; i++) {
        printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, Coords: %d %d\n", customers[i].CustNum, customers[i].FirstName, customers[i].LastName, customers[i].Building, customers[i].Entrance, customers[i].Floor, customers[i].locationx, customers[i].locationy);
    }
}*/