#include "PT53.h"

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

void move_cursor(int row, int col) // move the cursor to where the next character will printed
{
    // Move cursor to specified row and column 
    COORD coord;
    coord.X = col;
    coord.Y = row;
    //Windows function to position cursor 
    SetConsoleCursorPosition(Console, coord);

}

void draw_box(int ulr, int ulc, int height, int width) {
    int rc; /* Row count */
    int cc; /* Column count */

    // Draw Top Edge
    move_cursor(ulr, ulc);
    DISP(UL);
    for (cc = 0; cc < width - 2; cc++)
        DISP(HBAR);
    DISP(UR);

    // Draw Vertical Edges
    for (rc = 1; rc < height - 1; rc++) {
        move_cursor(ulr + rc, ulc);
        DISP(VBAR);
        move_cursor(ulr + rc, ulc + width - 1);
        DISP(VBAR);
    }

    // Draw Bottom Edge
    move_cursor(ulr + height - 1, ulc);
    DISP(LL);
    for (cc = 0; cc < width - 2; cc++)
        DISP(HBAR);
    DISP(LR);
}

// my functions //
void label_building(int y, int x, char label1, char label2) {
    move_cursor(y + buildingh / 2, x + buildingw / 2 - 1);
    printf("%c%c", label1, label2);
}

/*void add_building_to_coords(int x, int y) { // this adds every coordinate used in the building to an array // function no longer needed
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            building_coords[total_building_coords].x = x + j;
            building_coords[total_building_coords].y = y + i;
            total_building_coords++;
        }
    }
}*/

/*int is_building_at(int x, int y) { // this function is no longer needed
    for (int i = 0; i < total_building_coords; i++) {
        if (building_coords[i].x == x && building_coords[i].y == y) {
            return 1;
        }
    }
    return 0;
}*/

/*
void move_car_to_destination(Car* car, int dest_x, int dest_y) {
    /*
    - this is the primary function that is used to move the vehicles.
    - the function creates new temporary coordinates that take the car to closest intersection
    - it moves the car by comparing the current car coordinates to destination coordinates
    - after the car gets to the closest interection it easily makes its way to the building
    - it does that by doing the same method above my comparing coordinates but this time to destination coordinates

    -Update the following:
        - function must go to specific entrances
        - function has to take into account the directions of the streets and avenues

    *


    int temp_x = (dest_x / 4) * 4;
    int temp_y = (dest_y / 4) * 4;

    // move towards the temporary coordinates (closest intersection)
    while (car->x != temp_x || car->y != temp_y) {
        Sleep(500);
        clear_car(*car);

        if (car->x < temp_x) {
            car->x++;
        }
        else if (car->x > temp_x) {
            car->x--;
        }
        else {
            if (car->y < temp_y) {
                car->y++;
            }
            else if (car->y > temp_y) {
                car->y--;
            }
        }

        draw_car(*car);
        move_cursor(10, 15);
        printf("car @: %d %d", car->x, car->y);

        if (car->x == dest_x && car->y == dest_y) { // check if you have reached the destination
            printf(". Destination reached!, car is at: %d %d\n", dest_x, dest_y);
            Sleep(3000);
            return; // exit the function since destination is reached
        }
    }

    // move towards the final destination if not already reached
    while (car->x != dest_x || car->y != dest_y) {
        Sleep(500);
        clear_car(*car);

        if (car->x < dest_x) {
            car->x++;
        }
        else if (car->x > dest_x) {
            car->x--;
        }
        else {
            if (car->y < dest_y) {
                car->y++;
            }
            else if (car->y > dest_y) {
                car->y--;
            }
        }

        draw_car(*car);
        move_cursor(10, 15);
        printf("car @: %d %d", car->x, car->y);

        if (car->x == dest_x && car->y == dest_y) { // check if you have reached the destination
            printf(". Destination reached!, car is at: %d %d\n", dest_x, dest_y);
            //printf("map width=%d map height=%d", );
            Sleep(3000);
            return; // exit the function since destination is reached
        }
    }
}*/

void draw_car(Car car) {
    move_cursor(car.y, car.x);
    printf("%c", car.symbol);
    //printf("%d %d", car.x, car.y); // test
}

void clear_car(Car car) {
    move_cursor(car.y, car.x);
    printf(" ");
}

