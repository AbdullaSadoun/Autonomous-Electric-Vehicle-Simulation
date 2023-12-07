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
 
    Car cars[4];

    //setting the preconditions for the car:
    for (int i = 0; i < MAX_CARS; i++) {
        printf("choose car%d?... 1 for yes, 0 for no: ", i);
        scanf("%d", &cars[i].chosen);
        //cars[i].chosen = 1;
        cars[i].x = 0;
        cars[i].y = 0;
        cars[i].symbol = 'p' + 1;
        cars[i].tempstate = 0;
        cars[i].batterylevel = 100;
        cars[i].milage = 0;
        cars[i].fullfillingorder = 0;
        cars[i].reachedorigin = 0;
        cars[i].reacheddestination = 0;
        //cars[0].recordneeded = 0;
        //cars[0].senderindex = 0
        cars[i].Deliveryno = 0;
        cars[i].availability = 1; // 1=available, 0=unavailable
        cars[i].waittime = 0;    
    }

    /*
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            move_cursor(i, j);
            printf(" ");
        }
    }*/
    

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

    //FILE* Vehicle = fopen("Vehicle.txt", "w");
    //fprintf();

    //Car cars[4];

    //PRE CONDITIONS:
    //cars[0].chosen = 1;
    //cars[0].x = 0;
    //cars[0].y = 0;
    //cars[0].symbol = 'x';
    //cars[0].tempstate = 0;
    //cars[0].batterylevel = 100;
    //cars[0].milage = 0;
    //cars[0].fullfillingorder = 0;
    //cars[0].reachedorigin = 0;
    //cars[0].reacheddestination=0;
    ////cars[0].recordneeded=0;
    ////cars[0].senderindex = 0
    //cars[0].Deliveryno=0;
    //
    //cars[0].availability = 1; // 1=available, 0=unavailable
    //cars[0].waittime = 0;

    //cars[1].chosen = 1;
    //cars[1].x = 0;
    //cars[1].y = 0;
    //cars[1].symbol = 'y';
    //cars[1].tempstate = 0;
    //cars[1].batterylevel = 100;
    //cars[1].milage = 0;
    //cars[1].fullfillingorder = 0;
    //cars[1].reachedorigin = 0;
    //cars[1].reacheddestination = 0;
    ////cars[0].recordneeded=0;
    ////cars[0].senderindex = 0
    //cars[1].Deliveryno = 0;
    //cars[1].availability = 1; // 1=available, 0=unavailable
    //cars[1].waittime = 0;


    int exitloop = 0;
    int timecount = 0;
    int eventindex = 0;
    int Deliveryno = 0;
    
    int i = 0;
    int Carindex=0;
    //carindex =i
    int why=0;

    while (1) { // make this while(there are no more orders.) //exitloop != 1 

        for (int i = 0; i < 4;i++) {
            if (cars[i].chosen==1) {
                if (cars[i].fullfillingorder == 0) {
                    // get the time for the event
                    if (events[eventindex].time <= timecount) { // changed was ==
                        if (cars[i].availability == 1) {
                            cars[i].assignedeventindex = eventindex;
                            eventindex++;
                            cars[i].fullfillingorder = 1;
                            cars[i].availability = 0;
                            cars[i].tempstateset = 0;
                            events[cars[i].assignedeventindex].pickuptime = timecount;
                            cars[i].senderindex = events[cars[i].assignedeventindex].OriginCust - 1000;
                            cars[i].receiverindex = events[cars[i].assignedeventindex].DestinationCust - 1000;
                            eventindex++;

                            if (cars[i].x == customers[cars[i].senderindex].buildingx + 2 && cars[i].y == customers[cars[i].senderindex].buildingy + 2) {
                                cars[0].reachedorigin = 1;
                                cars[0].waittime = (customers[cars[i].senderindex].Floor * 15) + (customers[cars[i].senderindex].Floor * 10);
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
                                clear_car(cars[0]); //new
                                move_cursor(8 + i, 50);
                                printf("car%d is at %s", i, customers[cars[i].receiverindex].Building);
                                cars[i].waittime = (customers[cars[i].receiverindex].Floor * 15) + (customers[cars[i].receiverindex].Floor * 10);
                            }
                            else { // recordneeded ==1

                                //making the record
                                events[eventindex].deliverytime = timecount;
                                fprintf(fout, "%d\t%d\t%d\t%d\t%d\n", cars[i].assignedeventindex + 500, events[cars[i].assignedeventindex].OriginCust, events[cars[i].assignedeventindex].DestinationCust, events[cars[i].assignedeventindex].pickuptime, events[cars[i].assignedeventindex].deliverytime); // make sure to add not overwrite 
                                //resetting the variables
                                move_cursor(8+i, 50);
                                printf("              "); // clear the car arrived message
                                cars[i].fullfillingorder = 0; // yes=1 No=0
                                cars[i].availability = 1;
                                cars[i].waittime = 0;
                                cars[i].reachedorigin = 0;
                                cars[i].reacheddestination = 0;
                                cars[i].recordneeded = 0;
                                exitloop = 1;
                                //eventindex++;
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
                        if (cars[i].batterylevel < 100) {
                            cars[i].batterylevel++;
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

        
        //increment time counter
        Sleep(10);
        timecount++;
        move_cursor(1, 50);
        //move_cursor(2, map_width+50);
        printf("time: %d", timecount);
        
        
        //move_cursor(2, 50);
        //printf("                                                                      ");
        //move_cursor(2, 50);
        //printf("car %d @: %d %d.Battery:%d tmilage:%d waittime:%d", cars[0].x, cars[0].y, cars[0].batterylevel, cars[0].milage, cars[0].waittime);
        
        //printf("time: %d", timecount);
    }

    /*
    while (1) { // make this while(there are no more orders.) //exitloop != 1 


        // is there a current order being fulfilled?
        //for (int i = 0; i < 4;i++) {
        //    if (cars[i].chosen==1) {

        //    }
        //}

        if (cars[0].fullfillingorder == 0) {
            // get the time for the event
            if (events[eventindex].time <= timecount) { // changed was ==
                if (cars[0].availability == 1) {
                    cars[0].fullfillingorder = 1;
                    cars[0].availability = 0;
                    cars[0].tempstateset = 0;
                    events[eventindex].pickuptime = timecount;
                    cars[0].senderindex = events[eventindex].OriginCust - 1000;
                    cars[0].receiverindex = events[eventindex].DestinationCust - 1000;

                    if (cars[0].x == customers[cars[0].senderindex].buildingx + 2 && cars[0].y == customers[cars[0].senderindex].buildingy + 2) {
                        cars[0].reachedorigin = 1;
                        cars[0].waittime = (customers[cars[0].senderindex].Floor * 15) + (customers[cars[0].senderindex].Floor * 10);
                        //continue;
                    }
                    else {
                        if (cars[0].reachedorigin == 1) {
                            goto gotodestination;
                        }
                        else {
                        gotodestination2:

                            if (cars[0].x == customers[cars[0].senderindex].buildingx + 2 && cars[0].y == customers[cars[0].senderindex].buildingy + 2) { // new 
                                cars[0].reachedorigin = 1; // new 
                                cars[0].waittime = (customers[cars[0].senderindex].Floor * 15) + (customers[cars[0].senderindex].Floor * 10);
                                clear_car(cars[0]); //new
                                move_cursor(3, 50);
                                printf("car is at %s", customers[cars[0].senderindex].Building);
                            }
                            else {
                                move_cursor(3, 50); //NEW STUFF HEREHHREHRHERHERHREHR
                                printf("              "); // clear the car arrived message
                                move_car_to_destination(&cars[0], customers[cars[0].senderindex].buildingx, customers[cars[0].senderindex].buildingy, map_width, map_height);
                            }
                        }
                    }
                }
            }
        }
        else { // fullfillingorder = 1
            if (cars[0].waittime == 0 && cars[0].reachedorigin == 1) {
            gotodestination:
                if (cars[0].x == customers[cars[0].receiverindex].buildingx + 2 && cars[0].y == customers[cars[0].receiverindex].buildingy + 2) { //yes
                    if (cars[0].recordneeded == 0) {
                        cars[0].recordneeded = 1;
                        cars[0].reacheddestination = 1;
                        clear_car(cars[0]); //new
                        move_cursor(3, 50);
                        printf("car is at %s", customers[cars[0].receiverindex].Building);
                        cars[0].waittime = (customers[cars[0].receiverindex].Floor * 15) + (customers[cars[0].receiverindex].Floor * 10);
                    }
                    else { // recordneeded ==1

                        //making the record
                        events[eventindex].deliverytime = timecount;
                        fprintf(fout, "%d\t%d\t%d\t%d\t%d\n", Deliveryno + 500, events[eventindex].OriginCust, events[eventindex].DestinationCust, events[eventindex].pickuptime, events[eventindex].deliverytime); // make sure to add not overwrite 
                        //resetting the variables
                        move_cursor(3, 50);
                        printf("              "); // clear the car arrived message
                        cars[0].fullfillingorder = 0; // yes=1 No=0
                        cars[0].availability = 1;
                        cars[0].waittime = 0;
                        cars[0].reachedorigin = 0;
                        cars[0].reacheddestination = 0;
                        cars[0].recordneeded = 0;
                        exitloop = 1;
                        eventindex++;
                        Deliveryno++;
                        if (eventindex > MAX_EVENTS) {
                            break;
                        }
                    }
                }
                else {
                    move_cursor(3, 50);
                    printf("              "); // clear the car arrived message
                    move_car_to_destination(&cars[0], customers[cars[0].receiverindex].buildingx, customers[cars[0].receiverindex].buildingy, map_width, map_height);
                }
            }
            else { // waittime!=0 or reachedoring !=1
                if (cars[0].waittime == 0) {
                    goto gotodestination2;
                }
                cars[0].waittime--;
                if (cars[0].batterylevel < 100) {
                    cars[0].batterylevel++;
                }
            }
        }

        //increment time counter
        Sleep(10);
        timecount++;
        move_cursor(1, 50);
        //move_cursor(2, map_width+50);
        printf("time: %d", timecount);

        move_cursor(2, 50);
        printf("                                                                      ");

        move_cursor(2, 50);
        printf("car %d @: %d %d.Battery:%d tmilage:%d waittime:%d", cars[0].x, cars[0].y, cars[0].batterylevel, cars[0].milage, cars[0].waittime);

        //printf("time: %d", timecount);
    }*/

    fclose(fout);
    return;
}