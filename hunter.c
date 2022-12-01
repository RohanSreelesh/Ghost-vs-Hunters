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
