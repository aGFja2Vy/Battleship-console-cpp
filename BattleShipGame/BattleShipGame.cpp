// BattleShipGame.cpp : This file contains the 'main' function. Program execution begins and ends there.

/* 
*   Optional TODO:
*------------------------
*   -Coordanate bug with letters
*	-Score Board
*	-Better AI
*	-Track placement for 5 turns
*	-Clear screen fix
*	-Abilities
*	-Flexable Board Size (and number of ships)
*	-Make Object Oriented (add classes)
*/




#include "pch.h"
#include <iostream>
//Library includes random to create random inputs by the "ai".
#include <cstdlib>
//Libary included to create truly random numbers with the libary above random functions.
#include <ctime>

//DisplayBoard() to show all of the player's board values to them.
void DisplayBoard(int board[11][11]);

//set_board() to initially set all values of the board to O.
void set_board(int board[11][11]);

//ai_set_ship() to set the enemy's board via random input.
void ai_set_ship(int board[11][11], int w, int ships);

//updateboard() to update the top board to the show the same as the ai board, but without the ships.
void updateboard(int board[11][11], int board2[11][11]);

//set_ship() to set up the player's ships with their inputs. Also resets int values so it may be used again.
void set_ship(int board[11][11], int ships, int w, int x2 = 0, int y2 = 0);

//shoot() to recieve input from the player and input into the check_hit() function, and outputs whether or not the shot hit or missed.
void shoot(int board[11][11], int x1, int y1);

//ai_shoot() to generate random numbers and input it into a check_hit() function, and outputs whether or not the shot hit or missed.
void ai_shoot(int board[11][11], int &orientation, int &xai, int &yai);

//check_hit() to check the board for whether or not the input was successful or not, and outputs the answer to the shoot() function.
bool check_hit(int x, int y, int board[11][11], bool &duplicateShot);
bool check_hit(int x, int y, int board[11][11], bool &duplicateShot, bool &not_on_board);
bool check_hit(int x, int y, int board[11][11], bool &duplicateShot, bool &not_on_board, bool &shipShot);

//life_test() to check if any of a player's ships are alive. Will end the game if no ship spaces are found on either board.
bool life_test(int board[11][11]);

//hspace_test() to check if places chosen by the player for their ships will overlap other ships and/or clip out of the board for horizontal orientation.
bool hspace_test(int board[11][11], int ships, int x2, int y2);

//vspace_test() to check if places chosen by the player for their ships will overlap other ships and/or clip out of the board for vertical orientation.
bool vspace_test(int board[11][11], int ships, int x2, int y2);

//main() to start the thing.
int main()
{
	srand((unsigned)time(0));
	int xai = 0;
	int yai = 0;
	int orientation = 0;
	int turn_counter = 1;
	int ships[5] = { 5, 4, 3, 3, 2 };
	int bottom_board[11][11] = {};
	int top_board[11][11] = {};
	int ai_board[11][11] = {};

	set_board(top_board);
	set_board(bottom_board);
	set_board(ai_board);

	DisplayBoard(bottom_board);
	for (int w = 0; w < 5; w++) {
		set_ship(bottom_board, ships[w], w);
		system("cls");
		DisplayBoard(bottom_board);
		ai_set_ship(ai_board, w, ships[w]);
	}
	bool lifetest = life_test(bottom_board);
	bool ailifetest = life_test(ai_board);

	//This maybe a good while loop :)
	while (lifetest && ailifetest) {
		shoot(ai_board, 0, 0);
		updateboard(ai_board, top_board);
		system("cls");
		DisplayBoard(top_board);
		ailifetest = life_test(ai_board);
		if (!(ailifetest)) {
			std::cout << "You win!" << std::endl;
			DisplayBoard(ai_board);
			std::cout << "Turns taken: " << turn_counter << std::endl;
			return 0;
		}
		ai_shoot(bottom_board, orientation, xai, yai);
		DisplayBoard(bottom_board);
		lifetest = life_test(bottom_board);
		if (!(lifetest)) {
			std::cout << "You lost!" << std::endl;
			DisplayBoard(ai_board);
			std::cout << "Turns taken: " << turn_counter << std::endl;
			return 0;
		}
		turn_counter++;
	}
}

void DisplayBoard(int board[11][11])
{
	char tileType[8] = { '#','#','#','#','#', ' ','X','H' };
	char alpha[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	std::cout << "  |1  2  3  4  5  6  7  8  9  10" << std::endl;
	std::cout << "--|-----------------------------" << std::endl;
	for (int x = 0; x < 10; x++) {
		std::cout << alpha[x] << " |";
		for (int y = 0; y < 10; y++)
			std::cout << tileType[board[x][y]] << "  ";
		std::cout << std::endl;
	}
}

void set_board(int board[11][11])
{
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++)
			board[x][y] = 5;
	}
}

