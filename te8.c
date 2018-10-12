// For editing gui elements that will be 8x8
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define ESC 27
unsigned char far *video_buffer = (char far *)0xA0000000L;
int YDS = 0,YDE = 23;
int CPX = 0,CPY = 0;
int Xbit, Ybit;
int WhichTile = 0;
int TXTCOLOR;
int DrawColor;
int CurrCol;
int x,y;
typedef struct Tile
{
   int bitmap[8][8];
};

// This makes more sense than 64 or 128 though I'm starting with just 4 elements
struct Tile Tiles[256];

FILE *tiles;



void ReadTiles(void) {
  int TileNum;
  printf("Reading in tilefile.\n");
  tiles = fopen("shapes.bin", "r");
  for (TileNum = 0; TileNum < 256; TileNum++)
  {
  for (y = 0; y < 8; y++)
  {
    for (x = 0; x < 8; x++)
    {
      Tiles[TileNum].bitmap[x][y] = getc(tiles);
    };
  };
  }
  fclose(tiles);
};

//END TILE READ
void SaveTiles(void) {
  int TileNum;

  printf("Saving Tile File.\n");
  tiles = fopen("shapes.bin", "w");
  for (TileNum = 0; TileNum < 64; TileNum++)
  {
  for (y = 0; y < 8; y++)
  {
    for (x = 0; x < 8; x++)
    {
     fputc(Tiles[TileNum].bitmap[x][y],tiles);
    };
  };
  };
  fclose(tiles);
};






void DisplayTile(int WhichTile) {
    void DrawSquare(int, int, int, int, unsigned char);
    void DrawOutline(int, int, int, int, unsigned char);
//gotoxy(1,1);
//printf("\n");
//clrscr();

 for (y = 0; y < 8; y++)
  {
    for (x = 0; x < 8; x++)
    {

//    textcolor(TXTCOLOR);
//    printf("%c",Tiles[WhichTile].bitmap[x][y]);
    DrawSquare(x*5,y*5,(x+1)*5,(y+1)*5,Tiles[WhichTile].bitmap[x][y]);
    if(x == CPX && y == CPY) DrawOutline(x*5,y*5,((x+1)*5),((y+1)*5),60);
    };
  //  printf("\n");
  };
};

void PlotPixel(int x,int y,unsigned char Color)
{
  video_buffer[y*320 + x] = Color;
};

void DrawSquare(int x,int y,int x2, int y2, unsigned char Color)
{
   int index1, index2;

   for(index1 = x;index1 < x2;index1 ++) {
      for(index2 = y;index2 < y2;index2 ++) {
	 PlotPixel(index1,index2,Color);
      }
   }
}

void DrawOutline(int x,int y,int x2, int y2, unsigned char Color)
{
 PlotPixel(x,y,Color);
 PlotPixel(x2-1,y2-1,Color);
 PlotPixel(x2-1,y,Color);
 PlotPixel(x,y2-1,Color);
 PlotPixel(x+1,y+1,Color);
 PlotPixel(x2-2,y2-2,Color);
 PlotPixel(x2-2,y+1,Color);
 PlotPixel(x+1,y2-2,Color);
}

void DisplayPalette(void) {
  int PalIndex,PalX,PalY;

  for(PalIndex = 0;PalIndex <= 255;PalIndex++) {
    PalX = PalIndex%16;
    PalY = PalIndex/16;
    DrawSquare((PalX + 0)*10,120+(PalY * 5),
	       (PalX + 1)*10,125+(PalY * 5),
		PalIndex);
  }
}


void DrawPalCursor(void) {
int CurCol;
int CurX,CurY;
   CurCol = DrawColor;
   CurX = CurCol%16;
   CurY = CurCol/16;
   PlotPixel((CurX * 10)+3,122+(CurY * 5),  0);
   PlotPixel((CurX * 10)+5,122+(CurY * 5),  0);
   PlotPixel((CurX * 10)+4,120+(CurY * 5)+1,15);
   PlotPixel((CurX * 10)+4,120+(CurY * 5)+3,15);

   PlotPixel((CurX * 10)+4,120+(CurY * 5)+2,15);
   PlotPixel((CurX * 10)+3,120+(CurY * 5)+1,15);
   PlotPixel((CurX * 10)+3,120+(CurY * 5)+3,15);
   PlotPixel((CurX * 10)+5,120+(CurY * 5)+1,15);
   PlotPixel((CurX * 10)+5,120+(CurY * 5)+3,15);
}

void Display_One_Tile(int X_Location, int Y_Location, int TileNumber) {
  for (Ybit = 0;Ybit!=8;Ybit++) {
    for (Xbit = 0;Xbit!=8;Xbit++) {
      PlotPixel(X_Location+Xbit,Y_Location+Ybit,(Tiles[TileNumber].bitmap[Xbit][Ybit]));
    }
  }
}

