
#include "defs.h"

/*
  Function:  initializeEverything
  Purpose:   Intialize BuildingType*  and also creates the threads and everything needed for the program to run
       in:   Takes an pointer of the type BuildingType
   return:   nothing
*/
void initializeEverything(BuildingType* building) {
    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    // Now create a linked list of rooms using RoomNode in the Building
    RoomNode* van_node = calloc(1, sizeof(RoomNode));
    van_node->data = van;
    RoomNode* hallway_node = calloc(1, sizeof(RoomNode));
    hallway_node->data = hallway;
    RoomNode* master_bedroom_node = calloc(1, sizeof(RoomNode));
    master_bedroom_node->data = master_bedroom;
    RoomNode* boys_bedroom_node = calloc(1, sizeof(RoomNode));
    boys_bedroom_node->data = boys_bedroom;
    RoomNode* bathroom_node = calloc(1, sizeof(RoomNode));
    bathroom_node->data = bathroom;
    RoomNode* basement_node = calloc(1, sizeof(RoomNode));
    basement_node->data = basement;
    RoomNode* basement_hallway_node = calloc(1, sizeof(RoomNode));
    basement_hallway_node->data = basement_hallway;
    RoomNode* right_storage_room_node = calloc(1, sizeof(RoomNode));
    right_storage_room_node->data = right_storage_room;
    RoomNode* left_storage_room_node = calloc(1, sizeof(RoomNode));
    left_storage_room_node->data = left_storage_room;
    RoomNode* kitchen_node = calloc(1, sizeof(RoomNode));
    kitchen_node->data = kitchen;
    RoomNode* living_room_node = calloc(1, sizeof(RoomNode));
    living_room_node->data = living_room;
    RoomNode* garage_node = calloc(1, sizeof(RoomNode));
    garage_node->data = garage;
    RoomNode* utility_room_node = calloc(1, sizeof(RoomNode));
    utility_room_node->data = utility_room;

    // Building->rooms might be a linked list structre, or maybe just a node.
    initRoomList(building->rooms);

    appendRoom(building->rooms, van_node);
    appendRoom(building->rooms, hallway_node);
    appendRoom(building->rooms, master_bedroom_node);
    appendRoom(building->rooms, boys_bedroom_node);
    appendRoom(building->rooms, bathroom_node);
    appendRoom(building->rooms, basement_node);
    appendRoom(building->rooms, basement_hallway_node);
    appendRoom(building->rooms, right_storage_room_node);
    appendRoom(building->rooms, left_storage_room_node);
    appendRoom(building->rooms, kitchen_node);
    appendRoom(building->rooms, living_room_node);
    appendRoom(building->rooms, garage_node);
    appendRoom(building->rooms, utility_room_node);

    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    //testing
    char name1[MAX_STR];
    char name2[MAX_STR];
    char name3[MAX_STR];
    char name4[MAX_STR];
    printf("%s\n","Enter the name for the 1st hunter:" );
    scanf("%s", name1);

    printf("%s\n","Enter the name for the 2nd hunter:" );
    scanf("%s", name2);

    printf("%s\n","Enter the name for the 3rd hunter:" );
    scanf("%s", name3);

    printf("%s\n","Enter the name for the 4th hunter:" );
    scanf("%s", name4);

    HunterType* hunter = calloc(1,sizeof(HunterType));
    initHunter(hunter, van, name1, EMF);
    //
    HunterType* hunter2 = calloc(1,sizeof(HunterType));
    initHunter(hunter2, van, name2, TEMPERATURE);
    //
    HunterType* hunter3 = calloc(1,sizeof(HunterType));
    initHunter(hunter3, van, name3, FINGERPRINTS);

    HunterType* hunter4 = calloc(1,sizeof(HunterType));
    initHunter(hunter4, van, name4, SOUND);
    //
    addHunterToBuilding(hunter,building);
    addHunterToRoom(hunter, van);
    //
    addHunterToBuilding(hunter2,building);
    addHunterToRoom(hunter2, van);

    addHunterToBuilding(hunter3,building);
    addHunterToRoom(hunter3, van);

    addHunterToBuilding(hunter4,building);
    addHunterToRoom(hunter4, van);

    //ghost while loop
    pthread_t t1,t2,t3,t4,t5;
    GhostType ghost;
    initGhost(&ghost, left_storage_room);

    pthread_create(&t1, NULL, mainGhostFunction, &ghost);

     pthread_create(&t2, NULL, mainHunterFunction, hunter);
     pthread_create(&t3, NULL, mainHunterFunction, hunter2);
    pthread_create(&t4, NULL, mainHunterFunction, hunter3);
    pthread_create(&t5, NULL, mainHunterFunction, hunter4);

    pthread_join(t1, NULL);
     pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("%s\n","****RESULTS****");
    printf("%s fear: %d\n",hunter->name,hunter->fearValue);
    printf("%s fear: %d\n",hunter2->name,hunter2->fearValue);
    printf("%s fear: %d\n",hunter3->name,hunter3->fearValue);
    printf("%s fear: %d\n",hunter4->name,hunter4->fearValue);

    int lost =1;
    if(hunter->fearValue<100){
      if(guessGhost(hunter)==-1){
        printf("\n");
        printf("While the hunter(%s) was not feared, they could not collect enough evidence to identify the ghost \n",hunter->name);
      }
      else{
        printf("\n");
        printf("Guess ghost by %s: ",hunter->name);
        convertGhostEnumToString(guessGhost(hunter));

        lost=0;
      }

    }
    if(hunter2->fearValue<100){
      if(guessGhost(hunter2)==-1){
        printf("\n");
        printf("While the hunter(%s) was not feared, they could not collect enough evidence to identify the ghost\n",hunter2->name);
      }
      else{
        printf("\n");
      printf("Guess ghost by %s: ",hunter2->name);
      convertGhostEnumToString(guessGhost(hunter2));
      lost=0;
    }}
    if(hunter3->fearValue<100){
      if(guessGhost(hunter3)==-1){
        printf("\n");
        printf("While the hunter(%s) was not feared, they could not collect enough evidence to identify the ghost\n",hunter3->name);
      }
      else{

      printf("\n");
      printf("Guess ghost by %s: ",hunter3->name);
      convertGhostEnumToString(guessGhost(hunter3));
      lost=0;
    }
    }
    if(hunter4->fearValue<100){
      if(guessGhost(hunter4)==-1){
        printf("\n");
        printf("While the hunter(%s) was not feared, they could not collect enough evidence to identify the ghost \n",hunter4->name);
      }
      else{

      printf("\n");
      printf("Guess ghost by %s: ",hunter4->name);
      convertGhostEnumToString(guessGhost(hunter4));
      lost=0;
      }
    }
    if(lost){
      printf("\nGHOST TYPE WAS:");
      convertGhostEnumToString(ghost.ghostType);
      printf("\n");
      printf("%s\n","HUNTERS LOST :(" );
    }else{
      printf("\nGHOST TYPE WAS:");
      convertGhostEnumToString(ghost.ghostType);
      printf("\n");
      printf("%s\n","HUNTERS WON WOOOOO!!!" );
    }

}

