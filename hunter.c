#include "defs.h"

/*
  Function:  initGhost
  Purpose:   Initalize hunter and place it in a room
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type RoomType*
       in:   Takes an pointer of the type char*
       in:   Takes input EvidenceClassType
   return:   nothing
*/
void initHunter(HunterType* hunter, RoomType* room, char* name, EvidenceClassType type){
  strcpy(hunter->name, name);
  hunter->room = room;
  hunter->boredomValue = BOREDOM_MAX;
  hunter->fearValue = 0;
  hunter->evidenceType = type;
  EvidenceListType* evidences = calloc(1,sizeof(EvidenceListType));
  initEvidenceList(evidences);
  hunter->evidenceList = evidences;
  //hunter->buildingEvidenceList = sharedEvidenceList;
  sem_init(&(hunter->mutex),0 ,1);
}

/*
  Function:  initHunterList
  Purpose:   Initalize hunter list
       in:   Takes an pointer of the type HunterListType*
   return:   nothing
*/
void initHunterList(HunterListType* hunterList){
  hunterList->head =NULL;
}


/*
  Function:  mainHunterFunction
  Purpose:   main function which we pass into our threads, responsible for hunter behaviour
       in:   Takes an pointer of the type void *
   return:   void pointer
*/
void* mainHunterFunction(void* arg){
  HunterType* hunter = (HunterType*) arg;

  while(checkThreeGhostlyEvidence(hunter)!=1 && hunter->fearValue<100 && hunter->boredomValue>0){
    increaseHunterFear(hunter); //checks for ghost and adds accordingly
    int randChoice;
    randChoice = randInt(0,3);
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
  }
  removeHunterFromRoomFinal(hunter,hunter->room);
  printf("hunter left the building\n");
}


