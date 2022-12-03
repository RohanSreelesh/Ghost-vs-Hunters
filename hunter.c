#include "defs.h"


void initHunter(HunterType* hunter, RoomType* room, char* name, EvidenceClassType type,EvidenceListType* sharedEvidenceList){
  strcpy(hunter->name, name);
  hunter->room = room;
  hunter->boredomValue = BOREDOM_MAX;
  hunter->fearValue = 0;
  hunter->evidenceType = type;
  EvidenceListType* evidences = calloc(1,sizeof(EvidenceListType));
  initEvidenceList(evidences);
  hunter->evidenceList = evidences;
  hunter->buildingEvidenceList = sharedEvidenceList;
}

void initHunterList(HunterListType* hunterList){
  hunterList->head =NULL;
}

//delete hunter
//evidence data should be freed from building evidence list


void collectEvidenceFromRoom(HunterType* hunter){
  EvidenceNode* evidenceToRemove;


   EvidenceNode* cur;
   EvidenceNode* prev;
   EvidenceListType* evidences =  hunter->room->evidenceList;

   cur = hunter->room->evidenceList->head;

   int found= 0;
   while(cur!=NULL){
     //if evidence type matches the type the hunter can read
     if(cur->data->evidenceType==hunter->evidenceType){
       printf("%s has obtained evidence with type :", hunter->name);
       convertEvidenceEnumToString(cur->data->evidenceType);
       found=1;
       break;
     }
     prev = cur;
     cur = cur->next;
   }
   if(found==1){
     printf("%d", cur->data->evidenceType);
     printf("%f\n", cur->data->value);
   }
  if(found==0){
    return;
  }
  //printf("%s\n","room evidence before" );
  //printEvidence(hunter->room->evidenceList);
  //duplicate evidence to add to hunter
  if(cur!=NULL){
  EvidenceType* evidence = calloc(1,sizeof(EvidenceType));
  evidence->evidenceType =cur->data->evidenceType;
  evidence->value=cur->data->value;
  EvidenceNode* node = calloc(1,sizeof(EvidenceNode));
  node->data=evidence;

  addEvidenceToHunter(hunter,node);

 if(cur==evidences->head){
   //printf("1\n");
   //new->next = stuList->head;
   //free(evidences->head->data);
   //free(stuList->head);
   //printf("%s\n","head" );
   //printf("%f\n", evidences->head->data->value);
   EvidenceNode* tofree = evidences->head;
   evidences->head = evidences->head->next;
   free(tofree->data);
   free(tofree);
   //stuList->head = new;

 }
 else if (prev!=NULL){
   //printf("2\n");
   //printf("%s\n", prev->data->basicInfo.last);
   //free(prev->next);
   EvidenceNode* tofree =prev->next;
   //free(prev->next->data);
   prev->next =prev->next->next;
   free(tofree->data);
   free(tofree);

 }


}

}


  //add to hunter

void addEvidenceToHunter(HunterType* hunter, EvidenceNode* evidence){
  //citation: taken from my tutorial 6 code and modified

  //whenevr we add to hunter list, we also add to the buildings RoomListType
  //addEvidenceToBuilding(hunter,evidence);
  //Add only if ghostly evidence, make a check

  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = hunter->evidenceList;
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




void shareEvidenceBwHunters(HunterType* hunter1, HunterType* hunter2){
  //if evidence is ghost, add it to the other hunters collection
  EvidenceListType* list = hunter1->evidenceList;
  EvidenceNode* cur;
  EvidenceNode* prev;

  cur = list->head;

  if(cur==NULL){
    return;}

    while(cur!=NULL){
      int ghostly = checkIfGhostly(cur);
      if(ghostly){
        //add to other hunters list but first duplicate the evidence node
        EvidenceType* evidence = calloc(1,sizeof(EvidenceType));
        evidence->evidenceType =cur->data->evidenceType;
        evidence->value=cur->data->value;
        EvidenceNode* node = calloc(1,sizeof(EvidenceNode));
        node->data=evidence;
        //add print
        printf("%s has shared", hunter1->name);
        convertEvidenceEnumToString(evidence->evidenceType);
        printf(" evidence with %s\n",hunter2->name);
        addEvidenceToHunter(hunter2,node);
      }

      prev = cur;
      cur = cur->next;
    }
}

void convertEvidenceEnumToString(EvidenceClassType evidenceType){
  if(evidenceType==0){
    printf(" %s ","EMF" );
  }
  else if(evidenceType==1){
    printf(" %s ","TEMPERATURE");
  }
  else if(evidenceType==2){
    printf(" %s ","FINGERPRINTS");
  }
  else if(evidenceType==3){
    printf(" %s ","SOUND");
  }
}

//moves the hunter to one of the connected rooms from the current room
void moveHunter(HunterType* hunter){
  int choices = hunter->room->numConnections;
  int count=1;
  int randChoice = randInt(1,choices+1);
  printf("rand:%d\n",randChoice );
  RoomListType* list =hunter->room->rooms;
  RoomNode* cur;
  RoomNode* prev;
  cur = list->head;

  if(cur==NULL){
    printf("Something went wrong\n");
    return;}

  else{

  while(cur!=NULL){
    //printf("name of room: %s\n", cur->data->name);
    if(count==randChoice){
    //  printf("%s\n","choices" );
    //printRooms(cur->data->rooms);
      //remove from current room and add to the room we randomly chose
      removeHunterFromRoom(hunter,hunter->room);
      addHunterToRoom(hunter,cur->data);

      hunter->room = cur->data;
      break;
    }
    prev = cur;
    cur = cur->next;
    count++;
  }
  }

  //remove hunter from current room as well

}

void cleanupHunterData(HunterListType *list){
  //citation: code taken from MY assignment 4 submission
  HunterNode* cur = list->head;
  HunterNode* next=NULL;
  //EvidenceNode* nextEvidence =NULL;
  //free only nodes
  while(cur!=NULL){
    //make it a functions
    cleanupEvidenceData(cur->data->evidenceList);
    cleanupEvidenceNodes(cur->data->evidenceList);

    next = cur->next;
    free(cur->data->evidenceList);
    free(cur->data);
    cur = next;
  }
}




void cleanupHunterNodes(HunterListType *list){
  //citation: code taken from MY assignment 4 submission
  HunterNode* cur = list->head;
  HunterNode* next=NULL;
  //free only nodes
  while(cur!=NULL){
    //cleanupEvidenceNodes(cur->data->evidenceList);
    next = cur->next;
    //free(cur->data->evidenceList);
    free(cur);

    cur = next;
  }
}
