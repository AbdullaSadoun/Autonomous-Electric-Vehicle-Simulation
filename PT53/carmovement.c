#include "Simulation.h"

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
    - it does that by doing the same method above my comparing coordinates but this time to destination coordinates
    Updates:
    - the function now decrements the car battery every time its used.
    -
    */

    if (car->tempstateset == 0) {
        car->temp_x = dest_x + 2;
        car->temp_y = dest_y + 2;
        car->tempstateset = 1;
    }

    clear_car(*car);
    
    if (car->x < car->temp_x && car->y%4==0) { // moving in x-axis
        car->x++;
    }
    else if (car->x > car->temp_x && car->y % 4 == 0) {
        car->x--;
    }
    else if (car->y < car->temp_y && car->x % 4 == 0) {
        car->y++;
    }
    else if (car->y > car->temp_y && car->x % 4 == 0) {
        car->y--;
    }
    else{
        clear_car(*car);
        //movecursor(3, 50);
        //printf("car is at the destination");
        car->tempstate = 0;
        car->tempstateset = 0;
        Sleep(10);
        return;
    }
    
    car->TotalTimeMoving++;
    car->milage++;
    car->batterylevel = car->batterylevel - 1;
    draw_car(*car);

    Sleep(10);
    return;
}