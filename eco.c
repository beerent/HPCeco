#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

#define PORT 9999;


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
  int sockfd, n;
  struct sockaddr_in serv_addr;
};
typedef struct ecosystem ECO;


/*----------PROGRAMMER FUNCTIONS----------*/

void report(char *msg)
{
  printf("C: %s\n", msg);
}

void reportData(char *msg, char *data)
{
  printf("C: %s: %s\n", msg, data);
}

int intLen(int i)
{
  char testing[100];
  sprintf(testing,"%d",i);
  return strlen(testing);
}

void sendToServer(ECO *ep, char *msg, int len)
{
  ep->n = write(ep->sockfd, msg, len); 
}
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
  
  eco->creaturesp = malloc(sizeof(creature*)*eco->size);
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

creature* getCreatureByID(ECO *ep, int id){
  int i;
  for(i = 0; i < ep->creatureCount; i++){
    if(ep->creaturesp[i]->id ==  id){
      return ep->creaturesp[i];
    }
  }
  return NULL;
}

creature* getCreatureByCoords(ECO *ep, int x, int y)
{
  int id = ep->graph[x][y];
  int i;
  for(i = 0; i < ep->creatureCount; i++){
    if(ep->creaturesp[i]->id ==  id){
      return ep->creaturesp[i];
    }
  }
  return NULL;
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

void removeCreature(ECO *ep, creature *cp)
{
  //TODO
  int i;
  for(i = 0; i < ep->size; i++){
    if(cp->id == ep->creaturesp[i]->id){
      //creature to be removed
      ep->graph[ep->creaturesp[i]->x][ep->creaturesp[i]->y] = -1;
      for(i; i < ep->creatureCount; i++){
	ep->creaturesp[i] = ep->creaturesp[i+1];
      }
      break;
    }
  }
  ep->creatureCount--;
}

void creatureBattle(ECO *ep, creature *cp1, creature *cp2)
{
  cp1-> inBattle = true;
  cp2-> inBattle = true;
  
  if(cp1->type.aggressionLevel > cp2->type.aggressionLevel){
    printf("CREATURE %d CONSUMES CREATURE %d\n", cp1->id, cp2->id);
    removeCreature(ep, cp2);
    cp1-> inBattle = false;
  }else if(cp2->type.aggressionLevel > cp1->type.aggressionLevel){
    printf("CREATURE %d CONSUMES CREATURE %d\n", cp2->id, cp1->id);
    removeCreature(ep, cp1);
    cp2-> inBattle = false;
  }else{
    if(randomNum(2) == 1){
      printf("CREATURE %d CONSUMES CREATURE %d\n", cp1->id, cp2->id);
      removeCreature(ep, cp2);
      cp1->inBattle = false;
    }else{
      printf("CREATURE %d CONSUMES CREATURE %d\n", cp2->id, cp1->id);
      removeCreature(ep, cp1);
      cp2->inBattle = false;
    }
  }
}

//prints the entire status of the passed in ecosystem to the console.
  void printEcoState(ECO *eco)
  {  
    printf("name of ecosystem: %s\n",eco->name);
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
        if((sizeof(eco->graph[i][j]) / sizeof(int)) == 1) printf(" %d ",eco->graph[i][j]);
        else if((sizeof(eco->graph[i][j]) / sizeof(int)) == 2) printf("%d ", eco->graph[i][j]);
        else printf("%d", eco->graph[i][j]);
      }
      printf("\n");
    }
    printf("\n\n");
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

  int op = randomNum(6); 
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

void generateRandomCreatures(ECO *ep, int max)
{
  int i;
  for(i = 0; i < max; i++){
    creature *c;
    c = randomCreature();
    addCreature(ep, c);
  }
}

