#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

// Use #define to please the c89 compiler

#define TOTALROOMS 10
#define TYPETOTAL 3
#define ROOMS_TO_CREATE 7
#define MAX_CONNECTIONS 6

// constants

// const int TOTALROOMS = 10;
// const int TYPETOTAL = 3;
// const int ROOMS_TO_CREATE = 7;
// const int MAX_CONNECTIONS = 6;

// Create the room struct to store information about each room
struct Room {
  char* name;
  struct Room* outboundConnections[MAX_CONNECTIONS];
  char* room_type;
  int numOfConnections;
};

// function declarations
void generateRooms(struct Room* roomArr);
char* createDirectory();
int IsGraphFull(struct Room* roomArr);
void createConnections(struct Room* roomArr);
void AddRandomConnection(struct Room* roomArr);
struct Room GetRandomRoom(struct Room* roomArr);
int IsSameRoom(struct Room x, struct Room y);
int CanAddConnectionFrom(struct Room x);
int ConnectionAlreadyExists(struct Room x, struct Room y);
void ConnectRoom(struct Room* x, struct Room* y);

// store the room names
char* roomList[TOTALROOMS] ={"Kitchen", "ManCave", "SheShed", "Zen", "Game", "Boiler", "Library", "Cellar", "Attic", "Office"};

// store the types of rooms
char* roomType[TYPETOTAL] = {"START_ROOM", "MID_ROOM", "END_ROOM"};

int main() {

srand(time(0)); // set the time seed

// generate rooms
struct Room roomsArr[ROOMS_TO_CREATE];
generateRooms(roomsArr);

// create separate files for each room


// outputs all of the room names'
  // int i=0;
  // for (i=0; i<TOTALROOMS; i++) {
  //   printf("Room # %d: %s\n", i+1, roomList[i]);
  // }

// outputs all of the room in the created array
  int i;
  for (i=0; i<ROOMS_TO_CREATE; i++) {
    printf("Room # %d: %s\n", i+1, roomsArr[i].name);
  }

  // TESTING STATEMENTS -- print to console for testing purpose
  // ConnectRoom(&roomsArr[0], &roomsArr[4]);
  // for (i = 0; i < ROOMS_TO_CREATE; i++) {
  //   printf("Room %d Name: %s\n", i+1, roomsArr[i].name);
  //   printf("Room %d Type: %s\n", i+1, roomsArr[i].room_type);
  //   printf("Room %d # of Connections: %d\n", i+1, roomsArr[i].numOfConnections);
  // } 
  // printf("Room 1 Connected to: %s\n", roomsArr[0].outboundConnections[0]->name);
  // printf("Room 5 Connected to: %s\n", roomsArr[4].outboundConnections[0]->name);

  return 0;
}

// createDirectory will be used to create the individual folders for each room file

char* createDirectory() {
  int pid = getpid(); //store process id to append to directory name
  char* directoryBuffer = malloc(sizeof(char) * 50);

  sprintf(directoryBuffer, "rotenbed.rooms.%d", pid); // create name of directory
  // printf("New Dir Name: %s\n", directoryBuffer);
  mkdir(directoryBuffer, 0700); // create new directory and set permissions
  // printf("PID: %d:\n", pid);
  return directoryBuffer;
}

// generateRooms will be used to randomly create the 7 rooms
// It will set the initial numOfConnections to 0, assign names,
// and assign types (1st room being start_room, last room is end_room, and all others are mid_room)
// outbound connections will be set to NULL and then assigned by calling 'createConnections()'

void generateRooms(struct Room *roomArr) {
  // printf("Creating Rooms Now\n");
  int i;
  for(i = 0; i < ROOMS_TO_CREATE; i++) {
    roomArr[i].numOfConnections = 0; // set initial room connections to 0
  }

  // set the room type for each room, first room always being the start, last room always the ending room, all other rooms Mid room
  int j;
  for(j = 0; j < ROOMS_TO_CREATE; j++) {
    if(j==0) {
      roomArr[j].room_type = roomType[0]; 
    } else if (j == ROOMS_TO_CREATE - 1) {
      roomArr[j].room_type = roomType[2];
    } else {
      roomArr[j].room_type = roomType[1];
    }
  }

  // randomly assign room name to the current room by using modulo operator & check if name already exists 

  //first assign TBD names to prevent seg fault errors
  for (i = 0; i < ROOMS_TO_CREATE; i++) {
    roomArr[i].name = "TBD";
  }

  int k, x, randomRoomIndex;
  char* str1;
  char* str2;
  for (k = 0; k < ROOMS_TO_CREATE; k++) { //for each room name to create
    int roomNameUsed = 0;
    while (!roomNameUsed) { // while the room name is not used
      roomNameUsed = 1;
      randomRoomIndex = rand() % TOTALROOMS; // this will generate random #s 0-9
      for (x = 0; x < ROOMS_TO_CREATE; x++) { 
        str1 = roomList[randomRoomIndex];
        str2 = roomArr[x].name;
        // printf("Str1: %s\n Str2: %s\n", str1, str2);
        // printf("RoomArr[%d] = %s\n", x, roomArr[x].name);
        // printf("RandomRoom: %s\n", roomList[randomRoomIndex]);
        if (strcmp(str1, str2) == 0) { //check if name has been used
          roomNameUsed = 0; // if used, adjust variable to re-randomize
        }
      }
    }
    // if room name has not been used, assign to current 'k' indexed room  
    roomArr[k].name = roomList[randomRoomIndex];
  }

// set outbound connections to null before assigning to prevent segfaults
  int a, b;
  for (a = 0; a < ROOMS_TO_CREATE; a++) {
    for (b = 0; b < MAX_CONNECTIONS; b++) {
      roomArr->outboundConnections[b] = NULL;
    }
  }

  // call the createConnections function to add all of the array connections and stop upon completion
  createConnections(roomArr);

    // TESTING STATEMENTS -- print to console for testing purpose
    printf("\n");
    for (i = 0; i < ROOMS_TO_CREATE; i++) {
      printf("ROOM NAME: %s\n", roomArr[i].name);
      int c;
      for (c = 0; c < roomArr[i].numOfConnections; c++) {
        printf("CONNECTION %d : %s\n", c+1, roomArr[i].outboundConnections[c]->name);
      }
      printf("ROOM TYPE: %s\n", roomArr[i].room_type);
      printf("\n");
      // printf("Room %d # of Connections: %d\n\n", i+1, roomArr[i].numOfConnections);
    } 

  return;
}