/*
  Function:  appendRoom
  Purpose:   add a room as an exit
       in:   Takes an pointer of the type RoomListType*
       in:   Takes of pointer of the type RoomNode*
   return:   nothing
*/
void appendRoom(RoomListType* allRooms, RoomNode* roomtoAdd){
  //code taken from assignment 4 submission
  RoomListType* list = allRooms;
  //RoomNode* new = calloc(1, sizeof(RoomNode));
  RoomNode* new;
  new = roomtoAdd;
  //first add
  if(list->head==NULL){
  list->head = new;
  } //second add
  else if(list->tail==NULL){
  list->tail = new;
  list->head->next = list->tail;
  }//every add after the ones above, add at end once we have tail established
  else{
  list->tail->next = new;
  list->tail = list->tail->next;
  }
}
/*
  Function:  initBuilding
  Purpose:   initalizes building
       in:   Takes an pointer of the type BuildingType*
   return:   nothing
*/
void initBuilding(BuildingType* building){
  HunterListType* huntersList = calloc(1,sizeof(HunterListType));
  RoomListType* roomsList = calloc(1,sizeof(RoomListType));


  initRoomList(roomsList);
  initHunterList(huntersList);

  building->rooms = roomsList;
  building->hunters = huntersList;
}

/*
  Function:  cleanupBuilding
  Purpose:   Frees all the memory in a building
       in:   Takes an pointer of the type BuildingType*
   return:   nothing
*/
void cleanupBuilding(BuildingType* building){
  cleanupRoomData(building->rooms);
  cleanupRoomNodes(building->rooms);

  //cleanupEvidenceData(building->evidenceList);
  //cleanupEvidenceNodes(building->evidenceList);
  //cleanup hunters
  cleanupHunterData(building->hunters);
  cleanupHunterNodes(building->hunters);
  //cleanupEvidence


  free(building->rooms);
  free(building->hunters);
  //free(building->evidenceList);
}

/*
  Function:  addHunterToBuilding
  Purpose:   Adds hunter to a given building
       in:   Takes an pointer of the type HunterType*
       in:    Takes an pointer of the type BuildingType*
   return:   nothing
*/
void addHunterToBuilding(HunterType* hunter, BuildingType* building){
  //citation: taken from my tutorial 6 code and modified
  HunterNode* cur =NULL;
  HunterNode* prev =NULL;
  HunterListType* list = building->hunters;
  HunterNode* new = calloc(1,sizeof(HunterNode));
  new->data = hunter;

  cur = list->head;

  if(cur==NULL){
    list->head = new;
    list->head->next = cur;
    return;}

  else{

  while(cur!=NULL){

    prev = cur;
    cur = cur->next;
  }
  prev->next = new;
  new->next = cur;
  }
}
