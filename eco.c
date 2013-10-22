#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *Brent Ryczak
 */

/*
 *these boolean values are defined to determine if a creature's diet 
 *includes fish, plant, or both.
 */

typedef int boolean;
#define true 1;
#define false 0;
int psuedoRandomNumber = 0; 


//diet is the deciding factor of what the creature can eat. There are two 
//possible options that the creature may eat; 1) fish, 2) plants. It is possible
//for each item to be in the creatures diet.
//
//creatures who eat fish must satisfy two conditions. 1) the creature must contain a larger
//aggressionLevel its "opponent". 
struct diet
{
  boolean fish;
  boolean plant; 
};
typedef struct diet diet;

/*
aggressionLevel: how aggressive the creature is begins on a 0-5 scale. 
Aggression iscompared to another creature. The creature with the lower aggressionLevel 
is consumed by the creature with the larger aggressionLevel. A tie is resolved randomly.

worth: the worth of a creature is how much health the creature provides to the winner
if the creature has lost a fight, before the creature is eliminated from the ecosystem.

health: health is determined based on the creatures specific type. Once a creature's health
becomes =< 0, the creature is eliminated.

creature spawn chart (update as necessary):
*/
/*Creature      Health      Aggression 
 *
 * Musky          10            5 
 * Pike           08            4
 * Bass           06            3
 * Minnow         02            1
 * 
 * Seaweed        01            0
 * Algae          00            0
*/
struct type
{
  int aggressionLevel, worth, health;
  diet diet;
  int typeOp;
};
typedef struct type type;

//creature is the structure that makes up an individual "creature" in the ecosystem.
//to create a creature of any type, one must create a creature, and assign it to its desired
//type.
//
//type: dictates what the creature actually is (view creature spawn chart above).
//
//id: each creature contains an unique ID, which is how the creature can be referenced
//throughout its entire life. The id is guarenteed not to change. 
//
//inBattle: a 'boolean' type (truly an integer representing a 1 or 0). If the creature's 
//inBattle boolean (int) is set to true (1), the creature cannot be attacked by any other
//creatures besides it's current opponent.  
struct creature
{
  type type;
  int id, x, y;
  boolean inBattle;
};
typedef struct creature creature;

struct ecosystem
{  
  // identifier is an int that will ID each creature. Unlike creatureCount,
  // identifier will never decrement. This purpose is to prevent overlapping of
  // creature's id's (specifically when a creature is removed, and the creatureCount
  // is decremented. 
  int x, y, size, creatureMaxCount, creatureCount, identifier;
  creature **creaturesp;
  char name[12];
  int **graph;
};
typedef struct ecosystem ECO;



/*----------ECOSYSTEM FUNCTIONS-----------*/



void createEcosystem(ECO* eco)
{ 
  /* get ecosystem name and parameters from user, to create a new ecosystem. */
  int x, y;
  printf("Enter name of ecosystem: ");
  scanf("%s", eco->name);
  printf("Enter diameters of 2D ecosystem %s:", eco->name);
  scanf("%d%d", &x, &y);
  eco->x = x;
  eco->y = y; 
  
   /* total size of ecosystem. this limits the amount of possible 
    * creatures allowed in the ecosystem.
    * total amount of creatures = x * y
   */ 
  eco->size = x*y; 
  
  eco->creaturesp = malloc(sizeof(creature*)*99);
  eco->identifier = 0;
  eco->creatureCount = 0;
  eco->creatureMaxCount = x*y;
  
  /* initialize graph  */
  eco->graph = malloc(sizeof(int*)*x);
  int i, j;
  for(i = 0; i < x; i++){
    eco->graph[i] = malloc(sizeof(int)*y);
  }
  for(i = 0; i < x; i++){
    for(j = 0; j < y; j++){
      eco->graph[i][j] = -1;
    }
  }
}

//returns total number of creatures in eco
int countCreatures(ECO *eco)
{
  return eco->creatureCount;
}


void setRandomCoords(ECO *ep, creature *cp)
{
  int x = randomNum(ep->x);
  int y = randomNum(ep->y);
  while(ep->graph[x][y] != -1){
    x = randomNum(ep->x);
    y = randomNum(ep->y);
  }
  cp->x = x;
  cp->y = y;
  ep->graph[x][y] = getCreatureID(cp); 
}

//temporary function returns a psuedo random number.
int randomNum(int max)
{
  srand(time(0));
  return rand() % max;
}

//adds a creature to the passed in ecosystem.
void addCreature(ECO *eco, creature *cp)
{
  //max creature allowance check.
  if(eco->creatureCount == eco->creatureMaxCount){
    printf("Cannot add creature. Maximum exceeded.\n");
    return;
  }

  //this caps at 99, realloc() not functioning properly.
  cp->id = eco->identifier;
  eco->identifier++;
  eco->creaturesp[eco->creatureCount] = (cp);
  eco->creatureCount++;
  setRandomCoords(eco, cp);
}

