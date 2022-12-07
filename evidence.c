#include "defs.h"


/*
  Function:  initEvidenceList
  Purpose:   Initalize evidence list
       in:   Takes an pointer of the type EvidenceListType*
   return:   nothing
*/

void initEvidenceList(EvidenceListType* evidenceList){
  evidenceList->head =NULL;
}

/*
  Function:  leaveEvidenceInCurRoom
  Purpose:   Leave evidence in a room
       in:   Takes a pointer of the ghost
       in:   Takes an pointer of the type EvidenceType*
   return:   nothing
*/

void leaveEvidenceInCurRoom(GhostType* ghost, EvidenceType* evidence){
  //citation: taken from my tutorial 6 code and modified
  sem_wait(&(ghost->room->mutex));
  int type = evidence->evidenceType;
  EvidenceNode* cur;
  EvidenceNode* prev;
  EvidenceListType* list = ghost->room->evidenceList;
  EvidenceNode* new = calloc(1,sizeof(EvidenceNode));
  new->data = evidence;

  cur = list->head;

  if(cur==NULL){
    list->head = new;
    list->head->next = cur;
    printf("Ghost left evidence in %s with type: ", ghost->room->name);
    convertEvidenceEnumToString(type);
    printf("\n");
    sem_post(&(ghost->room->mutex));
    return;}

  else{

  while(cur!=NULL){

    prev = cur;
    cur = cur->next;
  }
  prev->next = new;
  new->next = cur;
  }
  printf("Ghost left evidence in %s with type: ", ghost->room->name);
  convertEvidenceEnumToString(type);
  printf("\n");
  sem_post(&(ghost->room->mutex));
}

/*
  Function:  checkIfGhostly
  Purpose:   checks if given evidence node is ghostly or not
       in:   Takes a pointer of the evidenceNode*
   return:   nothing
*/
int checkIfGhostly(EvidenceNode* evidence){
  int evidenceEnum = evidence->data->evidenceType;
  float value =evidence->data->value;

  if(evidenceEnum==0){
    if(value>=4.90 && value<=5.00){
      return 1;
    }
  }

  else if(evidenceEnum==1){
    if(value>=-10.0 && value<=0.0){
      return 1;
    }
  }
  else if(evidenceEnum==2){
    if(value==1.0){
      return 1;
  }
  }
  else if(evidenceEnum==3){
    if(value>=70.00 && value<=75.00){
      return 1;
  }
  }
  return 0;

}

/*
  Function:  convertEvidenceEnumToString
  Purpose:   prints out the evidence type given the enum
       in:   Takes a input of EvidenceClassType
   return:   nothing
*/

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
/*
  Function:  cleanupEvidenceNodes
  Purpose:   Cleanup evidence nodes given an EvidenceListType* pointer
       in:   Takes a pointer of EvidenceListType*
   return:   nothing
*/

void cleanupEvidenceNodes(EvidenceListType* list){
  //code taken from my t6 or t7 submission
  EvidenceNode* cur = list->head;
  EvidenceNode* next=NULL;
  while(cur!=NULL){
    next = cur->next;
    free(cur);
    cur = next;

}
}

/*
  Function:  cleanupEvidenceData
  Purpose:   Cleanup evidence data given an EvidenceListType* pointer
       in:   Takes a pointer of EvidenceListType*
   return:   nothing
*/
void cleanupEvidenceData(EvidenceListType* list){
  //code taken from my t6 or t7 submission
  EvidenceNode* cur = list->head;
  EvidenceNode* next=NULL;
  //free only data
  while(cur!=NULL){
    next = cur->next;
    free(cur->data);
    cur = next;

}
}