// creates all connections for the graph
void createConnections(struct Room* roomArr) {
  while (IsGraphFull(roomArr) == 0) {
    AddRandomConnection(roomArr);
  }
  return;
}

// Returns true if all rooms have 3 to MAX_CONNECTIONS outbound connections, false otherwise
int IsGraphFull(struct Room* roomArr)  
{
  int i;
  int full = 1; // if all rooms meet requirements, return 1, else return 0

  for (i = 0; i < ROOMS_TO_CREATE; i++) {
    if(roomArr[i].numOfConnections < 3 || roomArr[i].numOfConnections > MAX_CONNECTIONS) {
      // printf("Room # %d Connections: %d\n", i, roomArr[i].numOfConnections);
      full = 0;
    }
  }

  return full;
}

// **********************************************************************************************
// **********************************************************************************************
// // Adds a random, valid outbound connection from a Room to another Room

void AddRandomConnection(struct Room* roomArr) {
  struct Room A;  // Maybe a struct, maybe global arrays of ints
  struct Room B;

  while(1) { 
    // printf("Inside While\n");
    A = GetRandomRoom(roomArr);

    if (CanAddConnectionFrom(A) == 1) {
      break;
    }
  }

  do {
    // printf("Inside Do Loops\n");
    B = GetRandomRoom(roomArr);
    // if (CanAddConnectionFrom(B) == 0) {
    //   printf("CanAddConnectionFrom(B) == 0\n");
    // }
    // if (IsSameRoom(A, B) == 1) {
    //   printf("IsSameRoom(A, B) == 1\n");
    // }
    // if (ConnectionAlreadyExists(A, B) == 1) {
    //   printf("ConnectionAlreadyExists(A, B) == 1\n");
    // }
  } while (CanAddConnectionFrom(B) == 0 || IsSameRoom(A, B) == 1 || ConnectionAlreadyExists(A, B) == 1);

  // printf("finished do while\n");

  int i, A_Index, B_Index;
  for (i = 0; i < ROOMS_TO_CREATE; i++) {
    if (roomArr[i].name == A.name) {
      A_Index = i;
      // printf("A_Index is: %d\n", i);
    }
    if (roomArr[i].name == B.name) {
      // printf("A_Index is: %d\n", i);
      B_Index = i;
    }
  }

  ConnectRoom(&roomArr[A_Index], &roomArr[B_Index]);  // TODO: Add this connection to the real variables, 
  // ConnectRoom(&B, &A);  //  because this A and B will be destroyed when this function terminates
  // printf("A: Num of Connections: %d\n", A.numOfConnections);
}

// **********************************************************************************************
// **********************************************************************************************

// Returns a random Room, does NOT validate if connection can be added
struct Room GetRandomRoom(struct Room* roomArr) {
  int randomIndex = rand() % ROOMS_TO_CREATE;
  // printf("randomIndex: %d\n", randomIndex);
  return roomArr[randomIndex];
}

// Returns true if a connection can be added from Room x (< MAX_CONNECTIONS outbound connections), false otherwise
int CanAddConnectionFrom(struct Room x) {
  if (x.numOfConnections < MAX_CONNECTIONS) {
    return 1;
  }
  return 0;
}

// Returns true if a connection from Room x to Room y already exists, false otherwise
int ConnectionAlreadyExists(struct Room x, struct Room y) {
  int i;
  for (i = 0; i < x.numOfConnections; i++) {
    if (x.outboundConnections[i]->name == y.name) {
      return 1;
    }
  }
  return 0;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct Room* x, struct Room* y) {
  // printf("Inside Connect Room\n");
  x->outboundConnections[x->numOfConnections] = y;
  x->numOfConnections++;
  y->outboundConnections[y->numOfConnections] = x;
  y->numOfConnections++;
}

// Returns true if Rooms x and y are the same Room, false otherwise
int IsSameRoom(struct Room x, struct Room y) {
  if (x.name == y.name) {
    return 1;
  } 
  return 0;
}