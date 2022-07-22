#ifndef Cell_h
#define Cell_h

#include "Position.h"

//Cell class holds a single tile on the game board
class Cell
{
public:
	Cell() :cellSymbol(' ')  {}
	Cell(int xCellPos, int yCellPos) { cellSymbol = ' ';  }
	
private:
	char cellSymbol;				//Symbol at cell board
	bool isGhost = false;			//Is there a ghost at the cell
	bool isFruit = false;
	bool isMoveablePos = false;
	
public: 
	char get_cellSymbol()const { return cellSymbol; };
	void setcellSymbol(char symbol) { cellSymbol = symbol; };
	void setCellGhost(bool ghostAtPos) { isGhost = ghostAtPos; };
	bool getCellGhost() const { return isGhost; };
	bool getCellFruit()const { return isFruit; };
	void setCellFruit(bool setFruit) { isFruit = setFruit; };
	void setCellMoveablePos() { isMoveablePos = true; };
	bool getMoveablePos()const { return isMoveablePos; }
};


#endif	// Cell_h