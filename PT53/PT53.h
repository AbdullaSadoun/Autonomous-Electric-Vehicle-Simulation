#pragma once
#pragma once
#ifndef PT53_h
#define PT53_h

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>	// For Windows-specific graphic data type and functions 
#include <time.h>

/*from bldgGen.h */
#include <string.h>
#include <io.h>
#include <fcntl.h>     //for _O_TEXT and _O_BINARY
#define NUL	0x00
#define ESC	0x1b

// standard sizes and starting positions
#define originx 0
#define originy 0
#define buildingw 3
#define buildingh 3
#define avenuew 1
#define streeth 1
#define MAX_BUILDINGS 100

#define MAX_CUSTOMERS 100
#define MAX_EVENTS 100

// Box character codes (decimal), See https://www.lookuptables.com/text/extended-ascii-table
#define UL		218
#define LL		192
#define UR		191
#define LR		217
#define HBAR	196
#define VBAR	179

// DISP output character 
#define	DISP(x)	putc((x),stdout)

HANDLE Console;		// Identify action and I/O stream 

// my classes and methods //
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char symbol;
    int x; // later make this a type point called current 
    int y; // later make this a type point called current
    //Point current;
    Point destination;
    int batterylevel;
    int tempstate;
    int availability;

} Car;

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    int CustNum;
    char FirstName[11];
    char LastName[11];
    char Building[3];
    char Entrance[3];
    int Floor;
    int locationx;
    int locationy;
} Customer;

typedef struct {
    int time;
    char type[3];
    int OriginCust;
    int DestinationCust;
    int Weight;
    int pickuptime;
    int deliverytime;
} Event;

Coordinate building_coords[MAX_BUILDINGS * 9];
//int total_building_coords = 0;


struct bldg_data
{
    int x;
    int y;

    enum BLDG_TYPE bt;
    enum QUAD qd;
};

// List of valid responses and codes
struct prompt
{
    char* name;
    int code;
};

#define TRUNCATE(name)	name[strlen(name)-1] = '\0' 
#define NAMELEN	16	//Filename length

//Internal codes
enum ST_DIR { East, West };
enum AVE_DIR { North, South };
enum BLDG_TYPE { CHG, STB, BOTH };
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

//struct prompt bldg_t[] = { {"Charge",CHG}, {"Stable",STB}, {"Both", BOTH} };
///*struct prompt ew[] = {{"East", East}, {"West", West}};
//struct prompt ns[] = { {"North", North}, {"South", South} };*/
//struct prompt bldg_q[] = {
//    {"NE",NE}, {"N",N}, {"NW",NW},
//    {"E",E}, {"LBL",-1}, {"W",W},
//    {"SE",SE}, {"S",S}, {"SW",SW} };
// 
//Building file descriptor
FILE* bfd;

// Dr. Hughes' functions used: //
void read_file(unsigned int* EWno, unsigned int* NSno);
void move_cursor(int row, int col);
void draw_box(int ulr, int ulc, int height, int width);

// my functions //
void label_building(int y, int x, char label1, char label2);
void add_building_to_coords(int x, int y); // this function adds the coordinate of the generated building to array building_coords
int is_building_at(int x, int y); // this function returns 1 if coordinates are found in a building and 0 otherwise
//void move_car_to_destination(Car* car, int dest_x, int dest_y, int map_width, int map_height); // "attempts" moves car to destination
void move_car_to_destination(Car* car, int dest_x, int dest_y, int map_width, int map_height);
void draw_car(Car car); // draws the car on its current coordinates on the map
void clear_car(Car car); // clears the car on its current coordinates on the map
void run_simulation(int NSno, int EWno, Customer customers[], Event events[]); // uses all the functions above to run the simulation of one car

void readprcoessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]); // Reads and processes the customer file
//int readprocessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]);
//void readprocessEventF(char* filename) // Reads and processes the events file

void readprocessEventF(char* filename, Event events[MAX_EVENTS]);



void generateMap(int NSno, int EWno, int* map_height, int* map_width);






#endif //PT53