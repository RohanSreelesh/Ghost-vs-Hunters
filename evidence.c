#include "defs.h"

void initEvidenceList(EvidenceListType* evidenceList){
  evidenceList->head =NULL;
}
//add evidence

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


void printEvidence(EvidenceListType *list) {
  //citation: taken from my tutorial 6 code and modified
  EvidenceNode* cur;
  cur = list->head;
  while(cur!=NULL){
    printf("%f\n", cur->data->value);
    cur = cur->next;
  }
}


void cleanupEvidenceNodes(EvidenceListType* list){
  EvidenceNode* cur = list->head;
  EvidenceNode* next=NULL;
  //free only data
  while(cur!=NULL){
    //cleanupHunterData(cur->data->hunters);
    next = cur->next;
    free(cur);
    cur = next;

}
}

void cleanupEvidenceData(EvidenceListType* list){
  EvidenceNode* cur = list->head;
  EvidenceNode* next=NULL;
  //free only data
  while(cur!=NULL){
    //cleanupHunterData(cur->data->hunters);
    next = cur->next;
    free(cur->data);
    cur = next;

}
}

// EvidenceNode* curEvidenceOfEvidences = cur->data->evidenceList->head;
// while(curEvidenceOfEvidences!=NULL){ //frees the evidence inside each hunter
//   nextEvidence = curEvidenceOfEvidences->next;
//   free(curEvidenceOfEvidences);
//   curEvidenceOfEvidences =nextEvidence;
// }
//delete evidence
