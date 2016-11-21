#include "Minesweeper.h"
#include "time.h"
#include "windows.h"
#include "conio.h"

/*
recusive function to reveal adjacent fields
*/
void revealNeighbors(int x, int y) {
	board[x][y].hidden = false;

	// validate each neighbor field
	if (board[x][y].info == 0){
		for (int col = y - 1; col <= y + 1; col++) {
			for (int row = x - 1; row <= x + 1; row++) {
				if (row >= 0 && row < boardSize && col >= 0 && row < boardSize) {
					if (board[row][col].hidden && !board[row][col].marked && board[row][col].info != MINE) {
						// recusive call
						revealNeighbors(row, col);
					}
				}
			}
		}
	}
}

/*
general method to reveal a field
*/
void reveal() {
	if (board[cursor.x][cursor.y].hidden && !board[cursor.x][cursor.y].marked) {
		revealNeighbors(cursor.x, cursor.y);
	}
}

/*
marks or unmarks a hidden field
*/
void mark() {
	if (!board[cursor.x][cursor.y].marked && minesMarked < mines && board[cursor.x][cursor.y].hidden) {
		board[cursor.x][cursor.y].marked = true;
	}
	else if (board[cursor.x][cursor.y].marked) {
		board[cursor.x][cursor.y].marked = false;
	}
}

/*
controls the user input
*/
void navigate(char c) {
	switch (c) {
	case 'w':
		if (cursor.y > 0) cursor.y--; break;
	case 'a':
		if (cursor.x > 0) cursor.x--; break;
	case 's':
		if (cursor.y < boardSize - 1) cursor.y++; break;
	case 'd':
		if (cursor.x < boardSize - 1) cursor.x++; break;
	case 'e':
		reveal(); break;
	case 'f':
		mark(); break;
	default: break;
	}
}

/*
allocates memory for a two dimensional Field array
*/
void allocMemory() {
	board = (Field**)malloc(boardSize * sizeof(Field*));
	for (int i = 0; i < boardSize; i++) {
		board[i] = (Field*)malloc(boardSize * sizeof(Field));
	}
}

/*
free memory of board
*/
void freeMemory() {
	for (int i = 0; i < boardSize; i++) {
		free(board[i]);
	}
	free(board);
}

/*
opens every field
*/
void openAllFields() {
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			board[x][y].hidden = false;
		}
	}
}

/*
resets info of every field
*/
void resetFields() {
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			board[x][y].info = FREE;
			board[x][y].marked = false;
			board[x][y].hidden = true;
		}
	}
}

/*
calculates x and y from a position value
*/
void getCoordinates(int *x, int *y, int pos) {
	(*y) = pos / boardSize;
	(*x) = pos % boardSize;
}

/*
randomly generates a position value for a mine
*/
int randMine(int rest, int mineCount) {
	return (rand() % (rest / mineCount)) + 1;
}

/*
evenly scatters mines with a random factor
*/
void generateMines() {
	srand(time(NULL));

	int x, y;

	int total = boardSize * boardSize;
	int position = 0;
	for (int i = 0; i < mines; i++) {
		position += randMine(total - position, mines - i);
		getCoordinates(&x, &y, position - 1);
		board[x][y].info = MINE;
	}
}

/*
returns amount of mines adjacent to x and y coordinates
*/
int countMines(int x, int y) {
	int counter = 0;
	for (int col = y - 1; col <= y + 1; col++) {
		for (int row = x - 1; row <= x + 1; row++) {
			if (row >= 0 && row < boardSize && col >= 0 && col < boardSize && board[row][col].info == MINE)
				counter++;
		}
	}
	return counter;
}

/*
calculates the info value for each field
*/
void calculateInfo() {
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			if (board[x][y].info == FREE) {
				countMines(x, y);
				board[x][y].info = countMines(x, y);
			}
		}
	}
}

/*
draws the game board
*/
void drawBoard() {
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			if (cursor.x == x && cursor.y == y) {
				// windows.h method to color the console, uses a * 16 + b whereas a is background and b is font color
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 * 16 + 12);
			}
			if (board[x][y].hidden) {
				if (board[x][y].marked) {
					printf("%c", SYMBOLE_BOARD[MARKED]);
				}
				else {
					printf("%c", SYMBOLE_BOARD[HIDDEN]);
				}
			}
			else {
				printf("%c", SYMBOLE_BOARD[board[x][y].info]);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 * 16 + 15);
		}
		printf("\n");
	}
}

/*
draws the game info
*/
void drawInfo() {
	printf("Mines Marked: %i - %c - Mines Hidden: %i\n", minesMarked, GAME_SMILEY[gameSmiley], mines);
}

/*
draws the current board onto the console
*/
void draw() {
	system("cls");
	drawInfo();
	drawBoard();
	//debug
	//printf("size: %i\nmines: %i", boardSize, mines);
}

/*
determines if game is won or lost or ongoing
game is won if all mines have been marked correctly
game is lost if user entered a field with a mine
 - updates minesMarked value
*/
void updateGameStatus() {
	int gameCounter = 0;
	minesMarked = 0;

	bool steppedOnMine = false;
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			if (board[x][y].marked) {
				minesMarked++;
				if (board[x][y].info == MINE)
					gameCounter++;
			}
			if (board[x][y].info == MINE && !board[x][y].hidden) {
				steppedOnMine = true;
				gameSmiley = false;
			}
		}
	}
	if (steppedOnMine || gameCounter == mines)
		game = false;
}

/*
verifies legit input
*/
void verify() {
	boardSize = max(boardSize, 2);
	boardSize = min(boardSize, 20);
	mines = max(mines, 1);
	mines = min(mines, (boardSize*boardSize) - 1);
}

/*
initializes and runs the game
Parameters for size and number of mines
*/
void run(int size, int mineCount) {
	gameSmiley = true;
	boardSize = size;
	mines = mineCount;
	minesMarked = 0;
	cursor.x = 0;
	cursor.y = 0;

	verify();
	allocMemory();
	resetFields();
	generateMines();
	calculateInfo();
	game = true;

	while (game) {
		draw();
		char c = _getch();
		navigate(c);
		updateGameStatus();
	}

	openAllFields();
	draw();
}

/*
the main method acts as a main menu of the game
it allows customization of the game board and exiting the game
*/
void main() {
	char c = 'y';
	int size;
	int mines;
	while (c != 'n') {
		system("cls");
		printf("Lets play Minesweeper!\nChoose your board size and mine count. Confirm with ENTER: ");
		scanf_s("%i%i", &size, &mines);
		run(size, mines);
		printf("\nPlay another game of Minesweeper y/n?\n");
		do {
			c = _getch();
		} while (c != 'y' && c != 'n');
	}
	freeMemory();
}
