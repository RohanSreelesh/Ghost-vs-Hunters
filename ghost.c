#include "defs.h"

void initGhost(GhostType* ghost, RoomType* room){
  int randomEnum;
  randomEnum = randInt(0,4);
  ghost->ghostType = randomEnum;
  ghost->room = room;
  ghost->boredomValue = BOREDOM_MAX;
}

//checks for hunter in the room returns 1 or 0
int checkHunter(GhostType* ghost){
  HunterNode* cur = ghost->room->hunters->head;

  if(cur==NULL){
    return FALSE;
  }
  else{
    return TRUE;
  }
}

//input will be value of check hunter
void updateGhostBoredom(GhostType* ghost,int boolean){
  if(boolean){
    ghost->boredomValue = BOREDOM_MAX;
  }
  else{
    ghost->boredomValue = ghost->boredomValue -1;
  }

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
  //printf("%f\n",value );

  //add it to evidence evidence list of the room
  leaveEvidenceInCurRoom(ghost,evidence);


}
