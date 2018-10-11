#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define Xend     59
#define Yend     59
#define ESC      27

int YDS = 0,YDE = 16;
int CPX = 1,CPY = 1;
int MAP[Xend][Yend];
int TXTCOLOR;
int x,y;
FILE *map;

void GetMapName(char *MapName)
{
 printf("\nPlease enter the name of the map you would like to edit : ");
 scanf("%s",MapName);
 printf("%s\n",MapName);
}

void MapCreate(void) {
 int Ix,Iy;
 for (Iy = 0; Iy < Yend; Iy++)
  {
    for (Ix = 0; Ix < Xend; Ix++)
    {
      MAP[Ix][Iy] = 0;
    };
  };
};

void ReadMap(char *MapName) {
  int nul;
  printf("Reading in mapfile. %s \n",MapName);
  if((map = fopen(MapName, "r")) == NULL) MapCreate();
  else {
  for (y = 0; y < Yend; y++)
  {
    for (x = 0; x < Xend; x++)
    {
      MAP[x][y] = getc(map);
    };
  nul = getc(map);
  };
  };
  fclose(map);
};
//END MAP READ
void SaveMap(char *MapName) {
  printf("Saving mapfile.\n");
  map = fopen(MapName, "w");
  for (y = 0; y < Yend; y++)
  {
    for (x = 0; x < Xend; x++)
    {
     fputc(MAP[x][y],map);
    };
  };
  fclose(map);
};






void DisplayMap(int Y_Disp_Start,int Y_Disp_End) {
char PutIt;
gotoxy(1,1);

//printf("\n");
//clrscr();

for (y = Y_Disp_Start; y < Y_Disp_End; y++)
  {
    for (x = 0; x < Xend; x++)
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
  TXTCOLOR = MAP[x][y]%16;
  PutIt = '�';
    if(x == CPX & y == CPY) TXTCOLOR = BLINK+TXTCOLOR;
    textcolor(TXTCOLOR);
    putch(PutIt);
    };
    printf("\n");
  };
};

void ChangeMap(void) {
 scanf("%d",&MAP[CPX][CPY]);
 };
void ReadKeyboard(char *MapName) {

  char KeyBoardIn=getch();
  switch (KeyBoardIn)
    {
     case '-':
	 if (YDS >= 1)YDS--;
	 else YDS = YDS;
      break;

     case '+':
	 if (YDS + 24 < Yend) YDS++;
	 else YDS = YDS;
      break;

     case '8':
	 if(CPY - 1 >= 0) CPY--;
	 else CPY = CPY;
      break;

     case '6':
	 if(CPX + 1 < Xend) CPX++;
	 else CPX = CPX;
      break;

     case '2':
	 if(CPY + 1 < Yend) CPY++;
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
    printf("%d",MAP[CPX][CPY]);
    }
}
