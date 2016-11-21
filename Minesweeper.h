#include "stdio.h"
#include "stdlib.h"

/*
Cursor struct stores the coordinates of the cursor
*/
struct CursorStruct {
	unsigned int x;
	unsigned int y;
};

typedef struct CursorStruct Cursor;

/*
Field struct stores all informations of a field*/
struct FieldStruct {
	int info; // a field is either a mine or a number
	bool marked; //true if marked and false if not
	bool hidden; // true for hidden and false for open
};

typedef struct FieldStruct Field;

// constants
const int FREE = 0;
const int MINE = 9;
const int MARKED = 10;
const int HIDDEN = 11;

// 0 is free, 1 to 8 are numbers, 9 is mine , 10 marked, 11 hidden
const unsigned char SYMBOLE_BOARD[12] = { 28, 49, 50, 51, 52, 53, 54, 55, 56, 15, 77, 35 };

// smiley symbols
const unsigned char GAME_SMILEY[2] = { 15, 2 };

// global variables
Cursor cursor;
Field** board;

int boardSize;
int mines;
int minesMarked;

// determines the state of the game
bool game;
bool gameSmiley;