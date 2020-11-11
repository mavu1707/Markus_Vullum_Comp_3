#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <vector>


const int BOARD_WIDTH = 15;
const int BOARD_HEIGHT = 10;
const int SHIP_TYPES = 5;

const char isWATER = 247;
const char isHIT = 'X';
const char isSHIP = 'S';
const char isMISS = '0';

struct POINT {
	int X;
	int Y;
};

struct SHIP {
	std::string name;
	int length;
	POINT onGrid[5];
	bool hitFlag[5];
}ship[SHIP_TYPES];

struct PLAYER {
	char grid[BOARD_WIDTH][BOARD_HEIGHT];
}player[3];

enum DIRECTION { HORIZONTAL, VERTICAL };
struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

bool gameRunning = false;

void LoadShips();
void ResetBoard();
void DrawBoard(int);
PLACESHIPS UserInputShipPlacement();
bool UserInputAttack(int&, int&, int);
bool GameOverCheck(int);
int answerMenu;

void drawGrid(char grid[4][4]) {
	system("cls");
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			std::cout << grid[row][column] << "| ";
		}
		std::cout << std::endl;
	}
}

//################################
//#                              #
//#         Password             #
//#                              #
//################################
std::string makePass() {
	char grid[4][4] = { {'A','B','C','D'},{'E','F','G','H'},{'I','J','K','L'},{'M','N','O','P'} };
	drawGrid(grid);

	int inputPosX{ 0 };
	int inputPosY{ 0 };

	char obj = ' ';

	std::string pass;
	pass.push_back(grid[inputPosY][inputPosX]);
	grid[inputPosY][inputPosX] = '*';
	drawGrid(grid);

	std::cout << "use q when finsished" << std::endl;

	while (obj != 'q') {
		bool ok = false;
		obj = _getch();
		switch (obj) {
		case 'a':
			inputPosX = inputPosX > 0 ? inputPosX - 1 : 0;
			ok = true;
			break;
		case 'w':
			inputPosY = inputPosY > 0 ? inputPosY - 1 : 0;
			ok = true;
			break;
		case's':
			inputPosY = inputPosY < 4 ? inputPosY + 1 : 3;
			ok = true;
			break;
		case'd':
			inputPosX = inputPosX < 4 ? inputPosX + 1 : 3;
			ok = true;
			break;
		}
		if (ok) {
			pass.push_back(grid[inputPosY][inputPosX]);

			grid[inputPosY][inputPosX] = '*';
			drawGrid(grid);
		}


	}
	return pass;
}

//################################
//#                              #
//#            MAIN              #
//#                              #
//################################
int main()
{

	std::ifstream passInFile("tuna.txt");
	std::string defaultPass = "ABCDHLPONM";

	if (passInFile.good()) {
		std::string line;
		std::ifstream defaultPassFile("tuna.txt");
		if (defaultPassFile.is_open())
		{
			while (std::getline(defaultPassFile, line))
			{
				defaultPass = line;
			}
			defaultPassFile.close();

		}
	}
	else
	{
		std::cout << "default password is: ABCDHLPONM" << std::endl;
	}

	std::string pass = makePass();

	if (pass != defaultPass) {
		system("cls");
		std::cout << "wrong password" << std::endl;
		std::cout << "'" << pass << "'   '" << defaultPass << "'" << std::endl;
		return 0;
	}

	system("pause");
	system("cls");

	std::cout << "Please select one of the following: \n";
	std::cout << "1. Change the password\n";
	std::cout << "2. play battleship\n";
	std::cout << "3. to quit\n";

	std::cin >> answerMenu;

	//################################
	//#                              #
	//#         Answer = 1           #
	//#                              #
	//################################
	if (answerMenu == 1) {
		system("cls");

		std::cout << "change the password to anything you'd like" << std::endl;

		std::string newPass = makePass();

		std::ofstream passwrodFile("tuna.txt");

		if (passwrodFile.is_open()) {
			passwrodFile << newPass << "\n";

			passwrodFile.close();
		}


	}

	//################################
	//#                              #
	//#         Answer = 2           #
	//#                              #
	//################################
	else if (answerMenu == 2) {
		system("cls");

		LoadShips();
		ResetBoard();

		for (int aplyr = 1; aplyr < 3; ++aplyr)
		{
			for (int thisShip = 0; thisShip < SHIP_TYPES; ++thisShip)
			{
				system("cls");
				DrawBoard(aplyr);
				std::cout << "\n";
				std::cout << "INSTRUCTIONS (Player " << aplyr << ")\n\n";
				std::cout << "You are about to place your ships.  Format should be:\n";
				std::cout << "Facing (0:Horizontal,1:Vertical), X (top-row) coords, Y (left-side) coords\n";
				std::cout << "Example: 0 7 2    This would place a ship beginning at X:7 Y:2 going horizontal\n\n";
				std::cout << "Ship to place: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length << "\n";
				std::cout << "Where do you want it placed? ";

				PLACESHIPS aShip;
				aShip.shipType.onGrid[0].X = -1;
				while (aShip.shipType.onGrid[0].X == -1)
				{
					aShip = UserInputShipPlacement();
				}

				aShip.shipType.length = ship[thisShip].length;
				aShip.shipType.name = ship[thisShip].name;

				player[aplyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isSHIP;

				for (int i = 1; i < aShip.shipType.length; ++i)
				{
					if (aShip.direction == HORIZONTAL) {
						aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i - 1].X + 1;
						aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i - 1].Y;
					}
					if (aShip.direction == VERTICAL) {
						aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i - 1].Y + 1;
						aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i - 1].X;
					}


					player[aplyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isSHIP;
				}

			}

		}

		//################################
		//#                              #
		//#        Playing game          #
		//#                              #
		//################################

		gameRunning = true;
		int thisPlayer = 1;
		do {

			int enemyPlayer;
			if (thisPlayer == 1) enemyPlayer = 2;
			if (thisPlayer == 2) enemyPlayer = 1;
			system("cls");
			DrawBoard(enemyPlayer);


			bool goodInput = false;
			int x, y;
			while (goodInput == false) {
				goodInput = UserInputAttack(x, y, thisPlayer);
			}


			if (player[enemyPlayer].grid[x][y] == isSHIP) player[enemyPlayer].grid[x][y] = isHIT;
			if (player[enemyPlayer].grid[x][y] == isWATER) player[enemyPlayer].grid[x][y] = isMISS;


			int aWin = GameOverCheck(enemyPlayer);
			if (aWin != 0) {
				gameRunning = false;
				break;
			}

			thisPlayer = (thisPlayer == 1) ? 2 : 1;
		} while (gameRunning);

		system("cls");
		std::cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";

		system("pause");

	}

	//################################
	//#                              #
	//#         Answer = 3           #
	//#                              #
	//################################
	else if (answerMenu == 3) {
		system("cls");
		std::cout << "bye, bye..." << std::endl;

	}

	//################################
	//#                              #
	//#       Invalid answer         #
	//#                              #
	//################################
	else {
		std::cout << "that is not a valid option, please try again ";
		std::cin >> answerMenu;
	}

	return 0;
}