void move_car_to_destination(Car* car, int dest_x, int dest_y, int map_width, int map_height) {
    /*
    - this is the primary function that is used to move the vehicles.
    - the function creates new temporary coordinates that take the car to closest intersection
    - it moves the car by comparing the current car coordinates to destination coordinates
    - after the car gets to the closest interection it easily makes its way to the building
    - it does that by doing the same method above my comparing coordinates but this time to destination coordinates

    -Update the following:
        - function must go to specific entrances (it does)
        - Second part of the function should have rules implemented on it.
        - function has to take into account the directions of the streets and avenues (it does)
        - stop car from going out of the map

    */

    int temp_x = (dest_x / 4) * 4;
    int temp_y = (dest_y / 4) * 4;

    Sleep(100);
    clear_car(*car);

    if(car->tempstate == 0){
        if (car->x < temp_x) { // moving in x-axis
            if ((car->y % 4 == 0) && ((car->y / 4) % 2 == 0) || (car->y == 0) || (car->y == map_height)) {
                // if car on movable street, and street/4 is odd numbered street or car is on top strret or botom
                car->x++;
            }
            else {
                temp_y = temp_y + 4;//increment temporary y by 4
                draw_car(*car);
            }
        }
        else if (car->x > temp_x) {
            // Check if y is on an even-numbered block and if it's allowed to move
            if ((car->y % 4 == 0) && ((car->y / 4) % 2 != 0) || (car->y == 0) || (car->y == map_height)) {
                car->x--;
            }
            else {
                temp_y = temp_y + 4; //increment temporary y by 4
                draw_car(*car);
                
            }
        
        }
        else {
            if (car->y < temp_y) {
                // Check if x is on an even-numbered block and if it's allowed to move
                if ((car->x % 4 == 0) && ((car->x / 4) % 2 == 0) || (car->x == 0) || (car->x == map_width)) {
                    car->y++;
                }
                else {
                    temp_x = temp_x + 4; //increment temporary x by 4
                    //draw_car(*car);
                    //return;
                }
            }
            else if (car->y > temp_y) {
                // Check if x is on an even-numbered block and if it's allowed to move
                if ((car->x % 4 == 0) && ((car->x / 4) % 2 != 0) || (car->x == 0) || (car->x == map_width)) {
                    car->y--;
                }
                else {
                    temp_x = temp_x + 4;//increment temporary x by 4
                    //draw_car(*car);
                    //return;
                }
            }
            else {
                car->tempstate = 1;
                return;
            }
        }
    } else {
        if (car->x < dest_x) {
            car->x++;
        }
        else if (car->x > dest_x) {
            car->x--;
        }
        else {
            if (car->y < dest_y) {
                car->y++;
            }
            else if (car->y > dest_y) {
                car->y--;
            }
        }
    }

    car->batterylevel = car->batterylevel - 1;
    draw_car(*car);
    move_cursor(10, 15);
    printf("car @: %d %d. Car Batter level = %d. Tempstate:%d", car->x, car->y, car->batterylevel, car->tempstate);

    if (car->x == dest_x && car->y == dest_y) { // check if you have reached the destination
        printf(". Destination reached!, car is at: %d %d\n", dest_x, dest_y);
        Sleep(3000);
        car->tempstate = 0;
        return; // exit the function since destination is reached
    }

    return;

}

void readprcoessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    //const int MAX_CUSTOMERS = 200;
    //Customer customers[MAX_CUSTOMERS];
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
        */

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
        printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, Coords: %d %d\n",customers[i].CustNum, customers[i].FirstName, customers[i].LastName, customers[i].Building, customers[i].Entrance, customers[i].Floor, customers[i].locationx, customers[i].locationy);
    }
} 

//event file function
/*
void readprocessEventF(char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Customer customer;
    char line[100]; // Adjust size as needed

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove potential newline character
        line[strcspn(line, "\n")] = 0;

        // Pass the line into the Customer struct
        if (sscanf(line, "%d\t%10s\t%10s\t%2s\t%2s\t%d", &customer.CustNum, customer.FirstName, customer.LastName, customer.Building, customer.Entrance, &customer.Floor) == 6) {
            //test
            //printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d\n", customer.CustNum, customer.FirstName, customer.LastName,customer.Building, customer.Entrance, customer.Floor);
        }

        //test
        //printf("CustNum: %d, First Name: %s, Last Name: %s, Building: %s, Entrance: %s, Floor: %d, coords: %d %d\n", customer.CustNum, customer.FirstName, customer.LastName,customer.Building, customer.Entrance, customer.Floor, customer.locationx, customer.locationy);
    }

    fclose(file);
}*/ // 

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

