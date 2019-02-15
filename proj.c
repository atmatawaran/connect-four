/*****************************
Credits to: 
LIGHTS OUT ics-os
*****************************/

#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

// CONSTANTS
#define WIDTH 8
#define HEIGHT 8

#define START 's'
#define QUIT 'q'

// values for the board array
#define PLAYER1 1
#define PLAYER2 2
#define EMPTY 0

#define GRAY 56

// no of pieces allowed for each player
#define PLAYER1_PIECE 32
#define PLAYER2_PIECE 32

#define X_COORD 50
#define Y_COORD 10

// GLOBAL VARIABLES
int board[HEIGHT][WIDTH]; // will set as the board
int circles_x[HEIGHT][WIDTH];
int circles_y[HEIGHT][WIDTH];
int player1_count = 0; // takes note of the number of pieces player1 has used
int player2_count = 0; // takes note of the number of pieces player2 has used
int turn = 0; // says whose turn it is (1 - player1, 2 - player2) initially 0
int winner = 0; // will be set as 1 or 2 depending on the result of the game

// FUNCTION PROTOTYPES
void printMenu();
void howToWin();
void legend();
void extra();
void placePiece(int);
int checkWinner();
void setCircles(int, int);
void light(int, int, int, int);

void erase(int, int, int, int);
void printBoard(int, int);
void printCircle(int, int, int, int);
void gray(int, int);

int main(){
	int i,j;
	char key_press = START;

	set_graphics(VGA_320X200X256);
	setCircles(X_COORD, Y_COORD); // set the coordinates of the circles

	printMenu(); // game title and option to start or quit

	// initializes the board to EMPTY cells (contains 0)
	for(i = 0; i < HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			board[i][j] = EMPTY;
		}
	}
	
	key_press=(char)getch(); // accepts 's' for start or 'q' for quit
	erase(1,1,400,200); // prints a black rectangle, rapainting over the current window
	
	if(key_press == START){ // if the user presses 's'
		howToWin(); // prints the mechanics on how to win
		legend(); // additonal instructions
		write_text("GAME START!", 110, 90, WHITE, 1);
		sleep(15);
		erase(1,1,400,200);

		extra(); // title beside the board; decoration only

		printBoard(X_COORD, Y_COORD); // print initial board (8x8 gray circles)
		turn = PLAYER1; // player 1 (red) will be the first turn
		
		do{
			key_press=(char)getch(); // accepts column 1-7

			// matches the key_press to the corresponding columnn number
			if(key_press == '1') placePiece(0); // places the piece of the current player to the board
			else if(key_press == '2') placePiece(1);
			else if(key_press == '3') placePiece(2);
			else if(key_press == '4') placePiece(3);
			else if(key_press == '5') placePiece(4);
			else if(key_press == '6') placePiece(5);
			else if(key_press == '7') placePiece(6);
			else if(key_press == '8') placePiece(7);

			winner = checkWinner(); // for every turn, we will check if there is a winner

			// if there is already a winner
			if(winner != 0){
				sleep(20); // to give time for the players to know who won
				erase(1,1,400,200); 
				if(winner == PLAYER1){
					write_text("Player 1 wins!",100,80,RED,1); 	
				}
				else{
					write_text("Player 2 wins!",100,80, GREEN,1);
				}
			}

			// if there is still no winner and both players already used up all their pieces
			if(winner == 0 && player2_count == 32){
				erase(1,1,400,200);
				write_text("No one won!",100,80,WHITE,1); 
			}

		} while(key_press != QUIT || player2_count < 32); // while not 'q' or when there's still pieces left for the players
	}

	else{ // if the user chooses 'q' the opening menu
		set_graphics(VGA_TEXT80X25X16);
		clrscr();
	}
}

void printMenu(){
	write_text("CONNECT FOUR",105,60,RED,1);
	write_text("s - START",50,110,GREEN,0); 
	write_text("q - QUIT",190,110,GREEN,0);
}

void howToWin(){
	char press="any";
	write_text("HOW TO WIN", 110, 20, WHITE, 1);
	write_text("Be the first to make a straight", 20, 60, WHITE, 0); 
	write_text("line of your own color piece.", 30, 80, WHITE, 0); 
	write_text("The line can be horizontal,", 42, 100, WHITE, 0);
	write_text("diagonal or vertical.", 70, 120, WHITE, 0);
	write_text("Press any key to continue.", 40, 170, YELLOW, 0);
	press=(char)getch();
	erase(1,1,400,200);
}

void legend(){
	char press="any";
	write_text("Player 1 - RED", 94, 30, RED, 0); 
	write_text("Player 2 - GREEN", 85, 50, GREEN, 0); 
	write_text("First turn goes to Player 1.", 35, 80, WHITE, 0);
	write_text("Press the column number (1-8)", 30, 100, WHITE, 0);
	write_text("where you want to drop your piece.", 10, 120, WHITE, 0);
	write_text("Press any key to continue.", 40, 170, YELLOW, 0);
	press=(char)getch();
	erase(1,1,400,200);
}

void extra(){
	write_text("C", 20, 40, RED,0);
	write_text("O", 20, 50, RED,0);
	write_text("N", 20, 60, RED,0);
	write_text("N", 20, 70, RED,0);
	write_text("E", 20, 80, RED,0);
	write_text("C", 20, 90, RED,0);
	write_text("T", 20, 100, RED,0);
	write_text("F", 20, 120, GREEN,0);
	write_text("O", 20, 130, GREEN,0);
	write_text("U", 20, 140, GREEN,0);
	write_text("R", 20, 150, GREEN,0);
}

