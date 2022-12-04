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
  sem_init(&(hunter->mutex),0 ,1);
}

void initHunterList(HunterListType* hunterList){
  hunterList->head =NULL;
}


void* mainHunterFunction(void* arg){
  HunterType* hunter = (HunterType*) arg;

  while(checkThreeGhostlyEvidence(hunter)!=1 && hunter->fearValue<100 && hunter->boredomValue>0){
    //while(i<5){
    increaseHunterFear(hunter); //checks for ghost and adds accordingly
    int randChoice;
    randChoice = randInt(0,3);
    //printf("rand: %d\n",randChoice);
    if(randChoice==0){
      collectEvidenceFromRoom(hunter);
    }
    else if(randChoice==1){
      moveHunter(hunter);
      decreaseHunterBoredom(hunter);

    }
    else if(randChoice==2){

      checkIfAnotherHunterInRoom(hunter);

    }
    else{
      printf("SOMETHING WENT WRONG\n");
    }
    //i++;
  }
  removeHunterFromRoomFinal(hunter,hunter->room);
  printf("hunter left the building\n");
}


//delete hunter
//evidence data should be freed from building evidence list


void collectEvidenceFromRoom(HunterType* hunter){
  //sem_wait(&(hunter->mutex));
  sem_wait(&(hunter->room->mutex));
  EvidenceNode* evidenceToRemove;


   EvidenceNode* cur;
   EvidenceNode* prev;
   EvidenceListType* evidences =  hunter->room->evidenceList;

   cur = hunter->room->evidenceList->head;

   int found= 0;
   while(cur!=NULL){
     //if evidence type matches the type the hunter can read
     if(cur->data->evidenceType==hunter->evidenceType){
       // printf("%s has obtained evidence with type :", hunter->name);
       // convertEvidenceEnumToString(cur->data->evidenceType);
       // printf("\n");
       found=1;
       break;
     }
     prev = cur;
     cur = cur->next;
   }
   // if(found==1){
   //   printf("%d", cur->data->evidenceType);
   //   printf("%f\n", cur->data->value);
   // }
  if(found==0){
    printf("%s did not find any evidence that it can read, it will read random standard evidence\n",hunter->name);
    genrateStandardEvidence(hunter);
    sem_post(&(hunter->room->mutex));
    //sem_post(&(hunter->mutex));
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
 sem_post(&(hunter->room->mutex));
 //sem_post(&(hunter->mutex));

}
}



  //add to hunter

void addEvidenceToHunter(HunterType* hunter, EvidenceNode* evidence){
  //citation: taken from my tutorial 6 code and modified

  //whenevr we add to hunter list, we also add to the buildings RoomListType
  //addEvidenceToBuilding(hunter,evidence);
  //Add only if ghostly evidence, make a check
  sem_wait(&(hunter->mutex));

  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = hunter->evidenceList;
  EvidenceNode* new;
  new = evidence;

  cur = list->head;

  if(cur==NULL){
    list->head = new;
    list->head->next = cur;
    if(checkIfGhostly(new)){
      printf("%s has obtained GHOSTLY evidence with type :", hunter->name);
      convertEvidenceEnumToString(new->data->evidenceType);
      printf("\n");
      hunter->boredomValue = BOREDOM_MAX;
      //since its ghostly we also update the hunter boredom
    }
    else{
      printf("%s has obtained STANDARD evidence with type :", hunter->name);
      convertEvidenceEnumToString(new->data->evidenceType);
      printf("\n");
    }
    sem_post(&(hunter->mutex));
    return;}

  else{

  while(cur!=NULL){

    prev = cur;
    cur = cur->next;
  }
  prev->next = new;
  new->next = cur;
  if(checkIfGhostly(new)){
    printf("%s has obtained GHOSTLY evidence with type :", hunter->name);
    convertEvidenceEnumToString(new->data->evidenceType);
    printf("\n");
    hunter->boredomValue = BOREDOM_MAX;
    //since its ghostly we also update the hunter boredom
  }
  else{
    printf("%s has obtained STANDARD evidence with type :", hunter->name);
    convertEvidenceEnumToString(new->data->evidenceType);
    printf("\n");
  }
  sem_post(&(hunter->mutex));
}
}


HunterType* checkIfAnotherHunterInRoom(HunterType* hunter){
  sem_wait(&(hunter->room->mutex));
  HunterListType* list =hunter->room->hunters;
  HunterNode* cur;
  HunterNode* prev;
  cur = list->head;

  if(cur==NULL){
    printf("Something went wrong\n");
    sem_post(&(hunter->room->mutex));
    return NULL;}

  else{

  while(cur!=NULL){
    //printf("name of room: %s\n", cur->data->name);
    if(hunter->name!=cur->data->name){
      printf("%s found another hunter(%s) will attempt to share evidence\n", hunter->name,cur->data->name);
      //if (sem_trywait(&(cur->data->room->mutex)) == 0) {
      if(cur->data!=NULL){
        shareEvidenceBwHunters(hunter,cur->data);
      }
        //sem_post(&(cur->data->room->mutex));
      //}


      sem_post(&(hunter->room->mutex));
      return NULL;
      break;
    }
    prev = cur;
    cur = cur->next;
  }
  printf("%s did not find another hunter to share evidence with\n", hunter->name);
  sem_post(&(hunter->room->mutex));
  return NULL;
  }

}

