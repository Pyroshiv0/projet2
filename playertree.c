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
int posmin4(int,int,int,int);
int posmax4(int,int,int,int);
action randommove(tree);
int depthChar(tree,int,char);
int GoNearWalls(tree);
action choose_Side(tree,int);
int count_Safe_Paths(tree,int,int,bool);
int IsthereBombs(tree);
bool sMapBomb(tree);
bool IsthereWalls(tree);
bool sMapWalls(tree);
action escapeBomb(tree,int,int);
bool bombThreat(tree,int,int);
int* posBomb(tree,int*);
action randomSafeMove(tree,int,int);
void addTab(int*,int*,int);
void copyTab(int*,int*,int);
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
  	printf("called ChooseSide\n");}
  else{
    int bomb=IsthereBombs(map);
    if (bomb==-1) {
      if (IsthereWalls(map)){
	      int acttodo=GoNearWalls(map);
	      printf("Go Near Walls\n");
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
		  printf("called randomMove\n");
		  break;
		 
		}
      	  }
      else a=randommove(map);
      }
      
    else {a=escapeBomb(map,bomb,explosion_range);
    	  printf("called escapeBomb\n");
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
      printf("north is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case EAST:
    if ((map->e)->c==BREAKABLE_WALL||(map->e)->c==WALL ||(map->e)->c==BOMB){
      printf("east is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case SOUTH:
    if ((map->s)->c==BREAKABLE_WALL||(map->s)->c==WALL ||(map->s)->c==BOMB){
      printf("south is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  case WEST:
    if ((map->w)->c==BREAKABLE_WALL||(map->w)->c==WALL ||(map->w)->c==BOMB){
      printf("west is deadlygo randomMove");
      return SafeLook(map,randommove(map));
    }
    break;
  default:
  	printf("NO Action?go randomMove");
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
int posmax4(int a,int b,int c,int d){//return the position of the maximum between four ints (a return 0,b return 1 ...
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
  switch (posmax4(npath,epath,spath,wpath))
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
  return a;
}

int count_Safe_Paths(tree smap,int idir,int dleft,bool count){//count the path available for a tree that aren't in bomb range(note: 1=NORTH,2=EAST...)
  if (smap!=0){
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

int IsthereBombs(tree map){//return number indicating where is the bomb,-1 if there isn't any bomb(considering that the player only see one bomb because we won't put any bomb in game if we see one)
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

bool IsthereWalls(tree map){
	return sMapWalls(map->n) || sMapWalls(map->e) || sMapWalls(map->s) || sMapWalls(map->w);
}

bool sMapWalls(tree map){
  if (map==0) return false;
  else if (map->c==BREAKABLE_WALL) return true;
  else return (sMapWalls(map->n)||sMapWalls(map->e)||sMapWalls(map->s)||sMapWalls(map->w));
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
  //try and verify if north is valid
  if (map->n != 0){
    if ((map->n)->c ==PATH){
      if (!((bpos[0]-1==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]==0) && (abs(bpos[0])<=explosion_range))){
      	a=NORTH;//if north move don't verify "go on the same line than the bomb and diff of col put bomberman in danger" we do it.
      	choose=true;
      	}
    }
  } 
  if (!choose){
  	//try and verify if east is valid
	  if (map->e != 0){
	    if ((map->e)->c ==PATH){
	      if (!((bpos[0]==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]+1==0) && (abs(bpos[0])<=explosion_range))){
	      	a= EAST;//if east move don't put bomberman in danger,we do it
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose){
	  if (map->s != 0){
	    if ((map->s)->c ==PATH){
	      if (!((bpos[0]+1==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]==0) && (abs(bpos[0])<=explosion_range))){
	      	a= SOUTH;//if east move don't put bomberman in danger,we do it
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose){
	  if (map->w != 0){
	    if ((map->w)->c ==PATH){
	      if (!((bpos[0]==0) && (abs(bpos[1])<=explosion_range)) && !((bpos[1]-1==0) && (abs(bpos[0])<=explosion_range))){
	      	a=WEST;//if east move don't put bomberman in danger,we do it
	      	choose=true;
	      	}
	    } 
	  }
  }
  if (!choose) a= randommove(map);//if it don't work,we return at list a random move
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
