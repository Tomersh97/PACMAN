#ifndef Position_h
#define Position_h


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using std::endl;
using std::cout;
using std::vector;


/*Class holds position attributes for game objects*/
void gotoxy(int x, int y);

enum class offsets { rightOffset = 1, leftOffset = -1, upOffset = -1, downOffset = 1, noOffset = 0 };

class Position
{
private:
	int xPos;
	int yPos;

public:
	Position() :xPos((int)offsets::noOffset), yPos((int)offsets::noOffset) {};
	Position(int _x, int _y) : xPos(_x), yPos(_y) {} 

	//Get and set methods for position:
	int  getPosX()const { return xPos; };
	int  getPosY() const { return yPos; };
	void  setPosX(int xCoord) { xPos = xCoord; };
	void  setPosY(int yCoord) { yPos = yCoord; };
};


#endif	// Position_h