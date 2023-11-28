#include "PT53.h"

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
    - after the car gets to the closest interection it changes the destination to be the actual destination and then makes its way to the building
    - it does that by doing the same method above my comparing coordinates but this time to destination coordinates
    Updates: 
    - the function now decrements the car battery every time its used. 
    - 
    */

    int temp_x = (dest_x / 4) * 4;
    int temp_y = (dest_y / 4) * 4;

    Sleep(50);
    clear_car(*car);

    if (car->tempstate == 0) {
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
    }
    else {
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
        //printf(". Destination reached!, car is at: %d %d\n", dest_x, dest_y);
        Sleep(3000);
        car->tempstate = 0;
        return; // exit the function since destination is reached
    }

    return;

}
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