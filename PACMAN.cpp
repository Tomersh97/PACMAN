#include "PACMAN.h"


// transates hit key to direction
void PACMAN::setDirectionFromKey(char dirKey)
{
	switch (dirKey)
	{
	case 'W':
	case 'w':
		setEntityDir(entityMoveDir::NORTH);
		break;

	case 'X':
	case 'x':
		setEntityDir(entityMoveDir::SOUTH);
		break;

	case 'D':
	case 'd':
		setEntityDir(entityMoveDir::EAST);
		break;

	case 'A':
	case 'a':
		setEntityDir(entityMoveDir::WEST);
		break;
	case 'S':
	case 's':
		setEntityDir(entityMoveDir::noDir);
		break;
	}
}


void PACMAN::move(gameBoard& board, GameMode* mode, ModeType mode_type)
{
	if (mode_type == ModeType::load || mode_type == ModeType::load_silent)
		setDirFromLoad(((load_mode*)mode)->read_action(typeEntity::pacman_entity));
		
	move_pacman(board, mode_type);

	if (mode_type == ModeType::save)	
		((save_mode*)mode)->save_movement(convertDirToString(), typeEntity::pacman_entity);	
}


//moves pacman depending on entityMoveDir. check the type movement made and if it is valid.
void PACMAN::move_pacman(gameBoard& board, ModeType mode_type)
{
	Position tmpPos = getEntityPos();
	gotoxy(getEntityPos().getPosX(), getEntityPos().getPosY());
	cout << " ";
	switch (getEntityMoveDir())
	{
	case entityMoveDir::NORTH:
		setEntityPos(getEntityPos().getPosY() - 1, getEntityPos().getPosX());
		setValidPACMANmovement(getEntityPos().getPosY(), -1, board, tmpPos, board.getBoardRow() - 1, false);
		break;

	case entityMoveDir::SOUTH:
		setEntityPos(getEntityPos().getPosY() + 1, getEntityPos().getPosX());
		setValidPACMANmovement(getEntityPos().getPosY(), board.getBoardRow(), board, tmpPos, 0, false);
		break;

	case entityMoveDir::WEST:
		setEntityPos(getEntityPos().getPosY(), getEntityPos().getPosX() - 1);
		setValidPACMANmovement(getEntityPos().getPosX(), -1, board, tmpPos, board.getBoardCol() - 1, true);
		break;

	case entityMoveDir::EAST:	
		setEntityPos(getEntityPos().getPosY(), getEntityPos().getPosX() + 1);
		setValidPACMANmovement(getEntityPos().getPosX(), board.getBoardCol(), board, tmpPos, 0, true);
		break;
	};
	board.setPACMANpos(getEntityPos());
	printEntityAtPos(mode_type);		// move pacman to new updated position
}


// Check if in this move there is a food in pacman's way
bool  PACMAN::isPacmanEatFood(gameBoard& board)const
{
	return (board.getCellBoardSymbol(getEntityPos()) == gameBoard::FOOD_BOARD);
}


//returns true if pacman and fruit at the same position
bool PACMAN::isPacmanEatFruit(gameBoard& board)const
{
	return(board.checkIfFruitAtCell(getEntityPos()));
}


// Check if in this move there is a ghost in pacman's way
bool PACMAN::isPacmanHitsGhost(gameBoard& board)
{
	return(board.checkIfGhostAtCell(getCurrPos()));
}


// Check if in this move there is a wall in pacman's way
bool PACMAN::checkWall(gameBoard& board, int yOffset, int xOffset)const
{	
	return (board.getCellBoardSymbol(getEntityPos().getPosY(), getEntityPos().getPosX(), yOffset, xOffset) != gameBoard::WALL);
}


// Reset pacman's positon to the initial state
void PACMAN::reset(gameBoard& board, ModeType isSilent)
{
	if (isSilent != ModeType::load_silent)
		board.printSymbolAtPos(getEntityPos(), (int)gameColors::foodColor);
	setEntityPos(board.getEntityPosOnBoard((int)entityNumOnPosVec::pacmanNum)); // reset pacman's pos
	printEntityAtPos(isSilent);
}


//checks movement and updates pacmans position
void PACMAN:: setValidPACMANmovement(int entityPosXY, int compareXY, gameBoard &board, Position tmpPos, int setter, bool isX)
{
	if (checkWall(board, 0, 0))				
	{
		if (entityPosXY == compareXY)
		{
			if (isX)
				setEntityPos(getEntityPos().getPosY() , setter);
			else
				setEntityPos(setter, getEntityPos().getPosX());
			if (board.getCellBoardSymbol(getEntityPos()) == gameBoard::WALL) //checks if other side is tunnel
				setEntityPos(tmpPos);
		}
	}
	else
		setEntityPos(tmpPos);	
}

