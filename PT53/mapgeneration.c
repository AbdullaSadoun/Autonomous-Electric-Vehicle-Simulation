#include "PT53.h"

void generateMap(int NSno, int EWno, int* map_height, int* map_width) {
    HANDLE Console;
    int buildingx, buildingy;

    *map_height = (NSno * (buildingh + streeth)) + streeth;
    *map_width = (EWno * (buildingw + avenuew)) + avenuew;

    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned int)time(NULL));

    for (int i = 0; i < NSno; ++i) {
        for (int j = 0; j < EWno; ++j) {
            Sleep(15);
            buildingx = originx + avenuew * (j + 1) + buildingw * j;
            buildingy = originy + streeth * (i + 1) + buildingh * i;
            draw_box(buildingy, buildingx, buildingh, buildingw);
            label_building(buildingy, buildingx, 'A' + i, 'A' + j);
        }
    }
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