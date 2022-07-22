#include "Ghosts.h"
#include "Colors.h"


// Reset ghosts data when players loses a life
void Ghosts::reset(gameBoard& board, ModeType isSilent)
{
	board.setCellBoardGhost(getEntityPos(), false);							//sets no ghost at cell on the board
	if (isSilent != ModeType::load_silent)
		board.printSymbolAtPos(getEntityPos().getPosY(), getEntityPos().getPosX(), Colors::getFoodColor());		
}


//polymorphic movement function for ghost
void Ghosts::move(gameBoard& board, GameMode* mode, ModeType mode_type)
{	
	if (mode_type == ModeType::load || mode_type == ModeType::load_silent)	
	{
		setDirFromLoad(((load_mode*)mode)->read_action(typeEntity::ghost_entity));
		moveEntityFromLoad(board, typeEntity::ghost_entity, mode_type);
	}
	else	//simple or save mode
	{
		move_ghost(board, mode_type);		
		if (mode_type == ModeType::save)
			((save_mode*)mode)->save_movement(convertDirToString(), typeEntity::ghost_entity);
	}
}


// "main" function for ghost movment.
void  Ghosts::move_ghost(gameBoard& board, ModeType mode_type)
{
	if (difficulty == ghostType::NOVICE)
		noviceGhostMove(board);

	else if (difficulty == ghostType::SMART)
		smartGhostMove(board, board.getPACMANpos());

	else // difficulty == ghostType::GOOD
	{
		if (movesCounter > 5)
			smartGhostMove(board, board.getPACMANpos());

		else // for 5 steps move like novice ghost
		{
			if (movesCounter != 0)
				noviceGhostMove(board);

			else	// semiSmartGhost_SmartMovesCounter == 0
			{
				movesCounter = ((rand() % 10) + (int)difficultyTimer::goodGhostTimer);
			}
		}
		movesCounter--;
	}
}


// Check if the ghost can stay in it's same direction.
bool Ghosts::IsSameDirValid(gameBoard& board, entityMoveDir currGhostDir)const
{
	bool res = false;

	if (currGhostDir == entityMoveDir::NORTH)
		if (notWallOrTunnel (board, (int)offsets::upOffset, (int)offsets::noOffset))
			res = true;
	if (currGhostDir == entityMoveDir::SOUTH)
		if (notWallOrTunnel (board, (int)offsets::downOffset, (int)offsets::noOffset))
			res = true;
	if (currGhostDir == entityMoveDir::WEST)
		if (notWallOrTunnel (board, (int)offsets::noOffset, (int)offsets::leftOffset))
			res = true;
	if (currGhostDir == entityMoveDir::EAST)
		if (notWallOrTunnel (board, (int)offsets::noOffset, (int)offsets::rightOffset))
			res = true;

	return res;
}


// Casing function for the novice ghost's move.
void Ghosts::noviceGhostMove(gameBoard& board)
{
	board.setCellBoardEntity(getEntityPos(), typeEntity::ghost_entity, false);
	entityMoveDir currGhostDir = getEntityMoveDir();
	board.printSymbolAtPos(getEntityPos().getPosY(), getEntityPos().getPosX(), Colors::getFoodColor());
	if (movesCounter != goodGhost_novice && IsSameDirValid(board, currGhostDir))
	{
		entityDirection(currGhostDir);
		movesCounter--;
	}
	else
	{
		checkAndMoveEntity(board, typeEntity::ghost_entity);
		movesCounter = goodGhost_smart;
	}
	board.setCellBoardEntity(getEntityPos(), typeEntity::ghost_entity, true);
}


// This function is for the "smart" ghost movement - using the currect location of the pacman.
void Ghosts::smartGhostMove(gameBoard& board,const Position& pacmanPos)
{
	int deltaX, deltaY, pacmanVerticalToGhost, pacmanHorizontalToGhost, index = 0;
	const Position ghostCurrPos = getEntityPos();
	board.setCellBoardGhost(ghostCurrPos, false);
	entityMoveDir currGhostDir = getEntityMoveDir();
	entityMoveDir possibleDirArray[DIRECTIONS];

	pacmanHorizontalToGhost = pacmanPos.getPosX() - ghostCurrPos.getPosX();
	pacmanVerticalToGhost = pacmanPos.getPosY() - ghostCurrPos.getPosY();

	deltaX = std::abs(pacmanHorizontalToGhost);
	deltaY = std::abs(pacmanVerticalToGhost);

	board.printSymbolAtPos(ghostCurrPos.getPosY(), ghostCurrPos.getPosX(), Colors::getFoodColor());

	if ((pacmanVerticalToGhost < 0) && checkIfDirValid(board, (int)offsets::upOffset, (int)offsets::noOffset, entityMoveDir::SOUTH))
		possibleDirArray[index++] = entityMoveDir::NORTH;
	
	if ((pacmanVerticalToGhost > 0) && checkIfDirValid(board, (int)offsets::downOffset, (int)offsets::noOffset, entityMoveDir::NORTH))
		possibleDirArray[index++] = entityMoveDir::SOUTH;
	
	if ((pacmanHorizontalToGhost > 0) && checkIfDirValid(board, (int)offsets::noOffset, (int)offsets::rightOffset, entityMoveDir::WEST))
		possibleDirArray[index++] = entityMoveDir::EAST;

	if ((pacmanHorizontalToGhost < 0) && checkIfDirValid(board, (int)offsets::noOffset, (int)offsets::leftOffset, entityMoveDir::EAST))
		possibleDirArray[index++] = entityMoveDir::WEST;

	if (index > 0)
	{
		setEntityDir(randEntityDir(possibleDirArray, index));
		entityDirection(getEntityMoveDir());
	}
	else
		smartGhostMoveNotValid(board, currGhostDir);

	board.setCellBoardGhost(getEntityPos(), true);
}


 // check if direction is valid.
bool Ghosts::checkIfDirValid(gameBoard& board, int yOffset, int xOffset, entityMoveDir oppositeDirection)
{
	if (notWallOrTunnel (board, yOffset, xOffset))		//if not wall
	{
		if (getEntityMoveDir() == entityMoveDir::noDir)  
			return true;
		if (getEntityMoveDir() != oppositeDirection)	 
		{
			Position checkingPos = getEntityPos();
			checkingPos.setPosX(getEntityPos().getPosX()+xOffset);
			checkingPos.setPosY(getEntityPos().getPosY() +yOffset);
			if (!board.checkIfGhostAtCell(checkingPos))
				return true;
		}
	}
	return false;
}


 //pick a direction for the ghost when it's two smart directions to move are not valid.
void Ghosts::smartGhostMoveNotValid(gameBoard& board, entityMoveDir& currGhostDir)
{
	if (IsSameDirValid(board, currGhostDir))
		entityDirection(currGhostDir);
	else
		checkAndMoveEntity(board, typeEntity::ghost_entity);
}