int openSocket(ECO *ep)
{
  //int sockfd = 0,n = 0;
  char recvBuff[1024];
  
  memset(recvBuff, '0' ,sizeof(recvBuff));
  if((ep->sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("\n Error : Could not create socket \n");
    return 1;
  }
  
  ep->serv_addr.sin_family = AF_INET;
  ep->serv_addr.sin_port = htons(9999); //defualt port set on the javaGraphicsPortal 
  ep->serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //localhost
  
  //check if connection fails
  if(connect(ep->sockfd, (struct sockaddr *)&ep->serv_addr, sizeof(ep->serv_addr))<0){
    printf("\n Error : Connect Failed \n");
    return 1;
  }
  
  //here, we are connected to the server

/* Write a response to the client */
  ep->n = write(ep->sockfd, "clientKEY\n",11);  
  ep->n = read(ep->sockfd, recvBuff, sizeof(recvBuff)-1);
  recvBuff[ep->n] = 0;

  /* Handshake Complete */
  report("connection established");

  return 0; 
}

void sendEcoBounds(ECO *ep)
{
  //get x and y to send to server
  int numLenX = intLen(ep->x);
  int numLenY = intLen(ep->y);

  //default to 20 for now
  char Xout[20];
  char Yout[20];

  sprintf(Xout, "%d\n", ep->x);
  sprintf(Yout, "%d\n", ep->y);
  ep->n = write(ep->sockfd, Xout, numLenX +2);
  ep->n = write(ep->sockfd, Yout, numLenY +2); 
  ep->n = write(ep->sockfd, strcat(ep->name, "\n"), strlen(ep->name)+2);
}

int countCreatureCoords(ECO *ep)
{
  int i;
  int count = 0;
  char numBuf[10];
  for(i = 0; i < ep -> creatureCount; i++)
    {
      sprintf(numBuf, "%d%d%d", ep->creaturesp[i]->id, ep->creaturesp[i]->x, ep->creaturesp[i]->y); 
      printf("adding :%d\n", strlen(numBuf));
      count += strlen(numBuf);
    }
  printf("returning %d\n", count);
  return count;
}

void sendCurrentEcoState(ECO *ep)
{
  int i;
  char charBuf[10];

  sendToServer(ep, "IN\n", 4);

  for(i = 0; i < ep->creatureCount; i++){
    sprintf(charBuf, "%d %d %d", ep->creaturesp[i]->id, ep->creaturesp[i]->x, ep->creaturesp[i]->y);
    sendToServer(ep, charBuf, strlen(charBuf));
    sendToServer(ep, "\n", 2);
  }

    sendToServer(ep, "OUT\n", 5);
  /*
  int i;
  char charBuf[countCreatureCoords(ep) + (countCreatureCoords(ep)*3)];
  sprintf(charBuf, "%d*%d*%d*", ep->creaturesp[0]->id, ep->creaturesp[0]->x, ep->creaturesp[0]->y);
  for(i = 1; i < ep->creatureCount; i++){
    sprintf(charBuf, "%s%d*%d*%d*", charBuf, ep->creaturesp[i]->id, ep->creaturesp[i]->x, ep->creaturesp[i]->y);
  }

  printf("***%s\n", charBuf);
  sendToServer(ep, "0\n", 3);
  sendToServer(ep, charBuf, strlen(charBuf));
  sendToServer(ep, "\n", 2); 
  */
}

void printCommands()
{
  printf("1) add random creature\n");
  printf("2) send eco state to graphics server\n");
  printf("3) exit\n");
}

void handleMasterInput(int input, ECO *ep)
{
  if(input == 1){
    generateRandomCreatures(ep, 1);
    printEcoState(ep);
    printGraph(ep); 
  }else if(input == 2){
    sendCurrentEcoState(ep);
  }else if(input == 3){
    sendToServer(ep, "done\n",6);
    exit(0);
  }else{
    printf("invalid command: %d\n", input);
  }
}

void manuallyControlEco(ECO *ep)
{
  int inEco = 1;
  int input;
  while(inEco == 1){
    printf("Enter command:\n");
    printCommands();
    printf("--> ");
    scanf("%d", &input);
    handleMasterInput(input, ep);
  }
}


/*----------MAIN FUNCTION-----------*/


int main()
{  
  /* create ecosystem */
  ECO *ep = malloc(sizeof(ECO));
  createEcosystem(ep);

  /* networking with graphics server */
  int i = openSocket(ep);
  sendEcoBounds(ep);

  /* run ecosystem */
  manuallyControlEco(ep);

  /* testing code*/

  //createEcosystem(ep);
  /*
  
  generateRandomCreatures(ep, 5);

  printGraph(ep);
  printEcoState(ep);

  creatureBattle(ep, getCreatureByID(ep, 0), getCreatureByID(ep, 1));

  printGraph(ep);
  printEcoState(ep);
  */
  return 0;
}