void FillColor(int ThisTile) {
 int IndX,IndY;
 int FillColor;

 scanf("%d",&FillColor);
 for(IndX = 0;IndX < 16;IndX++)
 {
   for(IndY = 0;IndY < 16;IndY++)
   {
   Tiles[ThisTile].bitmap[IndX][IndY] = FillColor;
   }
 }

};
void CopyTile(int WhichTile)
{
 int ToTile,IndX,IndY;

 scanf("%d",&ToTile);
 for(IndX = 0;IndX < 16;IndX++)
 {
   for(IndY = 0;IndY < 16;IndY++)
   {
   Tiles[ToTile].bitmap[IndX][IndY] = Tiles[WhichTile].bitmap[IndX][IndY];
   }
 }
}

void GotoTile(void)
{
 scanf("%d",&WhichTile);
}

void ChangeColor(char dir) {
/* if(dir == 'R' && Tiles[WhichTile].bitmap[CPX][CPY] < 255) Tiles[WhichTile].bitmap[CPX][CPY]++;
 if(dir == 'L' && Tiles[WhichTile].bitmap[CPX][CPY] > 0  ) Tiles[WhichTile].bitmap[CPX][CPY]--;
 if(dir == 'D' && Tiles[WhichTile].bitmap[CPX][CPY] < 239) Tiles[WhichTile].bitmap[CPX][CPY] = Tiles[WhichTile].bitmap[CPX][CPY]+16;
 if(dir == 'U' && Tiles[WhichTile].bitmap[CPX][CPY] > 16 ) Tiles[WhichTile].bitmap[CPX][CPY] = Tiles[WhichTile].bitmap[CPX][CPY]-16;
*/
 if(dir == 'R' && DrawColor < 255) DrawColor++;
 if(dir == 'L' && DrawColor > 0  ) DrawColor--;
 if(dir == 'D' && DrawColor < 239) DrawColor = DrawColor+16;
 if(dir == 'U' && DrawColor > 16 ) DrawColor = DrawColor-16;


};

void ChangeTile(void) {
 scanf("%d",&Tiles[WhichTile].bitmap[CPX][CPY]);
};

void draw_color(int CurCol) {
 Tiles[WhichTile].bitmap[CPX][CPY] = CurCol;
}

void Change_CurrCol(int *CurCol) {
  scanf("%d",CurCol);
}


void ReadKeyboard(void) {

  char KeyBoardIn=getch();
  switch (KeyBoardIn)
    {
     case '-':
	 if(WhichTile >= 1)
	  WhichTile = WhichTile - 1;
      break;

     case '+':
	 if(WhichTile < 63)
	 WhichTile = WhichTile + 1;
      break;

     case '8':
	 if(CPY - 1 >= 0) CPY--;
	 else CPY = CPY;
      break;

     case '6':
	 if(CPX + 1 < 16) CPX++;
	 else CPX = CPX;
      break;

     case '2':
	 if(CPY + 1 < 16) CPY++;
	 else CPY = CPY;
      break;

     case '4':
	 if(CPX - 1 >= 0) CPX--;
	 else CPX = CPX;
      break;

     case 'c':
       ChangeTile();
      break;

     case 'w' :
       ChangeColor('U');
      break;

     case 's':
       ChangeColor('D');
      break;

      case 'a' :
	ChangeColor('L');
      break;

      case 'd':
	ChangeColor('R');
      break;

     case 'f' :
       FillColor(WhichTile);
      break;

     case 'S':
       SaveTiles();
     break;

     case 'g':
       GotoTile();
     break;

     case 'p':
       CopyTile(WhichTile);
     break;

     case 'z':
       Tiles[WhichTile].bitmap[CPX][CPY] = 0;
     break;

     case '0':
       draw_color(DrawColor);
     break;

     case '/':
       Change_CurrCol(&CurrCol);
     break;

     case ESC :
     exit(0);
     break;
     };
 };
void SetMode(char far vidmode)
{
  asm {
    mov AH,0
    mov AL, BYTE PTR vidmode
    int 10h
  }
};


void main(void) {
clrscr();
ReadTiles();
SetMode(0x13);

DisplayTile(WhichTile);
DisplayPalette();
DrawPalCursor();


while(1) {
    ReadKeyboard();
    DisplayTile(WhichTile);
    DisplayPalette();
    DrawPalCursor();
    gotoxy(11,10);
    printf("Tile Number %3d",WhichTile);
    printf("\n%3d",Tiles[WhichTile].bitmap[CPX][CPY]);
    Display_One_Tile(128,0,WhichTile);
    Display_One_Tile(136,0,WhichTile);
    Display_One_Tile(128,8,WhichTile);

  }
}
