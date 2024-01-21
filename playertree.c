// compiler header files
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // rand
#include <stdio.h> // printf
#include <math.h> //abs
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
int posMin4(int,int,int,int);
int posMax4(int,int,int,int);
action randommove(tree);
int depthChar(tree,int,char);
int depthCharEnemy(tree,int);
int GoNearWalls(tree);
int GoNearEnemy(tree);
int GoNearBonuses(tree);
action choose_Side(tree,int);
int count_Safe_Paths(tree,int,int,bool);
int isThereBombs(tree);
bool isThereWalls(tree);
bool isThereBonuses(tree);
bool isThereEnemies(tree);
bool sMapBomb(tree);
bool sMapWalls(tree);
bool sMapBonuses(tree);
bool sMapEnemy(tree);
int NearEnemy(tree);
action escapeBomb(tree,int,int);
bool bombThreat(tree,int,int);
int* posBomb(tree,int*);
action randomSafeMove(tree,int,int);
void addTab(int*,int*,int);
void copyTab(int*,int*,int);
action SafeLook(tree,action);
bool IsAnEnemy(tree);
bool IsAGhost(tree);
int escapeGhost(tree);
int posMax3(int,int,int);
int posMax2(int,int);
void order4(int*,int*,int*,int*);
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
  if (last_action==BOMBING) {
  	a=choose_Side(map,explosion_range);
  	//printf("called ChooseSide\n");
  	}
  else{
    int bomb=isThereBombs(map);
    if (bomb==-1) {
        if (isThereEnemies(map)){
          switch(NearEnemy(map)){
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
            case 6://we aren't near an enemy
              int acttodo=GoNearEnemy(map);
                //printf("Go Near Walls\n");
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
                    a=randommove(map);
                    //printf("called randomMove\n");
                    break;            
                }
              break;
          }
        }
        else{
          if (isThereBonuses(map)){
            int acttodo=GoNearBonuses(map);
            //printf("bONUS?\n");
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
              a=randommove(map);
              //printf("called randomMove\n");
              break;
            }
          }
          else{     
            if (isThereWalls(map)){
                  int acttodo=GoNearWalls(map);
                  //printf("Go Near Walls\n");
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
                      a=randommove(map);
                      //printf("called randomMove\n");
                      break;            
                  }
            }
            else a=randommove(map);
          }
        }
      }
    else {a=escapeBomb(map,bomb,explosion_range);
    	  //printf("called escapeBomb\n");
    }}
    return SafeLook(map,a);
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
  default:
  	printf("NONE");
  	break;
  }
}

