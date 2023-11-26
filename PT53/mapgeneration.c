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