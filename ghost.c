#include "defs.h"

void initGhost(GhostType* ghost, RoomType* room){
  int randomEnum;
  randomEnum = randInt(0,4);
  ghost->ghostType = randomEnum;
  ghost->room = room;
  ghost->boredomValue = BOREDOM_MAX;
}

void* mainGhostFunction(void *arg){
  GhostType* ghost = (GhostType*) arg;
  while(checkEndHaunt(ghost)!=-1){
  //int i=0;
  //while(i!=10){
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
    //i++;
  }
  printf("ghost left the building\n");
}

//checks for hunter in the room returns 1 or 0
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

//input will be value of check hunter
void updateGhostBoredom(GhostType* ghost,int boolean){
  if(boolean==1){
    ghost->boredomValue = BOREDOM_MAX;
  }
  else{
    ghost->boredomValue = ghost->boredomValue -1;
  }
  //printf("Current ghost boredom is %d\n",ghost->boredomValue );

}


int checkEndHaunt(GhostType* ghost){

  if(ghost->boredomValue<=0){
      return -1;  //end hunt
  }
  return 0; //all is ok haunt can go on
}

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


void moveGhost(GhostType* ghost){
  int sem =0;
  if (sem_trywait(&(ghost->room->mutex)) == 0) {
    sem=1;
  }
  else{
    //printf("GHOST Could not move due to sem\n");
    return;
    }
  int choices = ghost->room->numConnections;
  int count=1;
  int randChoice = randInt(1,choices+1);
  //printf("rand:%d\n",randChoice );
  RoomListType* list =ghost->room->rooms;
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
        ghost->room->ghost =NULL;
        ghost->room = cur->data;
        ghost->room->ghost = ghost;   //here we try to change the next room
        printf("Ghost has moved to %s\n",ghost->room->name);
        sem_post(&(ghost->room->mutex));
        sem_post(&(cur->data->mutex));
        break;
      }else{
        //printf("GHOST Could not move due to sem2\n");

      }
    }
    prev = cur;
    cur = cur->next;
    count++;
    sem_post(&(ghost->room->mutex));
  }
  }
}
