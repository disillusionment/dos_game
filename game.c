#include   <stdlib.h>
#include    <conio.h>
#include    <stdio.h>

#define ESC    0x1b
#define MoveOk    1
#define MoveNotOk 0
#define mapsize   0x1000
#define Xend      0x40
#define Yend      0x40
unsigned char far *video_buffer = (char far *)0xA0000000L;

char KeyBoardIn;
int mapdata[mapsize];

int Xpos = 0, Ypos = 0, x, y, TileNumberToDisplay;
int EnemyXpos = 34, EnemyYpos = 27;
int BP_X, BP_Y, TileNum, null, Ch_X, Ch_Y;
int Xbit, Ybit;
int Check = 0, Continue = 1;

char *name = "Don Joe";

typedef struct Tile
  {
    int bitmap[16][16];
  };

typedef struct Mobile
 {
    int XPos,YPos;
 };

struct Tile Tiles[64];
struct Tile CharTile[2];
struct Mobile Chasers[1];


// Read In Section
int ReadMap(void) {
  int index;
  FILE *map;
  printf("Reading in mapfile.\n");

  if((map = fopen("sosaria.ult" , "r")) == NULL) {
    printf("Failed to open map file.\n");
    return(1);
  }

  for (index = 0; index < mapsize; index++)
  {
     mapdata[index] = getc(map);
  }
  fclose(map);
  return(0);
};


int ReadTile(void)
{
  FILE *TileFilePtr;
  printf("Reading in tilefile.\n");
  if((TileFilePtr = fopen("tilefile.64b", "r")) == NULL) {
    printf("Failed to open tile file.\n");
    return(1);
  }

  for (TileNum = 0;TileNum!=64;TileNum++) {
    for (BP_Y = 0;BP_Y!=16;BP_Y++) {
      for (BP_X = 0;BP_X!=16;BP_X++) {
	       Tiles[TileNum].bitmap[BP_X][BP_Y] = getc(TileFilePtr);
      }
    }
  }
  fclose(TileFilePtr);
  return(0);
};

int ReadNBTile(int TileNum, char FileName[12])
{
  FILE *TileFPtr;
  printf("Reading in charactertile.\n");
  if((TileFPtr = fopen(FileName, "r")) == NULL) {
    printf("Failed to open %s.\n",FileName);
    return(1);
  }
  for (Ch_Y = 0;Ch_Y!=16;Ch_Y++)
  {
    for (Ch_X = 0;Ch_X!=16;Ch_X++)
    {
      CharTile[TileNum].bitmap[Ch_X][Ch_Y] = getc(TileFPtr);
    }
   }
  fclose(TileFPtr);
  return(0);
};

//End of read in section
//
//
//
//
//
//
// Graphics Handeling Section
void PlotPixel(int x,int y,unsigned char Color)
{
  video_buffer[y*320 + x] = Color;
};

void SetMode(char far vidmode)
{
  asm {
    mov AH,0
    mov AL, BYTE PTR vidmode
    int 10h
  }
};

void Display_One_Tile(int X_Location, int Y_Location, int TileNumber) {
  for (Ybit = 0;Ybit!=16;Ybit++) {
    for (Xbit = 0;Xbit!=16;Xbit++) {
      PlotPixel(X_Location+Xbit,Y_Location+Ybit,(Tiles[TileNumber].bitmap[Xbit][Ybit]));
    }
  }
};