//################################
//#                              #
//#           Board              #
//#                              #
//################################
bool GameOverCheck(int enemyPLAYER)
{
	bool winner = true;

	for (int w = 0; w < BOARD_WIDTH; ++w) {
		for (int h = 0; h < BOARD_HEIGHT; ++h) {
			if (player[enemyPLAYER].grid[w][h] = isSHIP)
			{
				winner = false;
				return winner;
			}
		}
	}
	return winner;
}

//################################
//#                              #
//#           Attack             #
//#                              #
//################################
bool UserInputAttack(int& x, int& y, int theplayer)
{
	std::cout << "\nPLAYER " << theplayer << ", ENTER COORDINATES TO ATTACK: ";
	bool goodInput = false;
	std::cin >> x >> y;
	if (x < 0 || x >= BOARD_WIDTH) return goodInput;
	if (y < 0 || y >= BOARD_HEIGHT) return goodInput;
	goodInput = true;
	return goodInput;
}

//################################
//#                              #
//#         Placement            #
//#                              #
//################################
PLACESHIPS UserInputShipPlacement()
{
	int d, x, y;
	PLACESHIPS tmp;
	tmp.shipType.onGrid[0].X = -1;
	std::cin >> d >> x >> y;
	if (d != 0 && d != 1) return tmp;
	if (x < 0 || x >= BOARD_WIDTH) return tmp;
	if (y < 0 || y >= BOARD_HEIGHT) return tmp;
	tmp.direction = (DIRECTION)d;
	tmp.shipType.onGrid[0].X = x;
	tmp.shipType.onGrid[0].Y = y;
	return tmp;
}

//################################
//#                              #
//#            Ships             #
//#                              #
//################################
void LoadShips()
{

	ship[0].name = "Cruiser"; ship[0].length = 2;
	ship[1].name = "Frigate"; ship[1].length = 3;
	ship[2].name = "Submarine"; ship[2].length = 3;
	ship[3].name = "Escort"; ship[3].length = 4;
	ship[4].name = "Battleship"; ship[4].length = 5;
}
void ResetBoard()
{
	for (int plyr = 1; plyr < 3; ++plyr)
	{
		for (int w = 0; w < BOARD_WIDTH; ++w) {
			for (int h = 0; h < BOARD_HEIGHT; ++h) {
				player[plyr].grid[w][h] = isWATER;
			}
		}
	}
}

//################################
//#                              #
//#           Board              #
//#                              #
//################################
void DrawBoard(int thisPlayer)
{
	std::cout << "PLAYER " << thisPlayer << "'s GAME BOARD\n";
	std::cout << "----------------------\n";

	std::cout << "   ";
	for (int w = 0; w < BOARD_WIDTH; ++w) {
		if (w < 10)
			std::cout << w << "  ";
		else if (w >= 10)
			std::cout << w << " ";
	}
	std::cout << "\n";


	for (int h = 0; h < BOARD_HEIGHT; ++h) {
		for (int w = 0; w < BOARD_WIDTH; ++w) {

			if (w == 0) std::cout << h << " ";
			if (w < 10 && w == 0) std::cout << " ";
			if (gameRunning == false) std::cout << player[thisPlayer].grid[w][h] << "  ";
			if (gameRunning == true && player[thisPlayer].grid[w][h] != isSHIP)
			{
				std::cout << player[thisPlayer].grid[w][h] << "  ";
			}
			else if (gameRunning == true && player[thisPlayer].grid[w][h] == isSHIP)
			{
				std::cout << isWATER << "  ";
			}
			if (w == BOARD_WIDTH - 1) std::cout << "\n";
		}
	}
}


