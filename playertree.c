// compiler header files
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // rand
#include <stdio.h> // printf

// program header file
#include "bomberman.h"

// global declarations
extern const char BOMBERMAN; // ascii used for bomberman
extern const char WALL; // ascii used for the walls
extern const char BREAKABLE_WALL; // ascii used for the breakable walls
extern const char PATH; // ascii used for the pathes
extern const char EXIT; // ascii used for the EXIT
extern const char BOMB; // ascii used for a bomb
extern const char BOMB_BONUS; // ascii used for the bombs
extern const char FLAME_BONUS; // ascii used for the flames
extern const char FLAME_ENEMY; // ascii used for the flame ennemy
extern const char GHOST_ENEMY; // ascii used for the ghost ennemy

extern const int BOMB_DELAY; // time before a bomb explode
extern const int BREAKABLE_WALL_SCORE; // reward for bombing a breakable wall
extern const int FLAME_ENEMY_SCORE; // reward for bombing a flame enemy
extern const int GHOST_ENEMY_SCORE; // reward for bombing a ghost enemy 
extern const int BOMB_BONUS_SCORE; // reward for a bomb bonus, which increases by 1 the number of bombs that can be dropped at a time
extern const int FLAME_BONUS_SCORE; // reward for a flame bonus, which increses by 1 the explosion range of bombs
extern const int VIEW_DISTANCE; // bomberman view distance 

extern bool DEBUG; // indicates whether the game runs in DEBUG mode

const char * binome="Random"; // student name goes here

// prototypes of the local functions/procedures
void printAction(action);
void printBoolean(bool);
int min4(int,int,int,int);
int posmin4(int,int,int,int);
action randomMoove(tree);
int depthChar(tree,int,char);
int GoNearWalls(tree);
/*
  bomberman function:
  This function randomly select a valid move for BOMBERMAN based on its current position on the game map.
 */
action bomberman(
		 tree map, // 4-ary tree of char modeling a subset of the game map
		 action last_action, // last action made, -1 in the be beginning 
		 int remaining_bombs, // number of bombs
		 int explosion_range // explosion range for the bombs 
		 ) {
  action a; // action to choose and return
  if (remaining_bombs==0) return randomMoove(map);
  else{
    int acttodo=GoNearWalls(map);
    switch (acttodo) 
    {
    case 1:
      a=NORTH;
      break;
    case 2:
      a=EAST;
      break;
    case 3:
      a=SOUTH;
      break;
    case 4:
      a=WEST;
      break;
    case 5:
      a=BOMBING;
      break;
    default:
      a=randomMoove(map);
      break;
    }
    return a;
  }
}

/*
  printAction procedure:
  This procedure prints the input action name on screen.
 */
void printAction(action a) {
  switch(a) {
  case BOMBING:
    printf("BOMBING");
    break;
  case NORTH:
    printf("NORTH");
    break;
  case EAST:
    printf("EAST");
    break;
  case SOUTH:
    printf("SOUTH");
    break;
  case WEST:
    printf("WEST");
    break;
  }
}


/*
  printBoolean procedure:
  This procedure prints the input boolan value on screen.
 */
void printBoolean(bool b) {
  if(b==true) {
    printf("true");
  }
  else {
    printf("false");
  }
}

int GoNearWalls(tree map){//se rapproche d'un mur et pose une bombe  
  int ndep=depthChar(map->n,0,BREAKABLE_WALL);
  int edep=depthChar(map->e,0,BREAKABLE_WALL);
  int sdep=depthChar(map->s,0,BREAKABLE_WALL);
  int wdep=depthChar(map->w,0,BREAKABLE_WALL);
  if (ndep==0 || edep==0 || sdep==0 || wdep==0) return 5;
  else return posmin4(ndep,edep,sdep,wdep);   
}


int depthChar(tree toexp,int depth,char chartofind){//gives the depth at which a specific character is found in a character tree. If not found, returns 10000.(val "infinite")
  if (toexp==0) return 10000;//if the subtree is empty
  else{
    if (toexp->c==chartofind) return depth;
    else if (toexp->c==PATH) return min4(depthChar(toexp->n,depth+1,chartofind),depthChar(toexp->e,depth+1,chartofind),depthChar(toexp->s,depth+1,chartofind),depthChar(toexp->w,depth+1,chartofind));
    else return 10000;  
}}

int min4(int a,int b,int c,int d){//return minimum between four ints
  if (a>b){
    if (b>c){
      if (c>d)
        return d;
      else
        return c;
    }
    else{
      if (b>d) return d;
      else return b; 
    }
  }
  else{
    if (a>c){
      if (c>d) return d;
      else return c;
    }
    else {
      if (a>d) return d;
      else return a;
    }
  }
}
int posmin4(int a,int b,int c,int d){//return the position of the minimum between four ints (a return 0,b return 1 ...)
  if (a>b){
    if (b>c){
      if (c>d)
        return 4;
      else
        return 3;
    }
    else{
      if (b>d) return 4;
      else return 2; 
    }
  }
  else{
    if (a>c){
      if (c>d) return 4;
      else return 3;
    }
    else {
      if (a>d) return 4;
      else return 1;
    }
  }
}
action randomMoove(tree map){//do a random moove(not bombing)
  bool ok=false; // ok will be set to true as soon as a randomly selected action is valid
  action a;
  do {
    a=rand()%5; // ramdomly select an action: 0=BOMBING, 1=NORTH,...

    if(DEBUG) { // print the randomly selected action, only in DEBUG mode
      printf("Candidate action is: ");
      printAction(a);
      printf("\n");
    }

    switch(a) { // check whether the randomly selected action is valid, i.e., if its preconditions are satisfied 
    case BOMBING:
      ok=false;//prevent from using bombing
      break;
    case NORTH:
      if((map->n)->c!=WALL && (map->n)->c!=BREAKABLE_WALL && (map->n)->c!=BOMB) ok=true;
      break;
    case EAST:
      if((map->e)->c!=WALL && (map->e)->c!=BREAKABLE_WALL && (map->e)->c!=BOMB) ok=true;
      break;
    case SOUTH:
      if((map->s)->c!=WALL && (map->s)->c!=BREAKABLE_WALL && (map->s)->c!=BOMB) ok=true;
      break;
    case WEST:
      if((map->w)->c!=WALL && (map->w)->c!=BREAKABLE_WALL && (map->w)->c!=BOMB) ok=true;
      break;
    }

    if(DEBUG) { // print whether the randomly selected action is valid, only in DEBUG mode
      printf("Is this candidate action valide? ");
      printBoolean(ok);
      printf("\n");
    }
  } while(!ok);

  return a; // answer to the game engine
}