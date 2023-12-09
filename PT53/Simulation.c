#include "Simulation.h"

void move_cursor(int row, int col) // move the cursor to where the next character will printed
{
    // Move cursor to specified row and column 
    COORD coord;
    coord.X = col;
    coord.Y = row;
    //Windows function to position cursor 
    SetConsoleCursorPosition(Console, coord);
}

/*void add_building_to_coords(int x, int y) { // function no longer needed
 /*
 this adds every coordinate used in the building to an array
 *
 for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            building_coords[total_building_coords].x = x + j;
            building_coords[total_building_coords].y = y + i;
            total_building_coords++;
        }
    }
}*/

/*int is_building_at(int x, int y) { // this function is no longer needed
* checks if the coordinates in the argument have a building on them.
    for (int i = 0; i < total_building_coords; i++) {
        if (building_coords[i].x == x && building_coords[i].y == y) {
            return 1;
        }
    }
    return 0;
}*/

void run_simulation(int NSno, int EWno, Customer customers[], Event events[], FILE*Delivery, FILE*Vehicle) { // added Event events[]
    /*
    This functions does the following:
    - generates the map
    - creates the car/s
    - sets the preconditions
    - takes the destination order from the user
    - Makes the deliveries and runs the software in the while loop
    */
    

    //FILE* fout = fopen("Delivery.txt", "w");

    
    //fprintf(fout, "Package No.\tOriginCustomer\tDeliveryCustomer\tPickupTime\tDeliveryTime\n");

    COORD scrsize;
    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    scrsize = GetLargestConsoleWindowSize(Console);
    srand((unsigned int)time(NULL));
 
    /*
    Car cars[MAX_CARS]; // Declaring the array of Cars.

    for (int i = 0; i < MAX_CARS; i++) { //setting the preconditions for the car/s:
        printf("choose car%d?... 1 for yes, 0 for no: ", i);
        scanf("%d", &cars[i].chosen);
        cars[i].x = 0;
        cars[i].y = 0;
        cars[i].symbol = 'w' + i;
        cars[i].tempstate = 0;
        cars[i].batterylevel = 100;
        cars[i].milage = 0;
        cars[i].fullfillingorder = 0;
        cars[i].reachedorigin = 0;
        cars[i].reacheddestination = 0;
        cars[0].recordneeded = 0;//
        cars[0].senderindex = 0;//
        cars[i].Deliveryno = 0;
        cars[i].availability = 1; // 1=available, 0=unavailable
        cars[i].waittime = 0;    
        cars[i].maxbatterylevel = 100;
        cars[i].batteryrechargerate = 1; // kW.t
        cars[i].totaltimecharging = 0;
    }
    */
    int maxCars;
    printf("Enter the maximum number of cars: ");
    scanf("%d", &maxCars);

    // Dynamically allocate memory for the array of cars
    Car* cars = (Car*)malloc(maxCars * sizeof(Car));


    // Rest of your code for initializing and setting up each car
    for (int i = 0; i < maxCars; i++) {
        printf("choose car%d?... 1 for yes, 0 for no: ", i);
        scanf("%d", &cars[i].chosen);

        cars[i].VIN = 100 + (i * 2);  //set vin which starts at 100 and increments by 2 for each car
        cars[i].x = 0; // set x and y to 0
        cars[i].y = 0;
        cars[i].symbol = 'a' + i; // set car symbol to a + i
        cars[i].tempstate = 0;
        cars[i].batterylevel = 100;
        cars[i].milage = 0;
        cars[i].fullfillingorder = 0;
        cars[i].reachedorigin = 0;
        cars[i].reacheddestination = 0;
        cars[i].recordneeded = 0;//
        cars[i].senderindex = 0;//
        cars[i].Deliveryno = 0;
        cars[i].availability = 1; // 1=available, 0=unavailable
        cars[i].waittime = 0;
        cars[i].maxbatterylevel = 100;
        cars[i].batteryrechargerate = 1; // kW.t
        cars[i].totaltimecharging = 0;
        cars[i].drivingdischarge = 1;
        cars[i].IdlingDischargeRate = 0;
        cars[i].TotalTimeMoving = 0;
        cars[i].totalwaittime = 0;
        cars[i].totaltimecharging = 0;
    }

    for (int i = 0; i < maxCars+1; i++) {
        move_cursor(i, 0);
        printf("                                                      ");
    }
    
    /*for (int i = 0; i < MAX_CARS; i++) {
        move_cursor(i, 0);
        printf("                                          ");
    }*/

    //generating the map
    int mapwidth, mapheight;
    generateMap(NSno, EWno, &mapheight, &mapwidth);
    int map_width = &mapheight;
    int map_height = &mapwidth;

    //FILE* Vehicle = fopen("Vehicle.txt", "w");
    //fprintf();

    int exitloop = 0;
    int timecount = 0;
    int eventindex = 0;
    int Deliveryno = 0;
    
    int i = 0;
    int Carindex=0;
    //carindex =i
    int why=0;

    while (1) { // make this while(there are no more orders.) //exitloop != 1 

        for (int i = 0; i < maxCars;i++) {
            if (cars[i].chosen==1) {
                if (cars[i].fullfillingorder == 0) {
                    fprintf(Vehicle, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", cars[i].VIN, cars[i].laststable, cars[i].maxbatterylevel, cars[i].batterylevel, cars[i].batteryrechargerate, cars[i].drivingdischarge, cars[i].IdlingDischargeRate, cars[i].TotalTimeMoving, cars[i].totalwaittime, cars[i].totaltimecharging);
                    fflush(Vehicle);
                    // get the time for the event
                    if (events[eventindex].time <= timecount) { // changed was ==
                        if (cars[i].availability == 1) {
                            cars[i].assignedeventindex = eventindex;
                            eventindex++;
                            cars[i].fullfillingorder = 1;
                            cars[i].availability = 0;
                            cars[i].tempstateset = 0;
                            events[cars[i].assignedeventindex].pickuptime = timecount; // 
                            cars[i].senderindex = events[cars[i].assignedeventindex].OriginCust - 1000;
                            cars[i].receiverindex = events[cars[i].assignedeventindex].DestinationCust - 1000;
                            //eventindex++;
                            
                            if ((cars[i].x == customers[cars[i].senderindex].buildingx + 2) && (cars[i].y == customers[cars[i].senderindex].buildingy + 2)) {
                                cars[i].reachedorigin = 1;
                                cars[i].waittime = (customers[cars[i].senderindex].Floor * 15) + (customers[cars[i].senderindex].Floor * 10);
                                cars[i].totalwaittime = cars[i].totalwaittime + cars[i].waittime;
                                //continue;
                            }
                            else {
                                if (cars[i].reachedorigin == 1) {
                                    goto gotodestination;
                                }
                                else {
                                gotodestination2:

                                    if (cars[i].x == customers[cars[i].senderindex].buildingx + 2 && cars[i].y == customers[cars[i].senderindex].buildingy + 2) { // new 
                                        cars[i].reachedorigin = 1; // new 
                                        cars[i].waittime = (customers[cars[i].senderindex].Floor * 15) + (customers[cars[i].senderindex].Floor * 10);
                                        cars[i].totalwaittime = cars[i].totalwaittime + cars[i].waittime;
                                        clear_car(cars[i]); //new
                                        move_cursor(8 + i, 50);
                                        printf("car%d is at %s", i, customers[cars[i].senderindex].Building);
                                    }
                                    else {
                                        move_cursor(8+i, 50); //NEW STUFF HEREHHREHRHERHERHREHR
                                        printf("              "); // clear the car arrived message
                                        move_car_to_destination(&cars[i], customers[cars[i].senderindex].buildingx, customers[cars[i].senderindex].buildingy, map_width, map_height);
                                    }
                                }
                            }
                        }
                    }
                }
                else { // fullfillingorder = 1
                    if (cars[i].waittime == 0 && cars[i].reachedorigin == 1) {
                    gotodestination:
                        if (cars[i].x == customers[cars[i].receiverindex].buildingx + 2 && cars[i].y == customers[cars[i].receiverindex].buildingy + 2) { //yes
                            if (cars[i].recordneeded == 0) {
                                cars[i].recordneeded = 1;
                                cars[i].reacheddestination = 1;
                                clear_car(cars[i]); //new
                                move_cursor(8 + i, 50);
                                printf("car%d is at %s", i, customers[cars[i].receiverindex].Building);
                                cars[i].waittime = (customers[cars[i].receiverindex].Floor * 15) + (customers[cars[i].receiverindex].Floor * 10);
                            }
                            else { // recordneeded ==1

                                //making the record
                                events[cars[i].assignedeventindex].deliverytime = timecount;
                                fprintf(Delivery, "%d\t%d\t%d\t%d\t%d\n", cars[i].assignedeventindex + 500, events[cars[i].assignedeventindex].OriginCust, events[cars[i].assignedeventindex].DestinationCust, events[cars[i].assignedeventindex].pickuptime, events[cars[i].assignedeventindex].deliverytime); // make sure to add not overwrite 
                                fflush(Delivery);
                                //resetting the variables
                                move_cursor(8+i, 50);
                                printf("              "); // clear the car arrived message
                                // set the cars last stable as the entrance of the receiver customer
                                //copy the content of customers[cars[i].receiverindex].Entrance to cars[i].laststable
                                cars[i].laststable[0] = customers[cars[i].receiverindex].Entrance[0];
                                cars[i].laststable[1] = customers[cars[i].receiverindex].Entrance[1];
                                cars[i].laststable[2] = customers[cars[i].receiverindex].Entrance[2];


                                cars[i].fullfillingorder = 0; // yes=1 No=0
                                cars[i].availability = 1; // 1=available, 0=unavailable
                                cars[i].waittime = 0; // wait time
                                cars[i].reachedorigin = 0; // reset reached origin
                                cars[i].reacheddestination = 0; // reset reached destination
                                cars[i].recordneeded = 0; // reset record needed
                                Deliveryno++;
                                if (eventindex >= MAX_EVENTS) { 
                                    break;
                                }
                            }
                        }
                        else {
                            move_cursor(8+i, 50);
                            printf("              "); // clear the car arrived message
                            move_car_to_destination(&cars[i], customers[cars[i].receiverindex].buildingx, customers[cars[i].receiverindex].buildingy, map_width, map_height);
                        }
                    }
                    else { // waittime!=0 or reachedoring !=1
                        if (cars[i].waittime == 0) {
                            goto gotodestination2;
                        }
                        cars[i].waittime--;
                        if (cars[i].batterylevel < cars[i].maxbatterylevel) {
                            cars[i].batterylevel = cars[i].batterylevel + cars[i].batteryrechargerate;
                            cars[i].totaltimecharging++;
                        }
                    }
                }
                why = 2+i;
                move_cursor(why, 50);
                printf("                                                                      ");
                move_cursor(why, 50);
                printf("car%d @: %d %d.Battery:%d tmilage:%d waittime:%d", i, cars[i].x, cars[i].y, cars[i].batterylevel, cars[i].milage, cars[i].waittime);
                why = 0;

            }
        }
        

        // updating the time counter
        Sleep(10);
        timecount++; // increment the time counter
        move_cursor(1, 50); // move the cursor to the time
        printf("time: %d", timecount); // print the time

        
        //for (int i = 0; i < maxCars; i++) { // print out vehicle data
        //    fprintf(Vehicle, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", cars[i].VIN, cars[i].laststable, cars[i].maxbatterylevel, cars[i].batterylevel, cars[i].batteryrechargerate, cars[i].drivingdischarge, cars[i].IdlingDischargeRate, cars[i].TotalTimeMoving, cars[i].totalwaittime, cars[i].totaltimecharging);
        //    fflush(Vehicle); 
        //}
        
    }

    free(cars);
    return;
}