void Display_Non_Background_Tile(int X_Location, int Y_Location, int TileGoingHere /* 0=Char, 1= Enemy */) {
  for (Ybit = 0;Ybit!=16;Ybit++) {
    for (Xbit = 0;Xbit!=16;Xbit++) {
      PlotPixel(X_Location+Xbit,Y_Location+Ybit,(CharTile[TileGoingHere].bitmap[Xbit][Ybit]));
      /*if (CharTile[TileGoingHere].bitmap[Xbit][Ybit] == 15)
	 PlotPixel(X_Location+Xbit,Y_Location+Ybit,(CharTile[TileGoingHere].bitmap[Xbit][Ybit]));
      else if (CharTile[TileGoingHere].bitmap[Xbit][Ybit] == 0)
	 PlotPixel(X_Location+Xbit,Y_Location+Ybit,(CharTile[TileGoingHere].bitmap[Xbit][Ybit]));
      else
	 PlotPixel(X_Location+Xbit,Y_Location+Ybit,(Tiles[TileSupposedToGoHere].bitmap[Xbit][Ybit]));
       */
    }
  };
};
// End graphics section


// Mapping section

void DrawMap(void)
{
  int Tile_X_Position = 0,Tile_Y_Position = 0;
  int Plot_X,Plot_Y;
  gotoxy(30,20);
  printf("%i,%i:",Xpos,Ypos);
  for (y = (Ypos - 4); y<(Ypos + 5); y++)
  {
    Tile_Y_Position++;
    Tile_X_Position = 0;
    for (x = (Xpos - 4); x<(Xpos + 5); x++)
    {
      if (x < 0) Plot_X = Xend + x;
      else if (x > 63) Plot_X = x - Xend;
      else Plot_X = x;

      if (y < 0) Plot_Y = Yend + y;
      else if (y > 63) Plot_Y = y - Yend;
      else Plot_Y = y;

      Tile_X_Position++;

      if (Tile_Y_Position == 5 && Tile_X_Position == 5) {
	 Display_Non_Background_Tile(Tile_X_Position*16,Tile_Y_Position*16,0);
      } else if (Plot_X == EnemyXpos && Plot_Y == EnemyYpos) {
	 Display_Non_Background_Tile(Tile_X_Position*16,Tile_Y_Position*16,1);
      } else {
	Display_One_Tile(Tile_X_Position*16,Tile_Y_Position*16,mapdata[Plot_X%64 + Plot_Y%64 * 64]);
//      printf("%d",mapdata[Plot_X%64 + Plot_Y%64 * 64]);
     } ;

    };
  };
}
//
//
// End Of graphics handling section
//
//

//
// Game Stuff
//
//
/*
void CheckMap(int Xpos, int Ypos) {
  typedef struct MapLocs
  {
    int Xpos;
    int Ypos;
    char MapName[12];
  };
};
*/
void ExitFunc(void) {
//	clrscr();
//	SetMode(0x3);
	gotoxy(10,1);
	printf("Exiting");
	exit(0);
};


MoveCheck (int WhatThere)
 {
 int ReturnValue;
 switch(WhatThere)
 {
   case 12:
   case 2:
   case 1:
      ReturnValue = MoveNotOk;
      break;
   default:
      ReturnValue = MoveOk;
      break;
   };
return(ReturnValue);
};


void MoveControl(void)
 {
    char XP;
    char XM;
    char YM;
    char YP;

    if (Xpos == Xend - 1) XP = mapdata[0 +Ypos * 64];
    else XP = mapdata[(Xpos + 1) + Ypos * 64];

    if (Xpos == 0) XM = mapdata[(Xend - 1) + Ypos * 64];
    else XM = mapdata[(Xpos - 1) + Ypos * 64];

    if (Ypos == Yend - 1) YP = mapdata[Xpos + 0];
    else YP = mapdata[Xpos + (Ypos + 1) * 64];

    if (Ypos == 0) YM = mapdata[Xpos + (Yend - 1) * 64];
    else YM = mapdata[Xpos + (Ypos - 1) * 64];

    KeyBoardIn=getch();
    switch (KeyBoardIn)
    {
     case '8':
      if (MoveCheck(YM)==MoveOk)
       if (Ypos > 0) Ypos--;
       else Ypos = Yend - 1;
      break;

     case '6':
      if (MoveCheck(XP) == MoveOk)
       if (Xpos < Xend - 1) Xpos++;
       else Xpos = 0;
      break;

     case '2' :
      if (MoveCheck(YP) == MoveOk)
       if (Ypos < Yend - 1) Ypos++;
       else Ypos = 0;
      break;

     case '4':
      if (MoveCheck(XM) == MoveOk)
       if (Xpos > 0) Xpos--;
       else Xpos = Xend - 1;
      break;
     case 't' :
     case 'T' :
      gotoxy(21,2);
      printf("Talk to whom?");
      gotoxy(21,3);
      scanf("%s",name);
     break;

     case 'w' :
      gotoxy(21,2);
      printf("You're talking to:");
      gotoxy(21,3);
      printf("%s",name);
     break;

//     case 'e' :
//      if(CheckMap(Xpos,Ypos));
//      InsideStuff();

//      break;
     case ESC:
	ExitFunc();
       break;
    }
 }