void ai_set_ship(int board[11][11], int w, int ships)
{
	bool ship_probs = false;
		bool spacetest = false;
		int x2 = std::rand() % 10;
		int y2 = std::rand() % 10;
		int position = std::rand() % 2;
		if (position == 0) {
			spacetest = hspace_test(board, ships, x2, y2);
			if (spacetest) {
				for (int y = 0; y < ships; y++)
					board[x2][y2 - (ships / 2) - 1 + y] = w;
				ship_probs = false;
			}
		}
		else {
			spacetest = vspace_test(board, ships, x2, y2);
			if (spacetest) {
				for (int x = 0; x < ships; x++)
					board[x2 - (ships / 2) - 1 + x][y2] = w;
				ship_probs = false;
			}
		}
		if (!(spacetest))
			ship_probs = true;
		if (ship_probs)
			ai_set_ship(board, w, ships);
}

void updateboard(int board[11][11], int board2[11][11])
{
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (board[x][y] == 7)
				board2[x][y] = 7;
			else if (board[x][y] == 6)
				board2[x][y] = 6;
			else
				board2[x][y] = 5;
		}
	}
}

//Figure out a way to set int position to a bool value instead of a number?
void set_ship(int board[11][11], int ships, int w, int x2, int y2)
{
	bool ship_probs = false;
	bool spacetest = false;
	int position = 0;
	std::cout << "Where would you like to place your ship?" << std::endl;
	std::cin >> x2 >> y2;
	if (x2 > 10 || y2 > 10 || x2 < 0 || y2 < 0) {
		std::cout << "Invalid." << std::endl;
		set_ship(board, ships, x2, y2);
	}
	else {
		std::cout << "1: Horizontal \n2: Vertical" << std::endl;
		std::cin >> position;
		if (position == 1) {
			spacetest = hspace_test(board, ships, x2, y2);
			if (spacetest) {
				for (int y = 0; y < ships; y++)
					board[x2 - 1][y2 - (ships / 2) - 1 + y] = w;
				ship_probs = false;
			}
		}
		else if (position == 2) {
			spacetest = vspace_test(board, ships, x2, y2);
			if (spacetest) {
				for (int x = 0; x < ships; x++)
					board[x2 - (ships / 2) - 1 + x][y2 - 1] = w;
				ship_probs = false;
			}
		}
		else {
			std::cout << "Please try again0" << std::endl;
		}
		if (!spacetest) {
			std::cout << "Invalid position." << std::endl;
			ship_probs = true;
		}
		if (ship_probs)
			set_ship(board, ships, w, x2, y2);
	}
}

void shoot(int board[11][11], int x1, int y1)
{
	bool hit = false;
	bool duplicateShot = false;
	bool not_on_board = false;

		std::cout << "Where would you like to aim?" << std::endl;
		std::cin >> x1 >> y1;
		hit = check_hit(x1 - 1, y1 - 1, board, duplicateShot, not_on_board);

		if (duplicateShot)
			std::cout << "Already shot there. Please try again." << std::endl;
		if (not_on_board)
			std::cout << "Inputs are not on the board. Please try again." << std::endl;
		if (duplicateShot || not_on_board)
			shoot(board, x1, y1);
		else {
			std::cout << "You fired at " << x1 << ", " << y1 << std::endl;
			if (hit) {
				board[x1 - 1][y1 - 1] = 7;
				std::cout << "Hit!" << std::endl;
			}
			else {
				board[x1 - 1][y1 - 1] = 6;
				std::cout << "Miss!" << std::endl;
			}
		}
}

