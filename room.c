#include "defs.h"



/*
  Function:  initRoom
  Purpose:   Initalize a room
       in:   Takes a pointer of RoomType*
       in:    Takes in the name of the room as char* pointer
   return:   nothing
*/
void initRoom(RoomType* room, char* name){
  strcpy(room->name, name);
  //allocate space and assign a pointer to that space for list of rooms
  RoomListType* roomList = calloc(1,sizeof(RoomListType));
  initRoomList(roomList);
  room->rooms = roomList;

  HunterListType* hunterList =  calloc(1,sizeof(HunterListType));
  initHunterList(hunterList);
  room->hunters = hunterList;

  EvidenceListType* evidenceList =  calloc(1,sizeof(EvidenceListType));
  initEvidenceList(evidenceList);
  room->evidenceList = evidenceList;

  room->numConnections = 0;

  sem_init(&(room->mutex),0 ,1);

}

/*
  Function:  initRoomList
  Purpose:   Initalize room list
       in:   Takes an pointer of the type RoomListType*
   return:   nothing
*/
void initRoomList(RoomListType* roomList){
  //citation: code taken from MY assignment 4 submission
  roomList->head = NULL;
  roomList->tail = NULL;
}

/*
  Function:  connectRooms
  Purpose:   Connects 2 rooms
       in:   Takes an pointer of the type RoomType*
       in:   Takes an pointer of the type RoomType*
   return:   nothing
*/
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


  //connect the other room to room 1
  list = roomToConnect->rooms;
  new = calloc(1, sizeof(RoomNode));
  new->data = room;
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
  roomToConnect->numConnections = roomToConnect->numConnections +1;
}


/*
  Function:  addHunterToRoom
  Purpose:   Adds a hunter to a given room
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type RoomType*
   return:   nothing
*/
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
    printf("%s has moved to %s\n",hunter->name,room->name);
    return;}

  else{

  while(cur!=NULL){

    prev = cur;
    cur = cur->next;
  }
  prev->next = new;
  new->next = cur;
  }
  printf("%s has moved to %s\n",hunter->name,room->name);
}

/*
  Function:  removeHunterFromRoom
  Purpose:   Removes a hunter to a given room
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type RoomType*
   return:   nothing
*/
void removeHunterFromRoom(HunterType* hunter, RoomType*room){
    //citation: taken from my tutorial 6 code and modified
    printf("\n");
    HunterNode* cur;
    HunterNode* prev;
    HunterListType* list = room->hunters;
    cur = list->head;
    int found= 0;

     while(cur!=NULL){
       //if evidence type matches the type the hunter can read it
       if(hunter->name==cur->data->name){
         found=1;
         break;
       }
       prev = cur;
       cur = cur->next;
     }
     if(found==1){

     }
    if(found==0){
      return;
    }

    if(cur==list->head){
      HunterNode* tofree = list->head;
      list->head = list->head->next;
      free(tofree);
    }
    else if (prev!=NULL){

      HunterNode* tofree =prev->next;

      prev->next =prev->next->next;
      free(tofree);

    }

}

/*
  Function:  removeHunterFromRoom
  Purpose:   Removes a hunter to a given room and locks the given room, its called to remove a hunter when its done with everything, see mainHunterFunction
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type RoomType*
   return:   nothing
*/
void removeHunterFromRoomFinal(HunterType* hunter, RoomType*room){
    //citation: taken from my tutorial 6 code and modified
    // printf("name of the room: %s\n",room->name);
    // printHunter(room->hunters);
    sem_wait(&(hunter->room->mutex));
    printf("\n");
    HunterNode* cur;
    HunterNode* prev;
    HunterListType* list = room->hunters;
    //HunterNode* new = calloc(1,sizeof(HunterNode));


     cur = list->head;

     int found= 0;
     while(cur!=NULL){
       //if evidence type matches the type the hunter can read
       if(hunter->name==cur->data->name){
         found=1;
         break;
       }
       prev = cur;
       cur = cur->next;
     }
     if(found==1){

     }
    if(found==0){
      sem_post(&(hunter->room->mutex));
      return;
    }

    if(cur==list->head){
      HunterNode* tofree = list->head;
      list->head = list->head->next;
      free(tofree);
    }
    else if (prev!=NULL){

      HunterNode* tofree =prev->next;

      prev->next =prev->next->next;
      free(tofree);

    }
    sem_post(&(hunter->room->mutex));
  }



  /*
    Function:  cleanupRoomData
    Purpose:   Cleanup room node data given an RoomListType* pointer
         in:   Takes a pointer of RoomListType*
     return:   nothing
  */
void cleanupRoomData(RoomListType *list){
  RoomNode* cur = list->head;
  RoomNode* next=NULL;
  //free only data


  while(cur!=NULL){
    cleanupHunterNodes(cur->data->hunters);
    //clean evidence if not taken by any hunters
    cleanupEvidenceData(cur->data->evidenceList);
    cleanupEvidenceNodes(cur->data->evidenceList);

    free(cur->data->evidenceList);
    free(cur->data->hunters);
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


/*
  Function:  cleanupRoomNodes
  Purpose:   Cleanup room nodes given an RoomListType* pointer
       in:   Takes a pointer of RoomListType*
   return:   nothing
*/
void cleanupRoomNodes(RoomListType *list){
  //citation: code taken from MY assignment 4 submission
  RoomNode* cur = list->head;
  RoomNode* next=NULL;
  //free only nodes
  while(cur!=NULL){
    next = cur->next;
    free(cur);
    cur = next;
  }
}