void run_simulation(int NSno, int EWno, Customer customers[]) { // added Customer customers[]
    /*
    This functions does the following:
    - generates the map
    - creates the car/s
    - takes the destination order from the user
    - "take car to destination"
    */

    COORD scrsize;
    //int map_height, map_width; 
    int map_height = (NSno * (buildingh + streeth)) + streeth;
    int map_width = (EWno * (buildingw + avenuew)) + avenuew;
    int buildingx, buildingy;
    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    scrsize = GetLargestConsoleWindowSize(Console);
    //printf("Size: r: %d c: %d\n\n", scrsize.Y, scrsize.X); // not needed for now
    srand((unsigned int)time(NULL));


    for (int i = 0; i < NSno; ++i) { // creating and labeling buildings
        for (int j = 0; j < EWno; ++j) {
            Sleep(15);
            buildingx = originx + avenuew * (j + 1) + buildingw * j;
            buildingy = originy + streeth * (i + 1) + buildingh * i;
            draw_box(buildingy, buildingx, buildingh, buildingw); // drawing the building boxes
            label_building(buildingy, buildingx, 'A' + i, 'A' + j); // labeling buildings
            //add_building_to_coords(buildingx, buildingy); // store buildings coordinates 
        }
    }

    
    //printf("how many cars?(1-4)");
    //scanf("%d");

    //create the cars
    //Car cars[1] = { {originx + (avenuew / 2), originy + (streeth / 2), '1' } }; // creating the car
    Car cars[4];

    //Car cars[1] = { {originx, originy, '1' } }; // creating the car////////////////////////////////

    //for (int i = 0; i <= numberofcars; i++) {
    //    cars[i]={originx, originy, '1' + i};
    //}

        //car conditions
    cars[0].x = 0;
    cars[0].y = 0;
    //cars[0].destination.x = dest_x;
    //cars[0].destination.y = dest_y;
    cars[0].symbol = 'x';
    cars[0].tempstate = 0;
    cars[0].batterylevel = 100;
    
    Event events;
    events.time = 100;
    events.type = 'D';
    events.OriginCust = 1000;
    events.DestinationCust = 1044;
    events.Weight = 10;
    


    int timecount = 0;
    int fullfillingorder = 0; // yes=1 No=0
    cars[0].availability = 1;
    int waittime=0;
    int reachedorigin = 0;
    int reacheddestination = 0;
    int recordneeded = 0;

    //int destx, localdesty, localfloor;
    //int sender 
    
    while (1) { // make this while(there are no more orders.)
        int senderindex = events.OriginCust - 1000;
        int receiverindex = events.DestinationCust - 1000;
        // is there a current order being fulfilled?
        if (fullfillingorder == 0) {
            // get the time for the event
            if (events.time == timecount) { // change
                if (cars[0].availability == 1) {
                    fullfillingorder = 1;
                    cars[0].availability = 0;
                    //get origin coordinates from the files
                    int senderindex = events.OriginCust - 1000;
                    int receiverindex = events.DestinationCust - 1000;
                    //localdestx = customers[senderindex].locationx;
                    // localdestx = customer[receiverindex].
                    //localdesty = customers[senderindex].locationy;
                    //localfloor = customers[senderindex].Floor;

                    if (cars[0].x == customers[senderindex].locationx && cars[0].y == customers[senderindex].locationy) {
                        reachedorigin = 1;
                        waittime = (customers[senderindex].Floor * 15) + (customers[senderindex].Floor * 10);
                        continue;
                    }
                    else {
                        if (reachedorigin == 1) {
                            goto gotodestination;
                        }
                        else {
                            move_car_to_destination(&cars[0], customers[senderindex].locationx, customers[senderindex].locationy, map_width, map_height);
                            //put origin coordinates as destination);
                            continue;
                        }
                    }
                }
                else { // car is not available
                    continue;
                }
            }
            else { // events.time!=time
                continue;
            }
        }
        else { // fullfillingorder = 1
            if (waittime == 0 && reachedorigin == 1) {
            gotodestination:
                if (cars[0].x == customers[receiverindex].locationx && cars[0].y == customers[receiverindex].locationy) { //yes
                    if (recordneeded == 0) {
                        recordneeded = 1;
                        reacheddestination = 1;
                        //waittime = (floors * 15) + (floors * 10); // for origin
                        // HARDCODE:
                        waittime = (customers[receiverindex].Floor * 15) + (customers[receiverindex].Floor * 10);
                        continue;
                    }
                    else { // recordneeded ==1
                        //making the record
                            //fprintf(fout);
                        //resetting the variables
                        int fullfillingorder = 0; // yes=1 No=0
                        cars[0].availability = 1;
                        int waittime = 0;
                        int reachedorigin = 0;
                        int reacheddestination = 0;
                        int recordneeded = 0;
                    }
                }
                else {
                    move_car_to_destination(&cars[0], customers[receiverindex].locationx, customers[receiverindex].locationy, map_width, map_height); // HARDCODE
                    continue;
                }

            }
            else { // waittime!=0 or reachedoring !=1
                waittime--;
                continue;
            }
            //increment time counter
            timecount++;
            move_cursor(21, 10);
            printf("time: %d", timecount);
        }
    }
    



    // getting destination 
    //char dest_building[3]; // the coordinates of the destination building
    //char dest_quad[3]; // to get the coordinates of the quadrant in the building
    //printf("\n\n\n\nEnter destination building (e.g., AA, AB, etc.): "); // prompt user to enter dersired destination
    //scanf("%2s", dest_building); //%2s to limit the input to 2
    //int dest_building_row = dest_building[0] - 'A'; // this finds the row of the destination building (y) based on the letter
    //int dest_building_col = dest_building[1] - 'A'; // this find the coloumn of the destination building (x) based on the letter
    //int dest_y = originy + streeth * (dest_building_row + 1) + buildingh * dest_building_row + buildingh / 2; // sets dest coordinates to be later used in functions
    //int dest_x = originx + avenuew * (dest_building_col + 1) + buildingw * dest_building_col + buildingw / 2;
    ////setting the entrance
    //printf("Enter the destination Quadrant (e.g., N, S, NE, NW, etc): ");
    //scanf("%s", dest_quad);
    //dest_quad[2] = '\0';  // Null terminate the string
    ////setting the entrance
    //if (dest_quad[0] == 'N') {
    //    dest_y = dest_y - 1;
    //    if (dest_quad[1] == 'E') {
    //        dest_x = dest_x - 1;
    //    }
    //    else if (dest_quad[1] == 'W') {
    //        dest_x = dest_x + 1;
    //    }
    //}
    //else if (dest_quad[0] == 'S') {
    //    dest_y = dest_y + 1;
    //    if (dest_quad[1] == 'E') {
    //        dest_x = dest_x - 1;
    //    }
    //    else if (dest_quad[1] == 'W') {
    //        dest_x = dest_x + 1;
    //    }
    //}
    //else if (dest_quad[0] == 'E') {
    //    dest_x = dest_x - 1;
    //}
    //else if (dest_quad[0] == 'W') {
    //    dest_x = dest_x + 1;
    //}
    //else {
    //    printf("invalid input");
    //}
    //printf("car will go to: %d %d", dest_x, dest_y);

    //car conditions
    cars[0].x = 0;
    cars[0].y = 0;
    //cars[0].destination.x = dest_x;
    //cars[0].destination.y = dest_y;
    cars[0].symbol = 'x';
    cars[0].tempstate = 0;
    cars[0].batterylevel = 100;

    //while (cars->x != dest_x || cars->y != dest_y) {
    //    move_car_to_destination(&cars[0], dest_x, dest_y, map_width, map_height); // moving the car to the destination
    //    timecount++;
    //    move_cursor(21, 10);
    //    printf("time: %d", timecount);
    //}

    //becomes:
    //if (cars->x != dest_x || cars->y != dest_y) {
    //    move_car_to_destination(&cars[0], dest_x, dest_y, map_width, map_height); // moving the car to the destination
    //}

    return;
}