void removeCreature(ECO *eco, creature *cp)
{
  //TODO
  eco->creatureCount--;
}

void creatureBattle(ECO *ep, creature *cp1, creature *cp2)
{
  cp1-> inBattle = true;
  cp2-> inBattle = true;


}

//prints the entire status of the passed in ecosystem to the console.
void printEcoState(ECO *eco)
{  
  printf("\n\nname of ecosystem: %s\n",eco->name);
  printf("%s's dimensions: %d x %d\n",eco->name, eco->x, eco->y);
  printf("total ecosystem size: %d\n", eco->size);
  printf("total creatures in ecosystem: %d\n", countCreatures(eco));
  printf("\n\n");
}

void printGraph(ECO *eco)
{
  int i, j;
  for(i = 0; i < eco->x; i++){
    for(j = 0; j < eco->y; j++){
      printf("%d ",eco->graph[i][j]);
    }
    printf("\n");
  }
}

void printCreatures(ECO *eco)
{
  //initialize i @ 1, because creatures[0] is the beginning of the array.
  int i;
  for(i = 0; i < eco -> creatureCount; i++)
    {
      printf("creature %d: OP: %d\n", eco->creaturesp[i]->id, 
	     eco->creaturesp[i]->type.typeOp); 
    }
  printf("\n");
}



/*----------CREATURE CREATION FUNCTIONS-----------*/



void makeType(creature *cp, int type, int agg, int worth, int health, boolean fish, boolean plant)
{
  cp-> type.typeOp = type;
  cp-> type.aggressionLevel = agg;
  cp-> type.worth = worth; //FIXME
  cp-> type.health = health;
  cp-> type.diet.fish = fish;
  cp-> type.diet.plant = plant;
}


//accepts a creature and a string representation of a creature to be created.
//if the string is a valid creature in this ecosystem, turn creature into that 
//specific fish. 

//if the string respresentation is NULL or a name that is not accepted, a random creature
//will be generated.
void buildCreature(creature *cp, int op)
{

  //OP codes:
  // Musky   :0
  // Pike    :1
  // Bass    :2
  // Minnow  :3
  // Seaweed :4
  // Algae   :5


 if(op == 0) makeType(cp, 0, 5, 0, 10, 1, 0); //Musky
 else if(op == 1) makeType(cp, 1, 4, 0, 8, 1, 0); //Pike
 else if(op == 2) makeType(cp, 2, 3, 0, 6, 1, 1); //Bass
 else if(op == 3) makeType(cp, 3, 1, 0, 2, 1, 1); //Minnow
 else if(op == 4) makeType(cp, 4, 0, 0, 1, 0, 0); //Seaweed
 else if(op == 5) makeType(cp, 5, 0 ,0, 1, 0, 0); //Algae
 else{
   
 }
}

//this function creates a skeleton of a new creature, and returns its pointer
//the creature type is chosen at random, randomly generating a random typeOp code
creature* randomCreature(){

  //random seed initialized to 0
  //srand(time(0));
  //generate op number
  int op = randomNum(4); 
  creature *cp = malloc(sizeof(creature));
  buildCreature(cp, op);
  return cp;
}

//accepts one parameter, strp, which is a string representation of the creatures.
//If spelled correctly, the corresponding typeOp is returned. 
int getCreatureCode(char *strp)
{
  if(strcmp(strp, "Musky")== 0) return 0;
  if(strcmp(strp, "Pike")== 0) return 1;
  if(strcmp(strp, "Bass")== 0) return 2;
  if(strcmp(strp, "Minnow")== 0) return 3;
  if(strcmp(strp, "Seaweed")== 0) return 4;
  if(strcmp(strp, "Algae") == 0) return 5;
  
  //String name doesnt match.
  return -1;
}



/*----------CREATURE SPECIFIC FUNCTIONS-----------*/



//returns the unique int ID of the passed in creature.
int getCreatureID(creature *cp)
{
  return cp->id;
}

//returns the int typeOp of the passed in creature.
int getCreatureTypeOp(creature *cp)
{
  return cp->type.typeOp;
}



/*----------MAIN FUNCTION-----------*/



int main()
{
  ECO *ep = malloc(sizeof(ECO));
  createEcosystem(ep); 
  
  int i;
  for(i = 0; i < 3; i++){
    creature *c;
    c = randomCreature();
    addCreature(ep, c);
  }
  printCreatures(ep);
  //printEcoState(ep);
   
  //printGraph(ep);
  return 0;
}