void shareEvidenceBwHunters(HunterType* hunter1, HunterType* hunter2){
  //if evidence is ghost, add it to the other hunters collection
  //sem_wait(&(hunter2->mutex));
  //sem_wait(&(hunter1->room->mutex));
  EvidenceListType* list = hunter1->evidenceList;
  EvidenceNode* cur;
  EvidenceNode* prev;

  cur = list->head;

  if(cur==NULL){
    //sem_post(&(hunter2->mutex));
    //sem_post(&(hunter1->room->mutex));
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
        printf("%s has shared GHOSTLY", hunter1->name);
        convertEvidenceEnumToString(evidence->evidenceType);
        printf(" evidence with %s\n",hunter2->name);
        addEvidenceToHunter(hunter2,node);
        break;
      }
      prev = cur;
      cur = cur->next;
    }
    //
    //sem_post(&(hunter1->room->mutex));
    //sem_post(&(hunter2->mutex));
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
  int sem =0;
  sem_t* temp = &(hunter->room->mutex);
  if (sem_trywait(&(hunter->room->mutex)) == 0) {
    sem=1;
  }
  else{
    //printf("%s Could not move due to sem\n",hunter->name);
    return;
  }


  int choices = hunter->room->numConnections;
  int count=1;
  int randChoice = randInt(1,choices+1);
  //printf("rand:%d\n",randChoice );
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
      if (sem_trywait(&(cur->data->mutex)) == 0) {
        removeHunterFromRoom(hunter,hunter->room);
        addHunterToRoom(hunter,cur->data);
        hunter->room = cur->data;
        //sem_post(&(hunter->room->mutex));
        sem_post(&(cur->data->mutex));
        break;
      }
      else{
        //printf("%s Could not move due to sem\n",hunter->name);
        //sem_post(&(hunter->room->mutex));
        break;
      }

      //not modifying room here just updating pointers

    }
    prev = cur;
    cur = cur->next;
    count++;
  }
  //because hunters room has now changed
  sem_post((temp));

  }


}

void genrateStandardEvidence(HunterType* hunter){
  int hunterType = hunter->evidenceType;
  EvidenceClassType evidenceType;
  float value;

  if(hunterType==0){
    evidenceType = EMF;
    value = randFloat(0.0,4.90);
  }
  else if(hunterType==1){
    evidenceType = TEMPERATURE;
    value = randFloat(0.0,27.0);
  }
  else if(hunterType==2){
    evidenceType = FINGERPRINTS;
    value = 0.0;
  }
  else if(hunterType==3){
    evidenceType = SOUND;
    value = randFloat(40.0,70.0);
  }
  else{
    printf("%s\n","Something is wrong with hunter init" );
  }
  EvidenceType* evidence =  calloc(1,sizeof(EvidenceType));
  evidence->evidenceType = evidenceType;
  evidence->value = value;

  EvidenceNode* node = calloc(1,sizeof(EvidenceNode));
  node->data = evidence;
  addEvidenceToHunter(hunter,node);
}

int checkGhost(HunterType* hunter){
  if(hunter->room->ghost==NULL){
    return FALSE;
  }else{
    return TRUE;
  }
}

void increaseHunterFear(HunterType* hunter){
  if(checkGhost(hunter)){
    hunter->fearValue = hunter->fearValue +1;
    hunter->boredomValue = BOREDOM_MAX;
    printf("%s fear value was increased and boredom was reset\n",hunter->name);
  }
}

void decreaseHunterBoredom(HunterType* hunter){
  hunter->boredomValue = hunter->boredomValue-1;
  //printf("%s boredom was decreased\n",hunter->name);
}

//check if a ghost has 3 different types of ghostly evidence
int checkThreeGhostlyEvidence(HunterType* hunter){
  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = hunter->evidenceList;
  int count=0;
  int foundEMF =1;
  int foundTemp =1;
  int foundFinger =1;
  int foundSound =1;


  cur = list->head;

  if(cur==NULL){
    return 0;}

  else{

  while(cur!=NULL){
    if(checkIfGhostly(cur)){


      if(cur->data->evidenceType==0){
        if(foundEMF){
          foundEMF=0;
          count++;
        }
      }
      else if(cur->data->evidenceType==1){
        if(foundTemp){
          foundTemp=0;
          count++;
        }
      }
      else if(cur->data->evidenceType==2){
        if(foundFinger){
          foundFinger=0;
          count++;
        }
      }
      else if(cur->data->evidenceType==3){
        if(foundSound){
          foundSound=0;
          count++;
        }
      }
    }
    prev = cur;
    cur = cur->next;
  }
  if(count>=3){
    printf("%s has exited the building because they found 3 different ghostly evidence",hunter->name);
    sem_t* temp = &(hunter->room->mutex);
    sem_wait((temp));
    removeHunterFromRoom(hunter,hunter->room);
    sem_post((temp));
    return 1;
  }
  else{
    return 0;
  }

}
}

int guessGhost(HunterType* hunter){
  int emf=0;
  int temp=0;
  int finger =0;
  int sound = 0;

  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = hunter->evidenceList;
  cur = list->head;

  if(cur==NULL){
    return 0;}

  else{

  while(cur!=NULL){
    if(checkIfGhostly(cur)){


      if(cur->data->evidenceType==0){
          emf=1;

      }
      else if(cur->data->evidenceType==1){

          temp=1;

      }
      else if(cur->data->evidenceType==2){

          finger=1;

      }
      else if(cur->data->evidenceType==3){

          sound=1;

      }
    }
    prev = cur;
    cur = cur->next;
  }

  if(emf && temp && finger){
    return 0;
  }
  if(emf && temp && sound){
    return 1;
  }
  if(emf && finger && sound){
    return 2;
  }
  if(temp && finger && sound){
    return 3;
  }
  return -1;

}
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