action SafeLook(tree map,action a) {
  switch(a) {
  case BOMBING:
    return BOMBING;
    break;
  case NORTH:
    if ((map->n)->c==BREAKABLE_WALL||(map->n)->c==WALL ||(map->n)->c==BOMB){
      //printf("north is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case EAST:
    if ((map->e)->c==BREAKABLE_WALL||(map->e)->c==WALL ||(map->e)->c==BOMB){
      //printf("east is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case SOUTH:
    if ((map->s)->c==BREAKABLE_WALL||(map->s)->c==WALL ||(map->s)->c==BOMB){
      //printf("south is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case WEST:
    if ((map->w)->c==BREAKABLE_WALL||(map->w)->c==WALL ||(map->w)->c==BOMB){
      //printf("west is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  default:
  	//printf("NO Action?go randomMove");
    return SafeLook(map,randommove(map));
  	break;
  }
  return a;
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
  else return posMin4(ndep,edep,sdep,wdep);   
}

int GoNearBonuses(tree map){//se rapproche d'un mur et pose une bombe  
  int ndep=depthChar(map->n,0,BOMB_BONUS);
  int edep=depthChar(map->e,0,BOMB_BONUS);
  int sdep=depthChar(map->s,0,BOMB_BONUS);
  int wdep=depthChar(map->w,0,BOMB_BONUS);
  //printf("%d ,%d ,%d ,%d (bomb)\n",ndep,edep,sdep,wdep);
  if (ndep>=10000 && edep>=10000 && sdep>=10000 && wdep>=10000 ) {
  	ndep=depthChar(map->n,0,FLAME_BONUS);
  	edep=depthChar(map->e,0,FLAME_BONUS);
  	sdep=depthChar(map->s,0,FLAME_BONUS);
  	wdep=depthChar(map->w,0,FLAME_BONUS);
  	//printf("%d ,%d ,%d ,%d (flame)\n",ndep,edep,sdep,wdep);
  	
  	return posMin4(ndep,edep,sdep,wdep); 
  }
  return posMin4(ndep,edep,sdep,wdep);   
}

int GoNearEnemy(tree map){//se rapproche d'un mur et pose une bombe  
  int ndep=depthCharEnemy(map->n,0);
  int edep=depthCharEnemy(map->e,0);
  int sdep=depthCharEnemy(map->s,0);
  int wdep=depthCharEnemy(map->w,0);
  return posMin4(ndep,edep,sdep,wdep);   
}

int depthChar(tree toexp,int depth,char chartofind){//gives the depth at which a specific character is found in a character tree. If not found, returns 10000.(val "infinite")
  if (toexp==0) return 10000;//if the subtree is empty
  else{
    if (toexp->c==chartofind) return depth;
    else if (toexp->c==PATH) return min4(depthChar(toexp->n,depth+1,chartofind),depthChar(toexp->e,depth+1,chartofind),depthChar(toexp->s,depth+1,chartofind),depthChar(toexp->w,depth+1,chartofind));
    else return 10000;  
}}

int depthCharEnemy(tree toexp,int depth){//gives the depth at which a specific character is found in a character tree. If not found, returns 10000.(val "infinite")
  if (toexp==0) return 10000;//if the subtree is empty
  else{
    if (IsAnEnemy(toexp->c)) return depth;
    else if (toexp->c==PATH) return min4(depthCharEnemy(toexp->n,depth+1),depthCharEnemy(toexp->e,depth+1),depthCharEnemy(toexp->s,depth+1),depthCharEnemy(toexp->w,depth+1));
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
int posMin4(int a,int b,int c,int d){//return the position of the minimum between four ints (a return 1,b return 2 ...)
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
int posMax4(int a,int b,int c,int d){//return the position of the maximum between four ints (a return 1,b return 2 ...
  if (a<b){

    if (b<c){
    	
      if (c<d){
      	
        return 4;}
      else{
      	
        return 3;}
    }
    else{
    	
      if (b<d) {
      	
      	return 4;}
      else {
      
      return 2; }
    }
  }
  else{
    
    if (a<c){
    	
      if (c<d) {
      
      return 4;}
      else {
      return 3;}
      
    }
    else {
      if (a<d){
       return 4;}
      else {
      return 1;}
    }
  }
}
int posMax3(int a,int b,int c){//return the position of the maximum between three ints (a return 1,b return 2 ...
  if (a<b){
    if (b<c){
    	return 3;
    }
    else{
    	return 2;
    }
  }
  else{
    if (a<c){
    	return 3;
    }
    else {
      return 1;
    }
  }
}
int posMax2(int a,int b){//return the position of the maximum between two ints (a return 1,b return 2)
  if (a<b) return 2;
  else return 1;
}
void order4(int* a,int* b,int* c,int* d){//put the max in a ,the second max in b ,the min in d.
  int temp1=(*a);
  int temp2=(*b);
  int temp3=(*c);
  int temp4=(*d);
  switch (posMax4(temp1,temp2,temp3,temp4))
  {
  case 1:// a was first,we store it in the first variable
    *a=temp1;
    switch (posMax3(temp2,temp3,temp4))
    {
    case 1:// b was first from three left,we store it in the second variable
      *b=temp2;
      if (posMax2(temp3,temp4)==1){
        *c=temp3;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp3;
      }
      break;
    case 2:// c was first from three left,we store it in the second variable
      *b=temp3;
      if (posMax2(temp2,temp4)==1){
        *c=temp2;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp2;
      }
      break;
    case 3:// a was first,we store it in the first variable
      *b=temp4;
      if (posMax2(temp2,temp3)==1){
        *c=temp2;
        *d=temp3;
      }
      else{
        *c=temp3;
        *d=temp2;
      }
      break;
    default:
      break;
    }
    break;
  case 2:// b was first,we store it in the first variable
    *a=temp2;
    switch (posMax3(temp1,temp3,temp4))
    {
    case 1:// b was first from three left,we store it in the second variable
      *b=temp1;
      if (posMax2(temp3,temp4)==1){
        *c=temp3;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp3;
      }
      break;
    case 2:// c was first from three left,we store it in the second variable
      *b=temp3;
      if (posMax2(temp1,temp4)==1){
        *c=temp1;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp1;
      }
      break;
    case 3:// a was first,we store it in the first variable
      *b=temp4;
      if (posMax2(temp1,temp3)==1){
        *c=temp1;
        *d=temp3;
      }
      else{
        *c=temp3;
        *d=temp1;
      }
      break;
    default:
      break;
    }
    break;
  case 3:// a was first,we store it in the first variable
    *a=temp3;
    switch (posMax3(temp1,temp2,temp4))
    {
    case 1:// b was first from three left,we store it in the second variable
      *b=temp1;
      if (posMax2(temp2,temp4)==1){
        *c=temp2;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp2;
      }
      break;
    case 2:// c was first from three left,we store it in the second variable
      *b=temp2;
      if (posMax2(temp1,temp4)==1){
        *c=temp1;
        *d=temp4;
      }
      else{
        *c=temp4;
        *d=temp1;
      }
      break;
    case 3:// a was first,we store it in the first variable
      *b=temp4;
      if (posMax2(temp1,temp2)==1){
        *c=temp1;
        *d=temp2;
      }
      else{
        *c=temp2;
        *d=temp1;
      }
      break;
    default:
      break;
    }
    break;
  case 4:// a was first,we store it in the first variable
    *a=temp4;
    switch (posMax3(temp1,temp2,temp3))
    {
    case 1:// b was first from three left,we store it in the second variable
      *b=temp1;
      if (posMax2(temp2,temp3)==1){
        *c=temp2;
        *d=temp3;
      }
      else{
        *c=temp3;
        *d=temp2;
      }
      break;
    case 2:// c was first from three left,we store it in the second variable
      *b=temp2;
      if (posMax2(temp1,temp3)==1){
        *c=temp1;
        *d=temp3;
      }
      else{
        *c=temp3;
        *d=temp1;
      }
      break;
    case 3:// a was first,we store it in the first variable
      *b=temp3;
      if (posMax2(temp1,temp2)==1){
        *c=temp1;
        *d=temp2;
      }
      else{
        *c=temp2;
        *d=temp1;
      }
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}
action randommove(tree map){//do a random move(not bombing)
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

action choose_Side(tree map,int explosion_range){
  int npath=count_Safe_Paths(map->n,1,explosion_range,false);
  int epath=count_Safe_Paths(map->e,2,explosion_range,false);
  int spath=count_Safe_Paths(map->s,3,explosion_range,false);
  int wpath=count_Safe_Paths(map->w,4,explosion_range,false);
  action a;
  if (isThereEnemies(map)){
    int temp1=npath;
    int temp2=epath;
    int temp3=spath;
    int temp4=wpath;
    order4(&temp1,&temp2,&temp3,&temp4);
    printf("temp1=%d temp2=%d temp3=%d temp4=%d\n npath=%d epath=%d spath=%d, wpath=%d\n",temp1,temp2,temp3,temp4,npath,epath,spath,wpath);
    if (temp1==npath){printf("ebemy at north\n");
        if (depthCharEnemy(map->n,0)<=6){
          
          if (temp2==spath && temp2 >=2){
            a=SOUTH;
            printf("at least 2 path at south\n");
          }
          else if (temp2==epath && temp2 >=2){
            a=EAST;printf("at least 2 path at east\n");
          }
          else if (temp2==wpath && temp2 >=2){
            a=WEST;
            printf("at least 2 path at west\n");
          }
          else {a=NORTH;
          printf("north at least:/ \n");}
        }
        else{switch (posMax4(npath,epath,spath,wpath))
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
        default:
          break;
        }}
    }
    else if (temp1==epath){
        if (depthCharEnemy(map->e,0)<=6){
          if (temp2==wpath && temp2 >=2){
            a=WEST;
            printf("at least 2 path at wesr\n");
          }
          else if (temp2==npath && temp2 >=2){printf("at least 2 path at north\n");
            a=NORTH;
          }
          else if (temp2==spath && temp2 >=2){printf("at least 2 path at south\n");
            a=SOUTH;
          }
          else{ a=EAST;
          printf("east at least:/ \n");
        }}
        else{switch (posMax4(npath,epath,spath,wpath))
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
        default:
          break;
        }
        }}
    else if (temp1==spath){  
      printf("ebemy at south\n");
        if (depthCharEnemy(map->s,0)<=6){
          if (temp2==npath && temp2 >=2){printf("at least 2 path at north\n");
            a=NORTH;
          }
          else if (temp2==epath && temp2 >=2){printf("at least 2 path at east\n");
            a=EAST;
          }
          else if (temp2==wpath && temp2 >=2){printf("at least 2 path at wesr\n");
            a=WEST;
          }
          else {a=SOUTH;
          printf("south at list");}
        }
        else{switch (posMax4(npath,epath,spath,wpath))
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
        default:
          break;
        }}
    }
    else { printf("ebemy at west\n");
        if (depthCharEnemy(map->w,0)<=6){
          if (temp2==epath && temp2 >=2){printf("at least 2 path at east\n");
            a=EAST;
          }
          else if (temp2==npath && temp2 >=2){printf("at least 2 path at north\n");
            a=NORTH;
          }
          else if (temp2==spath && temp2 >=2){printf("at least 2 path at south\n");
            a=SOUTH;
          }
          else{ a=WEST;
          printf("west at list");}
        }
    }
    //si on a un ennemi on préfère éviter ce coté et on prendds le chemin du dessous.Faire attention à la distance de l'ennemi si elle est trop grande!
  }
  else{
    
    switch (posMax4(npath,epath,spath,wpath))
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
    default:
      break;
    }
    
  }
  return a;
}

int count_Safe_Paths(tree smap,int idir,int dleft,bool count){//count the path available for a tree that aren't in bomb range(note: 1=NORTH,2=EAST...)
  if (smap!=0){
    if (IsAnEnemy(smap)) return 0;
    if (smap->c==PATH || smap->c==FLAME_BONUS ||smap->c==BOMB_BONUS){
      if (dleft<=0 && !count)
          count=true;
      if (count){
        return count_Safe_Paths(smap->n,idir,dleft,count)+count_Safe_Paths(smap->e,idir,dleft,count)+count_Safe_Paths(smap->s,idir,dleft,count)+count_Safe_Paths(smap->w,idir,dleft,count)+1;
      }
      else{
        
        switch(idir){
          case 1: return count_Safe_Paths(smap->n,idir,dleft-1,false)+count_Safe_Paths(smap->e,idir,dleft-1,true)+count_Safe_Paths(smap->s,idir,dleft-1,true)+count_Safe_Paths(smap->w,idir,dleft-1,true);
            break;
          case 2: return count_Safe_Paths(smap->n,idir,dleft-1,true)+count_Safe_Paths(smap->e,idir,dleft-1,false)+count_Safe_Paths(smap->s,idir,dleft-1,true)+count_Safe_Paths(smap->w,idir,dleft-1,true);
            break;
          case 3: return count_Safe_Paths(smap->n,idir,dleft-1,true)+count_Safe_Paths(smap->e,idir,dleft-1,true)+count_Safe_Paths(smap->s,idir,dleft-1,false)+count_Safe_Paths(smap->w,idir,dleft-1,true);
            break;
          case 4: return count_Safe_Paths(smap->n,idir,dleft-1,true)+count_Safe_Paths(smap->e,idir,dleft-1,true)+count_Safe_Paths(smap->s,idir,dleft-1,true)+count_Safe_Paths(smap->w,idir,dleft-1,false);
            break;
        }
      }
    }
  }
  return 0;
}

int isThereBombs(tree map){//return number indicating where is the bomb,-1 if there isn't any bomb(considering that the player only see one bomb because we won't put any bomb in game if we see one)
  if(sMapBomb(map->n)) return 1;
  else if(sMapBomb(map->e)) return 2;
  else if(sMapBomb(map->s)) return 3;
  else if(sMapBomb(map->w)) return 4;
  else return -1;
}

bool sMapBomb(tree map){//say if  there is a bomb in the tree
  if (map==0) return false;
  else if (map->c==BOMB) return true;
  else return (sMapBomb(map->n)||sMapBomb(map->e)||sMapBomb(map->s)||sMapBomb(map->w));
}

bool isThereWalls(tree map){
	return sMapWalls(map->n) || sMapWalls(map->e) || sMapWalls(map->s) || sMapWalls(map->w);
}

bool sMapWalls(tree map){
  if (map==0) return false;
  else if (map->c==BREAKABLE_WALL) return true;
  else return (sMapWalls(map->n)||sMapWalls(map->e)||sMapWalls(map->s)||sMapWalls(map->w));
}
bool isThereBonuses(tree map){
	return sMapBonuses(map->n) || sMapBonuses(map->e) || sMapBonuses(map->s) || sMapBonuses(map->w);
}

bool sMapBonuses(tree map){
  if (map==0) return false;
  else if (map->c==FLAME_BONUS ||map->c==BOMB_BONUS) return true;
  else return (sMapBonuses(map->n)||sMapBonuses(map->e)||sMapBonuses(map->s)||sMapBonuses(map->w));
}
bool isThereEnemies(tree map){//return if player see an enemy
  return sMapEnemy(map->n) || sMapEnemy(map->e) || sMapEnemy(map->s) || sMapEnemy(map->w);
}
bool sMapEnemy(tree map){
  if (map==0) return false;
  else if (IsAnEnemy(map->c)) return true;
  else return (sMapEnemy(map->n)||sMapEnemy(map->e)||sMapEnemy(map->s)||sMapEnemy(map->w));
}
action escapeBomb(tree map,int bpos,int explosion_range){//check if bomberman is threatened directly by a bomb.
  bool isthreatened;
  switch(bpos) {
    case 1:
      isthreatened =bombThreat(map->n,1,explosion_range);
      break;
    case 2:
      isthreatened =bombThreat(map->e,2,explosion_range);
      break;
    case 3:
      isthreatened =bombThreat(map->s,3,explosion_range);
      break;
    case 4:
      isthreatened =bombThreat(map->w,4,explosion_range);
      break;
  }
  action a;
  printf("threat?:%d",isthreatened);
  if (isthreatened){
    switch (bpos)
    {
    case 1:
      if ((map->e)->c==PATH){//si on a bien un chemin valide
        if (((map->e)->s)!=0){
          if ((((map->e)->s)->c==PATH)) a= EAST;
        }//si la case au sud est valide
        if (((map->e)->e)!=0){
          if ((((map->e)->e)->c==PATH)) a=EAST;
        }//si la case au sud est valide
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      }
      else if ((map->w)->c==PATH){//si on a bien un chemin valide
        if (((map->w)->s)!=0){
          if ((((map->w)->s)->c==PATH)) a=WEST;
        }//si la case au sud est valide
        if (((map->w)->w)!=0){
          if ((((map->w)->w)->c==PATH)) a=WEST;
        }}//si la case au sud est valide
      else a= SOUTH;//si les cases sur les cotés sont invalides(à noter:on peut perdre ici car pas de vérification)
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      
      break;
    case 2:
      if ((map->n)->c==PATH){//si on a bien un chemin valide
        if (((map->n)->w)!=0){
          if ((((map->n)->w)->c==PATH)) a=NORTH;
        }//si la case au sud est valide
        if (((map->n)->n)!=0){
          if ((((map->n)->n)->c==PATH)) a=NORTH;
        }//si la case au sud est valide
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      }
      else if ((map->s)->c==PATH){//si on a bien un chemin valide
        if (((map->s)->w)!=0){
          if ((((map->s)->w)->c==PATH)) a=SOUTH;
        }//si la case au sud est valide
        if (((map->s)->s)!=0){
          if ((((map->s)->s)->c==PATH)) a=SOUTH;
      }}//si la case au sud est valide
      else a=WEST;//si les cases sur les cotés sont invalides(à noter:on peut perdre ici car pas de vérification)
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      
      break;
    case 3:
      if ((map->e)->c==PATH){//si on a bien un chemin valide
        if (((map->e)->n)!=0){
          if ((((map->e)->n)->c==PATH)) a= EAST;
        }//si la case au sud est valide
        if (((map->e)->e)!=0){
          if ((((map->e)->e)->c==PATH)) a= EAST;
        }//si la case au sud est valide
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      }
      else if ((map->w)->c==PATH){//si on a bien un chemin valide
        if (((map->w)->n)!=0){
          if ((((map->w)->n)->c==PATH)) a= WEST;
        }//si la case au sud est valide
        if (((map->w)->w)!=0){
          if ((((map->w)->w)->c==PATH)) a= WEST;
        }}//si la case au sud est valide
      else a=NORTH;//si les cases sur les cotés sont invalides(à noter:on peut perdre ici car pas de vérification)
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      
      break;
    case 4:
      if ((map->n)->c==PATH){//si on a bien un chemin valide
        if (((map->n)->e)!=0){
          if ((((map->n)->e)->c==PATH)) a= NORTH;
        }//si la case au sud est valide
        if (((map->n)->n)!=0){
          if ((((map->n)->n)->c==PATH)) a=NORTH;
        }//si la case au sud est valide
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      }
      else if ((map->s)->c==PATH){//si on a bien un chemin valide
        if (((map->s)->e)!=0){
          if ((((map->s)->e)->c==PATH)) a=SOUTH;
        }//si la case au sud est valide
        if (((map->s)->s)!=0){
          if ((((map->s)->s)->c==PATH)) a=SOUTH;
        }}//si la case au sud est valide
      else a=EAST;//si les cases sur les cotés sont invalides(à noter:on peut perdre ici car pas de vérification)
        //note: on pourrait revenir au nord ,mais c'est complexe à implémenter pour quelque chose de peut être pas nécessaire
      
      break;
    default:
      break;
    }
    return a;
  }
  else return randomSafeMove(map,bpos,explosion_range);
}
bool bombThreat(tree map,int dir,int dleft){//check if we are directly threat by a bomb in the tree.
  bool ret=false;
  //printf("dir :%d\n",dir);
  if (dleft<=0) ret=false;
  else if (map==0) ret=false;
  else if (map->c==BOMB) ret=true;
  else {
    switch (dir)
    {
    case 1:
      ret=bombThreat(map->n,dir,dleft-1);
      break;
    case 2:
      ret=bombThreat(map->e,dir,dleft-1);
      break;
    case 3:
      ret=bombThreat(map->s,dir,dleft-1);
      break;
    case 4:
      ret=bombThreat(map->w,dir,dleft-1);
      break;
    default:
      break;
    }
  }
  return ret;
}
int* posBomb(tree map,int* pos){
  if (pos[2]!=1){
    if (map==0) {
    return pos;}
    else if (map->c==BOMB){
      
      pos[2]=1;
      
      return pos;
    }
    else{
    	
      if (map->c==PATH){
        int npos[3]={-1,0,0};
        int epos[3]={0,1,0};
        int spos[3]={1,0,0};
        int wpos[3]={0,-1,0};
        addTab(pos,npos,3);
        addTab(pos,epos,3);
        addTab(pos,spos,3);
        addTab(pos,wpos,3);
        copyTab(posBomb(map->n,npos),npos,3);
        copyTab(posBomb(map->e,epos),epos,3);
        copyTab(posBomb(map->s,spos),spos,3);
        copyTab(posBomb(map->w,wpos),wpos,3);
       
        if (npos[2]==1) {
        	pos[0]=npos[0];
        	pos[1]=npos[1];
        	pos[2]=1;
        	
        	return pos;
        }
        else if (epos[2]==1) {
        	pos[0]=epos[0];
        	pos[1]=epos[1];
        	pos[2]=1;
        	
        	return pos;
        }
        else if (spos[2]==1) {
        	pos[0]=spos[0];
        	pos[1]=spos[1];
        	pos[2]=1;
        	
        	return pos;
        }
        else if (wpos[2]==1) {
        	pos[0]=wpos[0];
        	pos[1]=wpos[1];
        	pos[2]=1;
        	
        	return pos;
        }
        else return pos;
      }
      else return pos;
    }
  }
  return pos;
}
action randomSafeMove(tree map,int bdir,int explosion_range){//return a random safemove.
  int bpos[3]={0,0,0};//x,y,found
  switch (bdir)
  {
  case 1:
    bpos[0]-=1;//car on va vers le nord
    copyTab(posBomb(map->n,bpos),bpos,3);
    break;
  case 2:
    bpos[1]+=1;//car on va vers le nord
    
    copyTab(posBomb(map->e,bpos),bpos,3);
    break;
  case 3:
    bpos[0]+=1;//car on va vers le nord
    copyTab(posBomb(map->s,bpos),bpos,3);
    break;
  case 4:
    bpos[1]-=1;//car on va vers le nord
    copyTab(posBomb(map->w,bpos),bpos,3);
    break;  
  default:
    break;
  }
  action a;
  bool choose=false;
  //printf("bpos : (%d,%d)",bpos[0],bpos[1]);
  //try and verify if north is valid
  if (map->n != 0){
    if ((map->n)->c ==PATH){
      if (!((bpos[0]+1==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]==0) && (abs(bpos[0])<=explosion_range))){
      	//printf("north? %d %d",bpos[0],bpos[1]);
      	a=NORTH;//if north move don't verify "go on the same line than the bomb and diff of col put bomberman in danger" we do it.
      	choose=true;
      	}
    }
  } 
  if (!choose){
  	//try and verify if east is valid
	  if (map->e != 0){
	    if ((map->e)->c ==PATH){
	      if (!((bpos[0]==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]-1==0) && (abs(bpos[0])<=explosion_range))){
	      	a= EAST;//if east move don't put bomberman in danger,we do it
	      	//printf("east? %d %d",bpos[0],bpos[1]);
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose){
	  if (map->s != 0){
	    if ((map->s)->c ==PATH){
	      if (!((bpos[0]-1==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]==0) && (abs(bpos[0])<=explosion_range))){
	      	a= SOUTH;//if east move don't put bomberman in danger,we do it
	      	//printf("south? %d %d",bpos[0],bpos[1]);
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose){
	  if (map->w != 0){
	    if ((map->w)->c ==PATH){
	      if (!((bpos[0]==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]+1==0) && (abs(bpos[0])<=explosion_range))){
	      	a=WEST;//if east move don't put bomberman in danger,we do it
	      	//printf("west? %d %d",bpos[0],bpos[1]);
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose){ a= randommove(map);//if it don't work,we return at list a random move
  printf("uh? \n");
  }
  printAction(a);
  return a;
}
void addTab(int* tab1,int* tab2,int taille){//add the first tab in the second tab(el 1+ el2 =el returned)
  for (int i=0;i<taille;i++){
    tab2[i]+=tab1[i];
  }
}
void copyTab(int* tab1,int* tab2,int taille){//copythe first tab in the second tab(el 1+ el2 =el returned)
  for (int i=0;i<taille;i++){
    tab2[i]=tab1[i];
  }
}
int NearEnemy(tree map){//react if an ennemy is close to us(1 to 4 ->actions 5 boming 6 none)
  if (IsAGhost(map->n) || IsAGhost(map->e) || IsAGhost(map->s) || IsAGhost(map->w) ) return escapeGhost(map);//run
  else if ((map->n)->c==FLAME_ENEMY || (map->e)->c==FLAME_ENEMY || (map->s)->c==FLAME_ENEMY || (map->w)->c==FLAME_ENEMY ) return 5;//bomb
  else {
    /////////////////////////////////////////////////////north
    if ((map->n)->n!=0){
      if (IsAnEnemy((map->n)->n)) return 5;//bomb if we are at a distance of 2 from an enemy 
    } 
    if ((map->n)->e!=0){
      if (IsAnEnemy((map->n)->e)) return 5;
    } 
    if ((map->n)->w!=0){
      if (IsAnEnemy((map->n)->w)) return 5; 
    } 
    ///////////////////////////////////////////////////////east
    if ((map->e)->e!=0){
      if (IsAnEnemy((map->e)->e)) return 5;
    } 
    if ((map->e)->n!=0){
      if (IsAnEnemy((map->e)->n)) return 5;
    } 
    if ((map->e)->s!=0){
      if (IsAnEnemy((map->e)->s)) return 5;
    }
    //////////////////////////////////////////////////////south 
    if ((map->s)->s!=0){
      if (IsAnEnemy((map->s)->s)) return 5;
    } 
    if ((map->s)->e!=0){
      if (IsAnEnemy((map->s)->e)) return 5;
    } 
    if ((map->s)->w!=0){
      if (IsAnEnemy((map->s)->w)) return 5;
    } 
    //////////////////////////////////////////////////////west
    if ((map->w)->w!=0){
      if (IsAnEnemy((map->w)->w)) return 5;
    } 
    if ((map->w)->n!=0){
      if (IsAnEnemy((map->w)->n)) return 5;
    } 
    if ((map->w)->s!=0){
      if (IsAnEnemy((map->w)->s)) return 5;
    } 
  }
  return 6;
}
bool IsAnEnemy(tree map){//say if it's an enemy or not(here for a shorter syntax)
  if (map->c==GHOST_ENEMY || map->c==FLAME_ENEMY || map->c=='/') return true;// '/' represent a ghost on a wall.We Want to see it.
  return false;
}
bool IsAGhost(tree map){//say if it's a ghost or not(here for a shorter syntax)
  if (map->c==GHOST_ENEMY || map->c=='/') return true;// '/' represent a ghost on a wall.We Want to see it.
  return false;
}

int escapeGhost(tree map){
  if (IsAGhost(map->n)){
    if (map->s!=0){
      if ((map->s)->c==PATH || (map->s)->c==FLAME_BONUS || ((map->s)->c==BOMB_BONUS)) return 3;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->e!=0){
      if ((map->e)->c==PATH || (map->e)->c==FLAME_BONUS || ((map->e)->c==BOMB_BONUS)) return 2;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->w!=0){
      if ((map->w)->c==PATH || (map->w)->c==FLAME_BONUS || ((map->w)->c==BOMB_BONUS)) return 4;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    return 1;//else we go on the ghost to exploit the fact that enemies are computed in the same time than player(it won't kill us) 
  }
  if (IsAGhost(map->e)){
    if (map->w!=0){
      if ((map->w)->c==PATH || (map->w)->c==FLAME_BONUS || ((map->w)->c==BOMB_BONUS)) return 4;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->n!=0){
      if ((map->n)->c==PATH || (map->n)->c==FLAME_BONUS || ((map->n)->c==BOMB_BONUS)) return 1;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->s!=0){
      if ((map->s)->c==PATH || (map->s)->c==FLAME_BONUS || ((map->s)->c==BOMB_BONUS)) return 3;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    return 2;//else we go on the ghost to exploit the fact that enemies are computed in the same time than player(it won't kill us) 
  }
  if (IsAGhost(map->s)){
    if (map->n!=0){
      if ((map->n)->c==PATH || (map->n)->c==FLAME_BONUS || ((map->n)->c==BOMB_BONUS)) return 1;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->e!=0){
      if ((map->e)->c==PATH || (map->e)->c==FLAME_BONUS || ((map->e)->c==BOMB_BONUS)) return 2;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->w!=0){
      if ((map->w)->c==PATH || (map->w)->c==FLAME_BONUS || ((map->w)->c==BOMB_BONUS)) return 4;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    return 3;//else we go on the ghost to exploit the fact that enemies are computed in the same time than player(it won't kill us) 
  }
  if (IsAGhost(map->w)){
    if (map->e!=0){
      if ((map->e)->c==PATH || (map->e)->c==FLAME_BONUS || ((map->e)->c==BOMB_BONUS)) return 2;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->n!=0){
      if ((map->n)->c==PATH || (map->n)->c==FLAME_BONUS || ((map->n)->c==BOMB_BONUS)) return 1;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    if (map->s!=0){
      if ((map->s)->c==PATH || (map->s)->c==FLAME_BONUS || ((map->s)->c==BOMB_BONUS)) return 3;//we choose the oposite direction in priority beacause enemy are most supposed to turn when  there is an multiple path around him
    }
    return 4;//else we go on the ghost to exploit the fact that enemies are computed in the same time than player(it won't kill us) 
  }
  printf("anormal behaviour:use bombing \n");
  return 5;//never get here because we call a fonction if there is a ghost near us.
}