void Chase(void)
{
    int DirToMove;
    char EYP;
    char EYM;
    char EXM;
    char EXP;

    if (EnemyXpos == 0) EXM = mapdata[(Xend - 1) + EnemyYpos * 64];
    else EXM = mapdata[(EnemyXpos - 1) + EnemyYpos * 64];

    if (EnemyXpos == Xend - 1) EXP = mapdata[0 + EnemyYpos * 64];
    else EXP = mapdata[(EnemyXpos + 1) + EnemyYpos * 64];

    if (EnemyYpos == 0) EYM = mapdata[EnemyXpos + (Yend - 1) * 64];
    else EYM = mapdata[EnemyXpos + (EnemyYpos - 1) * 64];

    if (EnemyYpos == Yend - 1) EYP = mapdata[EnemyXpos + 0];
    else EYP = mapdata[EnemyXpos + (EnemyYpos + 1) * 64];

    if (EnemyXpos>Xpos && MoveCheck(EXM) == MoveOk) EnemyXpos--;
    else if (EnemyXpos<Xpos && MoveCheck(EXP) == MoveOk) EnemyXpos++;
    else if (EnemyYpos>Ypos && MoveCheck(EYM) == MoveOk) EnemyYpos--;
    else if (EnemyYpos<Ypos && MoveCheck(EYP) == MoveOk) EnemyYpos++;

};

CaughtStatus(void) {
 if (EnemyXpos == Xpos && EnemyYpos == Ypos) return(1);
 else return(0);
} ;

DiedPrompt(void) {
 printf("You have died, do you wish to continue?");
 KeyBoardIn = getch();
 while(KeyBoardIn != 'y'&& KeyBoardIn != 'Y' && KeyBoardIn != 'n' && KeyBoardIn != 'N') KeyBoardIn = getch();
 if(KeyBoardIn == 'y' || KeyBoardIn == 'Y')
  return(1);
 else
  return(0);
};

void ResetLocation(int StartXpos,int StartYpos,int StartEnemyXpos,int StartEnemyYpos)
{
 Xpos = StartXpos;
 Ypos = StartYpos;
 EnemyXpos = StartEnemyXpos;
 EnemyYpos = StartEnemyYpos;
};

void initialize(void) {
 if(ReadMap() != 0) ExitFunc();
 if(ReadTile() !=0) ExitFunc();
 if(ReadNBTile(0,"chartile.64b") != 0) ExitFunc();
 if(ReadNBTile(1,"enmytile.64b") != 0) ExitFunc();
 SetMode(0x13);
 DrawMap();
};

void Intro(void) {
};

void GamePlay(void)
{
 int MoveStat = 0;
 int MoveOn = 1;
 Check = 0;
 DrawMap();
 while(Check == 0)
   {
     MoveControl();
     if(MoveStat == 1) {
      Chase();
      MoveStat = 0;
      }
     else MoveStat = MoveStat + MoveOn;

     Check = CaughtStatus();
     DrawMap();

 };
}

void main(void)
{
  initialize();
  Intro();
  do {
   ResetLocation(1,14,34,27);
   GamePlay();
   Continue = DiedPrompt();
  }
  while(Continue);
  ExitFunc();
};
