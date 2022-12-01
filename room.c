#include "defs.h"


void initRoom(RoomType* room, char* name){
  strcpy(room->name, name);
  //allocate space and assign a pointer to that space for list of rooms
  RoomListType* roomList = calloc(1,sizeof(RoomListType));
  initRoomList(roomList);
  room->rooms = roomList;

  HunterListType* hunterList =  calloc(1,sizeof(HunterListType));
  initHunterList(hunterList);
  room->hunters = hunterList;

  room->numConnections = 0;
}

void initRoomList(RoomListType* roomList){
  //citation: code taken from MY assignment 4 submission
  roomList->head = NULL;
  roomList->tail = NULL;
}

void connectRooms(RoomType* room, RoomType* roomToConnect){
  //citation: code taken from MY assignment 4 submission addGhost
  RoomListType* list = room->rooms;
  RoomNode* new = calloc(1, sizeof(RoomNode));
  new->data = roomToConnect;
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
  room->numConnections = room->numConnections +1;
}

void addHunterToRoom(HunterType* hunter, RoomType*room){
  //citation: taken from my tutorial 6 code and modified
  HunterNode* cur;
  HunterNode* prev;
  HunterListType* list = room->hunters;
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


void printHunter(HunterListType *list) {
  //citation: taken from my tutorial 6 code and modifie
  HunterNode* cur;
  cur = list->head;
  while(cur!=NULL){
    printf("%s\n", cur->data->name);;
    cur = cur->next;
  }
}



void printRooms(RoomListType *list){
   RoomNode *currNode = list->head;
  //printf("|%-10s|%-10s|%-25s|%-25s\n", "ID","Likelihood","Room","Ghost Type");
  while (currNode != NULL) {
    printf("%s\n", currNode->data->name);
    currNode = currNode->next;
}
}

void cleanupRoomData(RoomListType *list){
  RoomNode* cur = list->head;
  RoomNode* next=NULL;
  //free only data
  while(cur!=NULL){
    RoomNode* curRoomListOfRoom = cur->data->rooms->head;
      while(curRoomListOfRoom!=NULL){ //frees the rooms inside each room
        next = curRoomListOfRoom->next;
        free(curRoomListOfRoom);
        curRoomListOfRoom =next;
      }
    next = cur->next;
    free(cur->data->rooms);
    free(cur->data);
    cur = next;

}
}

void cleanupRoomNodes(RoomListType *list){
  RoomNode* cur = list->head;
  RoomNode* next=NULL;
  //free only nodes
  while(cur!=NULL){
    next = cur->next;
    free(cur);
    cur = next;
  }
}
