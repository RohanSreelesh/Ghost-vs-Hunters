
#include "defs.h"

/*
    Note: While this function is provided, you will likely want
    to modify it to suit your solution. This has been provided
    to give you a starting point to lay out the room connectivity,
    but it is entirely possible that you will want to use different
    data structures to represent the rooms and their connectivity,
    or different function names.

    Not following this function's structure exactly will not result
    in a deduction of points.

    The map that this is recreating is visible in the assignment
    specification, and also available by the original creator here:
    https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947

*/
void populateRooms(BuildingType* building) {
    // First, create each room. Perhaps you want to include more data
    // in the init parameters?
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

    HunterType* hunter = calloc(1,sizeof(HunterType));
    initHunter(hunter, van, "hunter 1", EMF, building->evidenceList);

    HunterType* hunter2 = calloc(1,sizeof(HunterType));
    initHunter(hunter2, van, "hunter 2", TEMPERATURE, building->evidenceList);

    addHunterToBuilding(hunter,building);

    addHunterToRoom(hunter, van);

    addHunterToBuilding(hunter2,building);

    addHunterToRoom(hunter2, van);

    GhostType ghost;
    initGhost(&ghost, van);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    generateEvidence(&ghost);
    printEvidence(ghost.room->evidenceList);

    //printEvidence(hunter->evidenceList);
    collectEvidenceFromRoom(hunter);
    collectEvidenceFromRoom(hunter);
    collectEvidenceFromRoom(hunter);
    collectEvidenceFromRoom(hunter);
    collectEvidenceFromRoom(hunter);
    collectEvidenceFromRoom(hunter);
    //printEvidence(hunter->evidenceList);

    collectEvidenceFromRoom(hunter2);

    shareEvidenceBwHunters(hunter,hunter2);


    printf("%s\n","building evidence" );
    printEvidence(building->evidenceList);
    printf("%s\n","hunter evidence" );
    printEvidence(hunter->evidenceList);
    printf("%s\n","hunter2 evidence" );
    printEvidence(hunter2->evidenceList);
    printf("%s\n","room evidence" );
    printEvidence(hunter->room->evidenceList);

    printf("\n");
    printf("\n");
    moveHunter(hunter);
    moveHunter(hunter);
    moveHunter(hunter);
    moveHunter(hunter);



    // EvidenceType* evidence = calloc(1,sizeof(EvidenceType));
    // evidence->evidenceType =0;
    // evidence->value=1.1;
    // EvidenceNode* node = calloc(1,sizeof(EvidenceNode));
    // node->data=evidence;
    // addEvidenceToHunter(hunter,node);
    // printEvidence(hunter->evidenceList);
}


void appendRoom(RoomListType* allRooms, RoomNode* roomtoAdd){
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

void initBuilding(BuildingType* building){
  HunterListType* huntersList = calloc(1,sizeof(HunterListType));
  RoomListType* roomsList = calloc(1,sizeof(RoomListType));
  EvidenceListType* evidences = calloc(1,sizeof(EvidenceListType));


  initRoomList(roomsList);
  initHunterList(huntersList);
  initEvidenceList(evidences);

  building->rooms = roomsList;
  building->hunters = huntersList;
  building->evidenceList = evidences;
}

void cleanupBuilding(BuildingType* building){
  cleanupRoomData(building->rooms);
  cleanupRoomNodes(building->rooms);

  cleanupEvidenceData(building->evidenceList);
  cleanupEvidenceNodes(building->evidenceList);
  //cleanup hunters
  cleanupHunterData(building->hunters);
  cleanupHunterNodes(building->hunters);
  //cleanupEvidence


  free(building->rooms);
  free(building->hunters);
  free(building->evidenceList);
}

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

void addEvidenceToBuilding(HunterType* hunter, EvidenceNode* evidence){
  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = hunter->buildingEvidenceList;
  EvidenceNode* new;
  new = evidence;

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
