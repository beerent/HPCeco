#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  int x;
  int y;
} grid;

// In interface
GridNode **grid;

// In init
NSInteger nx = inFrame.size.width / blockSize;
NSInteger ny = inFrame.size.height / blockSize;
grid = malloc(sizeof(GridNode) * nx * ny);
grid[10][20].numActors = 3; // EXC_BAD_ACCESS
