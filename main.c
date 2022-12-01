#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // You may change this code; this is for demonstration purposes
     BuildingType building;
     initBuilding(&building);
    // populateRooms(&building);
    // //printRooms(building.rooms);
    // cleanupBuilding(&building);


    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");

    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");

    connectRooms(van, hallway);

    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");

    connectRooms(van, master_bedroom);
    printRooms(van->rooms);

    HunterType hunter;
    HunterType hunter2;
    HunterType hunter3;
    HunterType hunter4;
    initHunter(&hunter, van, "hunter 1", EMF, building.evidenceList);
    initHunter(&hunter2, van, "hunter 2", EMF, building.evidenceList);
    initHunter(&hunter3, van, "hunter 3", EMF, building.evidenceList);
    initHunter(&hunter4, van, "hunter 4", EMF, building.evidenceList);

    GhostType ghost;
    initGhost(&ghost, van);

    printf("%d\n",checkHunter(&ghost));

    addHunterToRoom(&hunter, van);
    addHunterToRoom(&hunter2, van);
    addHunterToRoom(&hunter3, van);
    addHunterToRoom(&hunter4, van);

    printHunter(van->hunters);
    //printf("%d\n",checkHunter(&ghost));

    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
