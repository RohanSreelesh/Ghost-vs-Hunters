#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99
#define TRUE              1
#define FALSE              0

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

//room defs

typedef struct {
  char name[MAX_STR];
   struct RoomListType* rooms;
   struct EvidenceListType* evidenceList;
   struct HunterListType* hunters;
   struct GhostType* ghost;
   int numConnections;
   sem_t mutex;
} RoomType;


 typedef struct Node{
  RoomType *data;
  struct Node *next;
} RoomNode;

typedef struct RoomListType{
  RoomNode* head;
  RoomNode* tail;
} RoomListType;

//evidence
typedef struct {
  float value;
  EvidenceClassType evidenceType;
} EvidenceType;

 typedef struct EvidenceNode{
  EvidenceType *data;
  struct EvidenceNode *next;
} EvidenceNode;

typedef struct EvidenceListType{
  EvidenceNode* head;
} EvidenceListType;


//ghost
typedef struct GhostType{
  GhostClassType ghostType;
  RoomType *room;
  int boredomValue;
} GhostType;


//hunter
typedef struct {
  char name[MAX_STR];
  RoomType *room;
  int boredomValue;
  int fearValue;
  EvidenceListType* evidenceList;
  EvidenceListType* buildingEvidenceList;
  EvidenceClassType evidenceType;
  sem_t mutex;

} HunterType;

typedef struct HunterNode{
  HunterType *data;
  struct HunterNode *next;
} HunterNode;

typedef struct HunterListType{
  HunterNode* head;
} HunterListType;

//building
typedef struct {
  GhostType ghost;	//statically allocated
  HunterListType* hunters;
  RoomListType* rooms;
  EvidenceListType* evidenceList;   //deprecated will be removed
}BuildingType;


int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms

//room functions
void initRoom(RoomType*, char*);
void initRoomList(RoomListType*);
void connectRooms(RoomType*, RoomType*);
void printRooms(RoomListType *);
void cleanupRoomData(RoomListType *);
void cleanupRoomNodes(RoomListType *);
void removeHunterFromRoom(HunterType* , RoomType*);

void appendRoom(RoomListType* , RoomNode* );

//hunter functions
void initHunter(HunterType*, RoomType* , char* , EvidenceClassType ,EvidenceListType*);
void initHunterList(HunterListType* );
void addHunterToRoom(HunterType*, RoomType*);
void printHunter(HunterListType *);
void cleanupHunterData(HunterListType *);
void cleanupHunterNodes(HunterListType *);
void cleanupHunterData(HunterListType *);
void collectEvidenceFromRoom(HunterType*);
void addEvidenceToHunter(HunterType* , EvidenceNode*);
void shareEvidenceBwHunters(HunterType*, HunterType*);
void convertEvidenceEnumToString(EvidenceClassType);
void moveHunter(HunterType* );
int checkGhost(HunterType*);
void increaseHunterFear(HunterType*);
void decreaseHunterBoredom(HunterType*);
int checkThreeGhostlyEvidence(HunterType*);
HunterType* checkIfAnotherHunterInRoom(HunterType*);
void* mainHunterFunction(void* );
void genrateStandardEvidence(HunterType*);
void removeHunterFromRoomFinal(HunterType*,RoomType*);
int guessGhost(HunterType*);

//building functions
void initBuilding(BuildingType* );
void cleanupBuilding(BuildingType*);
void addHunterToBuilding(HunterType*, BuildingType*);
void addEvidenceToBuilding(HunterType*, EvidenceNode*);

//evidence functions
void initEvidenceList(EvidenceListType*);
void printEvidence(EvidenceListType *);
void leaveEvidenceInCurRoom(GhostType*, EvidenceType*);
void generateEvidence(GhostType*);
void cleanupEvidenceNodes(EvidenceListType*);
void cleanupEvidenceData(EvidenceListType*);

//ghost funtions
void initGhost(GhostType*, RoomType*);
int checkHunter(GhostType*);
void updateGhostBoredom(GhostType*,int);
int checkIfGhostly(EvidenceNode*);
void moveGhost(GhostType*);
int checkEndHaunt(GhostType*);
void* mainGhostFunction(void *);