void placePiece(int col){
	int i, current_row;

	// will check if the available position for the current column chosen
	for(i = HEIGHT; i > 0; i--){
		if(board[i][col] == 0){
			current_row = i;
			break;
		}
	}

	board[current_row][col] = turn; // puts the player's piece on the board array

	// repaints the board; now with the current player's piece
	light(current_row, col, circles_x[current_row][col], circles_y[current_row][col]);
	
	// checks whose turn is it currently
	if(turn == PLAYER1){
		player1_count++; // increment the number of pieces player 1 has used
		turn = PLAYER2; // switches the turn to player 2
	}
	else{
		player2_count++;
		turn = PLAYER1;
	}
}

int checkWinner(){
	int i, j,win;
	for (i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j]!=0){ 

				// checks four consecutive cells vertically
				if((board[i][j]==board[i+1][j]) && (board[i+1][j]==board[i+2][j]) && (board[i+2][j]==board[i+3][j])){
					if(board[i][j] == PLAYER1) win = 1;
					else win = 2;
				return win;

				}

				// checks four consecutive cells horizontally
				if((board[i][j]==board[i][j+1]) && (board[i][j+1]==board[i][j+2]) && (board[i][j+2]==board[i][j+3])){
					if(board[i][j] == PLAYER1) win = 1;
					else win = 2;
				return win;

				}
				
				// checks four consecutive cells diagonally (from left up to right down)
				if((board[i][j]==board[i+1][j+1]) && (board[i+1][j+1]==board[i+2][j+2]) && (board[i+2][j+2]==board[i+3][j+3])){
					if(board[i][j] == PLAYER1) win = 1;
					else win = 2;
				return win;	
				}

				// checks four consecutive cells diagonally (from right up to left down)
				if((board[i][j]==board[i+1][j-1]) && (board[i+1][j-1]==board[i+2][j-2]) && (board[i+2][j-2]==board[i+3][j-3])){
					if(board[i][j] == PLAYER1) win = 1;
					else win = 2;
				return win;
				}
			}
		}
	}

	return 0; // if there is no winner yet
}

// initialize the coordinates of the circles
void setCircles(int x, int y){
	int i, j, a, b;
	
	a = x;
	b = y;
	
	for(i=0; i<HEIGHT; i++, b+=24){
		for(j=0; j<WIDTH; j++, a+=31){
			circles_x[i][j] = a;
			circles_y[i][j] = b;
		}
		a=x;
	}
}

// prints the colored piece per turn
void light(int r, int c, int x, int y){
	int i, color;
	
	switch(turn){ //sets the circle's color
		case 0: color = GRAY; break;
		case 1: color = RED; break;
		case 2: color = GREEN; break;
	}
	
	for (i=9;i<17;i++) write_pixel(i+x,2+y,color);
	for (i=7;i<19;i++) write_pixel(i+x,3+y,color);
	for (i=5;i<20;i++) write_pixel(i+x,4+y,color);
	for (i=4;i<21;i++) write_pixel(i+x,5+y,color);
	for (i=4;i<21;i++) write_pixel(i+x,6+y,color);
	for (i=3;i<22;i++) write_pixel(i+x,7+y,color);
	for (i=3;i<22;i++) write_pixel(i+x,8+y,color);
	for (i=4;i<21;i++) write_pixel(i+x,9+y,color);
	for (i=4;i<21;i++) write_pixel(i+x,10+y,color);
	for (i=5;i<20;i++) write_pixel(i+x,11+y,color);
	for (i=7;i<18;i++) write_pixel(i+x,12+y,color);
	for (i=9;i<17;i++) write_pixel(i+x,13+y,color);
}


/******************************************************************/

// covers the screen with a black rectangle
void erase(int x, int y, int w, int h){
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

// initialize the board with gray circles
void printBoard(int x, int y){
	int i, j, a, b;

	a = x;
	b = y;

	for(i = 0; i < HEIGHT; i++, b+=24){
		for(j = 0; j < WIDTH; j++, a+=31){
			printCircle(i, j, a, b);
		}
		a = x;
	}
}

// helper function
void gray(int x, int y){ //prints a gray bulb socket 
	int i = 0;
	for (i=8;i<18;i++)write_pixel(i+x,0+y,56);
	for (i=6;i<20;i++)write_pixel(i+x,1+y,56);
	for (i=4;i<21;i++)write_pixel(i+x,2+y,56);
	for (i=3;i<22;i++)write_pixel(i+x,3+y,56);
	for (i=2;i<23;i++)write_pixel(i+x,4+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,5+y,56);
	for (i=1;i<25;i++)write_pixel(i+x,6+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,7+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,8+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,9+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,10+y,56);
	for (i=0;i<25;i++)write_pixel(i+x,11+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,12+y,56);
	for (i=1;i<24;i++)write_pixel(i+x,13+y,56);
	for (i=2;i<23;i++)write_pixel(i+x,14+y,56);
	for (i=3;i<22;i++)write_pixel(i+x,15+y,56);
	for (i=5;i<20;i++)write_pixel(i+x,16+y,56);
	for (i=7;i<18;i++)write_pixel(i+x,17+y,56);
	for (i=9;i<15;i++)write_pixel(i+x,18+y,56);
}

// helper function
void printCircle(int r, int c, int x, int y){ //print a bulb 
	gray(x, y);
	light(r, c, x, y);
}
