#include "defs.h"

/*
  Function:  initGhost
  Purpose:   Initalize ghost and place it in a room
       in:   Takes an pointer of the type GhostType*
       in:   Takes an pointer of the type RoomType*
   return:   nothing
*/
void initGhost(GhostType* ghost, RoomType* room){
  int randomEnum;
  randomEnum = randInt(0,4);
  ghost->ghostType = randomEnum;
  ghost->room = room;
  ghost->boredomValue = BOREDOM_MAX;
}

/*
  Function:  mainGhostFunction
  Purpose:   main function which we pass into our threads, responsible for ghost behaviour
       in:   Takes an pointer of the type void *
   return:   void pointer
*/
void* mainGhostFunction(void *arg){
  GhostType* ghost = (GhostType*) arg;
  while(checkEndHaunt(ghost)!=-1){
    int randChoice;

    if(checkHunter(ghost)){
      randChoice = randInt(0,2);
      updateGhostBoredom(ghost,1);
      if(randChoice==0){
        generateEvidence(ghost);
        usleep(USLEEP_TIME);
        //important modify with caution as removing leads to longer simulations
        //this is because the ghost keeps generating evidence waiting for the hunters to move

      }
      else if(randChoice==1){
        continue; //do nothing
      }
      else{
        printf("SOMETHING WENT WRONG\n");
      }
    }
    else{
      updateGhostBoredom(ghost,0);

      randChoice = randInt(0,3);
      if(randChoice==0){
        generateEvidence(ghost);
      }
      else if(randChoice==1){
        moveGhost(ghost);
      }
      else if(randChoice==2){
        continue; //do nothing
      }
      else{
        printf("SOMETHING WENT WRONG\n");
      }
    }
  }
  printf("ghost left the building\n");
}

/*
  Function:  checkHunter
  Purpose:   Checks for a hunter in the same room
       in:   Takes an pointer of the type GhostType*
   return:   returns 1 if the hunter is present in the same room and returns 0 if not
*/
int checkHunter(GhostType* ghost){
  sem_wait(&(ghost->room->mutex));
  HunterNode* cur = ghost->room->hunters->head;
  sem_post(&(ghost->room->mutex));
  if(cur==NULL){
    return FALSE;
  }
  else{
    return TRUE;
  }
}

/*
  Function:  updateGhostBoredom
  Purpose:  updates ghost boredom based on the int value passed in
       in:   Takes an pointer of the type GhostType*
       in:   Takes in an input integer
   return:   nothing
*/
void updateGhostBoredom(GhostType* ghost,int boolean){
  if(boolean==1){
    ghost->boredomValue = BOREDOM_MAX;
  }
  else{
    ghost->boredomValue = ghost->boredomValue -1;
  }
  //printf("Current ghost boredom is %d\n",ghost->boredomValue );

}

/*
  Function:  checkEndHaunt
  Purpose:   checks if the boredom value is enough to end the hunt
       in:   Takes an pointer of the type GhostType*
   return:   -1 if hunt should end or 0 if the hunt can go on
*/
int checkEndHaunt(GhostType* ghost){

  if(ghost->boredomValue<=0){
      return -1;  //end hunt
  }
  return 0; //all is ok haunt can go on
}

/*
  Function:  generateEvidence
  Purpose:   Generates a random piece of evidence which falls in the ghostly range
       in:   Takes an pointer of the type GhostType*
   return:   nothing
*/
void generateEvidence(GhostType* ghost){
  //3 types of evidence for 4 typ of Ghosts
  int randChoice = randInt(0,3);
  int ghostEnum =  ghost->ghostType;
  EvidenceClassType evidenceType;
  float value;
  if(ghostEnum==0){
    if(randChoice==0){
       evidenceType = EMF;
       value = randFloat(4.7,5);

    }
    else if(randChoice==1){
       evidenceType = TEMPERATURE;
       value = randFloat(-10.0,1.0);
    }
    else if(randChoice==2){
       evidenceType = FINGERPRINTS;
       value = 1.0;
    }

  }
  else if(ghostEnum==1){
    if(randChoice==0){
       evidenceType = EMF;
       value = randFloat(4.7,5);

    }
    else if(randChoice==1){
       evidenceType = TEMPERATURE;
       value = randFloat(-10.0,1.0);
    }
    else if(randChoice==2){
       evidenceType = SOUND;
       value =randFloat(65.0,75.0);
    }
  }
  else if(ghostEnum==2){
    if(randChoice==0){
       evidenceType = EMF;
       value = randFloat(4.7,5);

    }
    else if(randChoice==1){
       evidenceType = FINGERPRINTS;
       value = 1.0;
    }
    else if(randChoice==2){
       evidenceType = SOUND;
       value =randFloat(65.0,75.0);
    }
  }
  else if(ghostEnum==3){
    if(randChoice==0){
      evidenceType = TEMPERATURE;
      value = randFloat(-10.0,1.0);

    }
    else if(randChoice==1){
       evidenceType = FINGERPRINTS;
       value = 1.0;
    }
    else if(randChoice==2){
       evidenceType = SOUND;
       value =randFloat(65.0,75.0);
    }
  }

  //build evicence node and add it to evidence list of the room
  EvidenceType* evidence =  calloc(1,sizeof(EvidenceType));
  evidence->evidenceType = evidenceType;
  evidence->value = value;


  //add it to evidence evidence list of the room
  leaveEvidenceInCurRoom(ghost,evidence);
}



/*
  Function:  moveGhost
  Purpose:   moves the ghost randomly to a connected room
       in:   Takes an pointer of the type GhostType*
   return:   nothing
*/
void moveGhost(GhostType* ghost){
  int sem =0;
  if (sem_trywait(&(ghost->room->mutex)) == 0) {
    sem=1;
  }
  else{
    return;
    }


  //selects a random room and moves to it
  int choices = ghost->room->numConnections;
  int count=1;
  int randChoice = randInt(1,choices+1);
  RoomListType* list =ghost->room->rooms;
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
        ghost->room->ghost =NULL;
        ghost->room = cur->data;
        ghost->room->ghost = ghost;   //here we try to change the next room
        printf("Ghost has moved to %s\n",ghost->room->name);
        sem_post(&(ghost->room->mutex));
        sem_post(&(cur->data->mutex));
        break;
      }else{
      }
    }
    prev = cur;
    cur = cur->next;
    count++;
    sem_post(&(ghost->room->mutex));
  }
  }
}

/*
  Function:  convertGhostEnumToString
  Purpose:   prints out the ghost type given a GhostClassType enum
       in:   Takes an input of the type GhostClassType
   return:   nothing
*/
void convertGhostEnumToString(GhostClassType ghostType){
  if(ghostType==0){
    printf(" %s ","POLTERGEIST" );
  }
  else if(ghostType==1){
    printf(" %s ","BANSHEE");
  }
  else if(ghostType==2){
    printf(" %s ","BULLIES");
  }
  else if(ghostType==3){
    printf(" %s ","PHANTOM");
  }
}
