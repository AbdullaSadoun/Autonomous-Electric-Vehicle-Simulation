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
/*
void draw_box(int ulr, int ulc, int height, int width) {
    int rc; // Row count 
    int cc; // Column count 

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

void label*/
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

void run_simulation(int NSno, int EWno, Customer customers[], Event events[]) { // added Event events[]
    /*
    This functions does the following:
    - generates the map
    - creates the car/s
    - takes the destination order from the user
    - "take car to destination"
    */

    COORD scrsize;
    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    scrsize = GetLargestConsoleWindowSize(Console);
    srand((unsigned int)time(NULL));

    //generating the map
    int mapwidth, mapheight;
    generateMap(NSno, EWno, &mapheight, &mapwidth);
    int map_width = &mapheight;
    int map_height = &mapwidth;

    FILE* fout = fopen("Delivery.txt", "w");
    fprintf(fout, "Package No.\tOriginCustomer\tDeliveryCustomer\tPickupTime\tDeliveryTime\n");


    //create the cars
    //Car cars[1] = { {originx + (avenuew / 2), originy + (streeth / 2), '1' } }; // creating the car
    Car cars[4];

    //Car cars[1] = { {originx, originy, '1' } }; // creating the car////////////////////////////////

    //for (int i = 0; i <= numberofcars; i++) {
    //    cars[i]={originx, originy, '1' + i};
    //}

    //PRE CONDITIONS:
    cars[0].x = 0;
    cars[0].y = 0;
    cars[0].symbol = 'x';
    cars[0].tempstate = 0;
    cars[0].batterylevel = 100;
    
    int fullfillingorder = 0; // yes=1 No=0
    cars[0].availability = 1; // 1=available, 0=unavailable
    int waittime=0;
    int reachedorigin = 0;
    int reacheddestination = 0;
    int recordneeded = 0;
    int exitloop = 0;
    int timecount=0;
    int eventindex=0;

    int senderindex, receiverindex;

    cars[0].waittime = 0;

    while (1) { // make this while(there are no more orders.) //exitloop != 1 

        senderindex = events[eventindex].OriginCust - 1000;
        receiverindex = events[eventindex].DestinationCust - 1000;
        // is there a current order being fulfilled?
        if (fullfillingorder == 0) {
            // get the time for the event
            if (events[eventindex].time <= timecount) { // changed was ==
                if (cars[0].availability == 1) {
                    fullfillingorder = 1;
                    cars[0].availability = 0;
                    cars[0].tempstateset = 0;
                    events[eventindex].pickuptime = timecount;
                    senderindex = events[eventindex].OriginCust - 1000;
                    receiverindex = events[eventindex].DestinationCust - 1000;

                    if (cars[0].x == customers[senderindex].buildingx +2 && cars[0].y == customers[senderindex].buildingy+2) {
                        reachedorigin = 1;
                        cars[0].waittime = (customers[senderindex].Floor * 15) + (customers[senderindex].Floor * 10);
                        //continue;
                    }
                    else {
                        if (reachedorigin == 1) {
                            goto gotodestination;
                        }
                        else {
                        gotodestination2:
                       
                            if (cars[0].x == customers[senderindex].buildingx +2 && cars[0].y == customers[senderindex].buildingy+2) { // new 
                                reachedorigin = 1; // new 
                                cars[0].waittime = (customers[senderindex].Floor * 15) + (customers[senderindex].Floor * 10);
                                clear_car(cars[0]); //new
                                move_cursor(3, 50);
                                printf("car is at %s", customers[senderindex].Building);
                            }
                            else {
                                move_cursor(3, 50); //NEW STUFF HEREHHREHRHERHERHREHR
                                printf("              "); // clear the car arrived message
                                move_car_to_destination(&cars[0], customers[senderindex].buildingx, customers[senderindex].buildingy, map_width, map_height);
                            }
                        }
                    }
                }
            }
        }
        else { // fullfillingorder = 1
            if (cars[0].waittime == 0 && reachedorigin == 1) {
            gotodestination:
                if (cars[0].x == customers[receiverindex].buildingx+2 && cars[0].y == customers[receiverindex].buildingy+2) { //yes
                    if (recordneeded == 0) {
                        recordneeded = 1;
                        reacheddestination = 1;
                        clear_car(cars[0]); //new
                        move_cursor(3, 50);
                        printf("car is at %s", customers[receiverindex].Building);
                        cars[0].waittime = (customers[receiverindex].Floor * 15) + (customers[receiverindex].Floor * 10);
                    }
                    else { // recordneeded ==1

                        //making the record
                        events[eventindex].deliverytime = timecount;
                        fprintf(fout, "500\t%d\t%d\%d\t%d\n", events[eventindex].OriginCust, events[eventindex].DestinationCust, events[eventindex].pickuptime, events[eventindex].deliverytime); // make sure to add not overwrite 
                        //resetting the variables
                        move_cursor(3, 50);
                        printf("              "); // clear the car arrived message
                        fullfillingorder = 0; // yes=1 No=0
                        cars[0].availability = 1;
                        cars[0].waittime = 0;
                        reachedorigin = 0;
                        reacheddestination = 0;
                        recordneeded = 0;
                        exitloop = 1;
                        eventindex++;
                        if (eventindex > MAX_EVENTS) {
                            break;
                        }
                    }
                }
                else {
                    move_cursor(3, 50);
                    printf("              "); // clear the car arrived message
                    move_car_to_destination(&cars[0], customers[receiverindex].buildingx, customers[receiverindex].buildingy, map_width, map_height);
                }
            }
            else { // waittime!=0 or reachedoring !=1
                if (cars[0].waittime == 0) {
                    goto gotodestination2;
                }
                cars[0].waittime--;
            }
        }

        //increment time counter
        Sleep(10);
        timecount++;
        move_cursor(1, 50);
        //move_cursor(2, map_width+50);
        printf("time: %d", timecount);
        move_cursor(2, 50);
        printf("car @: %d %d. Car Batter level = %d. Tempstate:%d. WT: %d", cars[0].x, cars[0].y, cars[0].batterylevel, cars[0].tempstate, cars[0].waittime);
        //printf("time: %d", timecount);
    }

    fclose(fout);
    return;
}