void ai_shoot(int board[11][11], int &orientation, int &xai, int &yai)
{
	int ptrxai = xai;
	int ptryai = yai;
	bool duplicateShot = false;
	bool hit = false;
	bool not_on_board = false;
	bool shipShot = false;
	switch (orientation) {
	case 1: {
		hit = check_hit(xai + 1, yai, board, duplicateShot, not_on_board, shipShot);
		if (shipShot) {
			ptrxai++;
			break;
		}
			if (duplicateShot || not_on_board) {
				orientation--;
				break;
			}
			std::cout << "Enemy fires at " << xai + 2 << ", " << yai + 1 << std::endl;
			if (hit) {
				board[xai + 1][yai] = 7;
				std::cout << "You've been hit!" << std::endl;
				ptrxai++;
			}
			else {
				board[xai + 1][yai] = 6;
				std::cout << "Enemy missed!" << std::endl;
				orientation--;
			}
			break;
	}
	case 2: {
		hit = check_hit(xai, yai + 1, board, duplicateShot, not_on_board, shipShot);
		if (shipShot) {
			ptryai++;
			break;
		}
		if (duplicateShot || not_on_board) {
			orientation--;
			break;
		}
		std::cout << "Enemy fires at " << xai + 1 << ", " << yai + 2 << std::endl;
		if (hit) {
			board[xai][yai + 1] = 7;
			std::cout << "You've been hit!" << std::endl;
			ptryai++;
		}
		else {
			board[xai][yai + 1] = 6;
			std::cout << "Enemy missed!" << std::endl;
			orientation--;
		}
		break;
	}
	case 3: {
		hit = check_hit(xai - 1, yai, board, duplicateShot, not_on_board, shipShot);
		if (shipShot) {
			ptrxai--;
			break;
		}
		if (duplicateShot || not_on_board) {
			orientation--;
			break;
		}
		std::cout << "Enemy fires at " << xai << ", " << yai + 1 << std::endl;
		if (hit) {
			board[xai - 1][yai] = 7;
			std::cout << "You've been hit!" << std::endl;
			ptrxai--;
		}
		else {
			board[xai - 1][yai] = 6;
			std::cout << "Enemy missed!" << std::endl;
			orientation--;
		}
		break;
	}
	case 4: {
		hit = check_hit(xai, yai - 1, board, duplicateShot, not_on_board, shipShot);
		if (shipShot) {
			ptryai--;
			break;
		}
		if (duplicateShot || not_on_board) {
			orientation--;
			break;
		}
		std::cout << "Enemy fires at " << xai + 1 << ", " << yai << std::endl;
		if (hit) {
			board[xai][yai - 1] = 7;
			std::cout << "You've been hit!" << std::endl;
			ptryai--;
		}
		else {
			board[xai][yai - 1] = 6;
			std::cout << "Enemy missed!" << std::endl;
			orientation--;
		}
		break;
	}
	default: {
		int xai = std::rand() % 10;
		int yai = std::rand() % 10;
		hit = check_hit(xai, yai, board, duplicateShot);
		if (duplicateShot)
			break;
		ptrxai = xai;
		ptryai = yai;
		std::cout << "Enemy fires at " << xai + 1 << ", " << yai + 1 << std::endl;
		if (hit) {
			board[xai][yai] = 7;
			std::cout << "You've been hit!" << std::endl;
			orientation = 4;
		}
		else {
			board[xai][yai] = 6;
			std::cout << "Enemy missed!" << std::endl;
		}
		break;
	}
	}
	yai = ptryai;
	xai = ptrxai;
	if (duplicateShot || not_on_board)
		ai_shoot(board, orientation, xai, yai);
}

bool check_hit(int x, int y, int board[11][11], bool &duplicateShot, bool &not_on_board)
{
	if (x > 14 || y > 14 || x < 0 || y < 0)
		not_on_board = true;
	else
		not_on_board = false;
	if (board[x][y] == 7 || board[x][y] == 6) 
		duplicateShot = true;
	else
		duplicateShot = false;

	if (board[x][y] < 5)
		return true;
	return false;
}
bool check_hit(int x, int y, int board[11][11], bool &duplicateShot)
{
	if (board[x][y] == 7 || board[x][y] == 6)
		duplicateShot = true;
	else
		duplicateShot = false;
	if (board[x][y] < 5)
		return true;
	return false;
}
bool check_hit(int x, int y, int board[11][11], bool &duplicateShot, bool &not_on_board, bool &shipShot)
{
	if (x > 14 || y > 14 || x < 0 || y < 0)
		not_on_board = true;
	else
		not_on_board = false;
	if (board[x][y] == 7)
		shipShot = true;
	else
		shipShot = false;
	if (board[x][y] == 6)
		duplicateShot = true;
	else
		duplicateShot = false;
	if (board[x][y] < 5)
		return true;
	return false;
}

//function name "life_test" undecided.
bool life_test(int board[11][11])
{
	int lifetest = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (board[x][y] < 5)
				lifetest++;
		}
	}
	if (lifetest >= 1)
		return true;
	return false;
}

bool hspace_test(int board[11][11], int ships, int x2, int y2)
{
	int hspacetest = 0;
	for (int y = 0; y < ships; y++) {
		if (board[x2 - 1][y2 - (ships / 2) - 1 + y] == 5) {
			hspacetest++;
		}
	}
	if (hspacetest == ships)
		return true;
	return false;
}
bool vspace_test(int board[11][11], int ships, int x2, int y2)
{
	int vspacetest = 0;
	for (int x = 0; x < ships; x++) {
		if (board[x2 - (ships / 2) - 1 + x][y2 - 1] == 5) {
			vspacetest++;
		}
	}
	if (vspacetest == ships)
		return true;
	return false;
}
