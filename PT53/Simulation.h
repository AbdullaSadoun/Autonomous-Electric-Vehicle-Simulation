#pragma once
#pragma once
#ifndef Simulation_h
#define Simulation_h

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
#define MAX_CARS 4
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
    int x;  
    int y; 
    int chosen;
    Point destination;
    int tempstate;
    int availability;
    int temp_x;
    int temp_y;
    int tempstateset;
    
    int VIN;
    char laststable[3];

    int waittime; // = time to wait

    int milage; // = total time moving
    int fullfillingorder;
    int reachedorigin; // = 1 if reached origin
    int reacheddestination; // = 1 if reached destination
    int recordneeded; // = 1 if record needed
    int senderindex; // = customer index
    int receiverindex; // = customer index
    int Deliveryno; // = delivery number
    int assignedeventindex; // = event index

    int batterylevel; // = current battery level
    int maxbatterylevel; // must be preset
    int batteryrechargerate; // must be preset
    int totaltimecharging; //increment as charging
    int drivingdischarge; // must be preset
    int totalwaittime; //increment as waiting
    int IdlingDischargeRate; // must be preset
    int TotalTimeMoving; //increment as moving
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
    int buildingx;
    int buildingy;
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

FILE* bfd;

// my functions //

/* fileporcessing.c prototypes */
void read_file(unsigned int* EWno, unsigned int* NSno); // Reads the binary input file used to generate the map
void readprcoessCustomerF(char* filename, Customer customers[MAX_CUSTOMERS]); // Reads and processes the customer file
void readprocessEventF(char* filename, Event events[MAX_EVENTS]); // Function that reads and processes Event File

/* mapgeneration.c prototypes */
void generateMap(int NSno, int EWno, int* map_height, int* map_width); // Function that generates the Map with the buildings in it
void draw_box(int ulr, int ulc, int height, int width); // draws a box which will later be the buildings 
void label_building(int y, int x, char label1, char label2); // labels the buildings AA, AB, AC ...etc.
void add_building_to_coords(int x, int y); // this function adds the coordinate of the generated building to array building_coords
int is_building_at(int x, int y); // this function returns 1 if coordinates are found in a building and 0 otherwise

/* carmovement.c prototypes */
void move_car_to_destination(Car* car, int dest_x, int dest_y, int map_width, int map_height); // "attempts" moves car to destination
void draw_car(Car car); // draws the car on its current coordinates on the map
void clear_car(Car car); // clears the car on its current coordinates on the map


/* Simulation.c Prototypes */
void move_cursor(int row, int col); // Function used to navigate through the graphical interface
void run_simulation(int NSno, int EWno, Customer customers[], Event events[], FILE *Delivery, FILE *Vehicle); // uses all the functions above to run the simulation of one car

#endif //Simulation