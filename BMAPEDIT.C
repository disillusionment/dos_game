#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define mapsize  0x1000
#define ESC      27

int YDS = 0,YDE = 16;
int CPX = 1,CPY = 1;
int mapdata[mapsize];
int TXTCOLOR;
int x,y;
FILE *map;

void GetMapName(char *MapName) {
   printf("\nPlease enter the name of the map you would like to edit : ");
   scanf("%s",MapName);
   printf("\n%s",MapName);
}

// create an empty map if one doesn't exist
void MapCreate(void) {
   int index;
   for (index = 0; index < mapsize; index++) {
      mapdata[index] = 0;
   }
}

void ReadMap(char *MapName) {
   int nul,index;
   printf("Reading in mapfile. %s \n",MapName);
   if((map = fopen(MapName, "r")) == NULL) {
      printf("No map found.\n");
      MapCreate();
  } else {
     for (index = 0; index < mapsize; index++) {
        mapdata[index] = getc(map);
     }
  }
  fclose(map);
}
//END MAP READ

void SaveMap(char *MapName) {
   int index;
   printf("Saving mapfile.\n");
   map = fopen(MapName, "w");
   for (index = 0; index < mapsize; index++) {
      fputc(mapdata[index],map);
    }
  fclose(map);
}

void DisplayMap(int Y_Disp_Start,int Y_Disp_End) {
   char PutIt;
   gotoxy(1,1);

//printf("\n");
//clrscr();

for (y = Y_Disp_Start; y < Y_Disp_End; y++)
  {
    for (x = 0; x < 64; x++)
    {
/*    switch(MAP[x][y]%16)
      {
    case 0:
     TXTCOLOR = GREEN;
     PutIt = '�';
    break;
    case 1:
     TXTCOLOR = BLUE;
     PutIt = '�';
    break;
    case 2:
     TXTCOLOR = RED;
     PutIt = '�';
    break;
    case 3:
     TXTCOLOR = BROWN;
     PutIt = '�';
    break;
    case 4:
     TXTCOLOR = DARKGRAY;
     PutIt = '�';
    break;
    case 5:
     TXTCOLOR = MAGENTA;
     PutIt = '�';
    break;
    case 7:
     TXTCOLOR = GREEN;
     PutIt = '/';
    break;

    case 9:
     TXTCOLOR = GREEN;
     PutIt = '\\';
    break;

    };
*/
  TXTCOLOR = mapdata[x + y * 64]%16;
  PutIt = 219;
    if(x == CPX & y == CPY) TXTCOLOR = BLINK+TXTCOLOR;
    textcolor(TXTCOLOR);
       putch(PutIt);
    }
    printf("\n");
  }
}

void ChangeMap(void) {
   scanf("%d",&mapdata[CPX + CPY * 64]);
}

void ReadKeyboard(char *MapName) {

  char KeyBoardIn=getch();
  switch (KeyBoardIn)
    {
     case '-':
	 if (YDS >= 1)YDS--;
	 else YDS = YDS;
      break;

     case '+':
	 if (YDS + 24 < 64) YDS++;
	 else YDS = YDS;
      break;

     case '8':
	 if(CPY - 1 >= 0) CPY--;
	 else CPY = CPY;
      break;

     case '6':
	 if(CPX + 1 < 64) CPX++;
	 else CPX = CPX;
      break;

     case '2':
	 if(CPY + 1 < 64) CPY++;
	 else CPY = CPY;
      break;

     case '4':
	 if(CPX - 1 >= 0) CPX--;
	 else CPX = CPX;
      break;

     case 'c':
	 ChangeMap();
      break;

    case 's':
      SaveMap(MapName);
     break;

     case ESC :
     exit(0);
     break;
     };
 };


void main(void) {
char MapName;
clrscr();
GetMapName(&MapName);
ReadMap(&MapName);
while(1) {
    ReadKeyboard(&MapName);
    DisplayMap(YDS,YDS+16);
    gotoxy(65,10);
    printf("%d",mapdata[CPX + CPY * 64]);
    }
}