/*
  Function:  collectEvidenceFromRoom
  Purpose:   collect evidence from the room a hinter is in, if there is no evidence, it reads random standard evidence
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void collectEvidenceFromRoom(HunterType* hunter){
  sem_wait(&(hunter->room->mutex));
  EvidenceNode* evidenceToRemove;


   EvidenceNode* cur;
   EvidenceNode* prev;
   EvidenceListType* evidences =  hunter->room->evidenceList;

   cur = hunter->room->evidenceList->head;

   int found= 0;
   while(cur!=NULL){
     //if evidence type matches the type the hunter can read it
     if(cur->data->evidenceType==hunter->evidenceType){
       found=1;
       break;
     }
     prev = cur;
     cur = cur->next;
   }
   //hunter did not find any reeadable evidence in the room and hence reads random standard evidence
  if(found==0){
    printf("%s did not find any evidence that it can read, it will read random standard evidence\n",hunter->name);
    genrateStandardEvidence(hunter);
    sem_post(&(hunter->room->mutex));
    return;
  }

  if(cur!=NULL){
  EvidenceType* evidence = calloc(1,sizeof(EvidenceType));
  evidence->evidenceType =cur->data->evidenceType;
  evidence->value=cur->data->value;
  EvidenceNode* node = calloc(1,sizeof(EvidenceNode));
  node->data=evidence;

  addEvidenceToHunter(hunter,node);

 if(cur==evidences->head){
   EvidenceNode* tofree = evidences->head;
   evidences->head = evidences->head->next;
   free(tofree->data);
   free(tofree);

 }
 else if (prev!=NULL){
   EvidenceNode* tofree =prev->next;
   prev->next =prev->next->next;
   free(tofree->data);
   free(tofree);

 }
 sem_post(&(hunter->room->mutex));

}
}



/*
  Function:  addEvidenceToHunter
  Purpose:   adds evidence to a hunters evidence collection
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type EvidenceNode*
   return:   nothing
*/
void addEvidenceToHunter(HunterType* hunter, EvidenceNode* evidence){
  //citation: taken from my tutorial 6 code and modified

  //whenever we add to hunter list, we also add to the buildings RoomListType

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
      //Add only if ghostly evidence
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

/*
  Function:  checkIfAnotherHunterInRoom
  Purpose:   Checks if there is another hunter in the room, helper function for sharing evidence
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void checkIfAnotherHunterInRoom(HunterType* hunter){
  sem_wait(&(hunter->room->mutex));
  HunterListType* list =hunter->room->hunters;
  HunterNode* cur;
  HunterNode* prev;
  cur = list->head;

  if(cur==NULL){
    printf("Something went wrong\n");
    sem_post(&(hunter->room->mutex));
    return;}

  else{

  while(cur!=NULL){
    //printf("name of room: %s\n", cur->data->name);
    if(hunter->name!=cur->data->name){
      printf("%s found another hunter(%s) will attempt to share evidence\n", hunter->name,cur->data->name);
      //if (sem_trywait(&(cur->data->room->mutex)) == 0) {
      if(cur!=NULL){
        shareEvidenceBwHunters(hunter,cur->data);
      }
        //sem_post(&(cur->data->room->mutex));
      //}


      sem_post(&(hunter->room->mutex));
      return;
      break;
    }
    prev = cur;
    cur = cur->next;
  }
  printf("%s did not find another hunter to share evidence with\n", hunter->name);
  sem_post(&(hunter->room->mutex));
  return;
  }

}

/*
  Function:  shareEvidenceBwHunters
  Purpose:   Shares evidence between hunters given the conditions are satisfied
       in:   Takes an pointer of the type HunterType*
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void shareEvidenceBwHunters(HunterType* hunter1, HunterType* hunter2){
  //if evidence is ghostly, add it to the other hunters collection
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
        printf("%s has shared GHOSTLY", hunter1->name);
        convertEvidenceEnumToString(evidence->evidenceType);
        printf(" evidence with %s\n",hunter2->name);
        addEvidenceToHunter(hunter2,node);
        break;
      }
      prev = cur;
      cur = cur->next;
    }
}



/*
  Function:  moveHunter
  Purpose:   moves the hunter randomly to a connected room
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void moveHunter(HunterType* hunter){
  int sem =0;
  sem_t* temp = &(hunter->room->mutex);
  if (sem_trywait(&(hunter->room->mutex)) == 0) {
    sem=1;
  }
  else{
    return;
  }


  int choices = hunter->room->numConnections;
  int count=1;
  int randChoice = randInt(1,choices+1);
  RoomListType* list =hunter->room->rooms;
  RoomNode* cur;
  RoomNode* prev;
  cur = list->head;

  if(cur==NULL){
    printf("Something went wrong\n");
    return;}

  else{

  while(cur!=NULL){
    if(count==randChoice){
      if (sem_trywait(&(cur->data->mutex)) == 0) {
        removeHunterFromRoom(hunter,hunter->room);
        addHunterToRoom(hunter,cur->data);
        hunter->room = cur->data;
        sem_post(&(cur->data->mutex));
        break;
      }
      else{
        break;
      }

    }
    prev = cur;
    cur = cur->next;
    count++;
  }
  sem_post((temp));

  }

}

/*
  Function:  genrateStandardEvidence
  Purpose:   Generates evidence in the standard range and also adds to the hunters collection using a helper funciton
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
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


/*
  Function:  increaseHunterFear
  Purpose:   Increases the fear value of a hunter
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void increaseHunterFear(HunterType* hunter){
  if(checkGhost(hunter)){
    hunter->fearValue = hunter->fearValue +1;
    hunter->boredomValue = BOREDOM_MAX;
    printf("%s fear value was increased and boredom was reset\n",hunter->name);
  }
}

/*
  Function:  decreaseHunterBoredom
  Purpose:   Decreases the boredom value of a hunter
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
void decreaseHunterBoredom(HunterType* hunter){
  hunter->boredomValue = hunter->boredomValue-1;
  //printf("%s boredom was decreased\n",hunter->name);
}

/*
  Function:  checkThreeGhostlyEvidence
  Purpose:   Checks if a given hunter has 3 different types of ghostly evidence
       in:   Takes an pointer of the type HunterType*
   return:   nothing
*/
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

/*
  Function:  guessGhost
  Purpose:   Checks if a given hunter can guess the ghost
       in:   Takes an pointer of the type HunterType*
   return:   returns the enum of the ghost if successful or else it returns -1
*/
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

/*
  Function:  cleanupHunterData
  Purpose:   Cleanup hunter node data given an HunterListType* pointer
       in:   Takes a pointer of EvidenceListType*
   return:   nothing
*/
void cleanupHunterData(HunterListType *list){
  //citation: code taken from MY assignment 4 submission
  HunterNode* cur = list->head;
  HunterNode* next=NULL;
  //free only nodes
  while(cur!=NULL){
    //cleanup each hunters evidence list
    cleanupEvidenceData(cur->data->evidenceList);
    cleanupEvidenceNodes(cur->data->evidenceList);

    next = cur->next;
    free(cur->data->evidenceList);
    free(cur->data);
    cur = next;
  }
}




/*
  Function:  cleanupHunterNodes
  Purpose:   Cleanup hunter nodes given an HunterListType* pointer
       in:   Takes a pointer of EvidenceListType*
   return:   nothing
*/
void cleanupHunterNodes(HunterListType *list){
  //citation: code taken from MY assignment 4 submission
  HunterNode* cur = list->head;
  HunterNode* next=NULL;
  //free only nodes
  while(cur!=NULL){
    next = cur->next;
    free(cur);

    cur = next;
  }
}
