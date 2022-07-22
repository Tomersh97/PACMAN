#include "gameEntity.h"



// general function to move entity at a random direction
// creates direction array from possible directions which are not wall or frame
// randomizes the index of the array to receive random direction and sets it to the entity
void gameEntity::checkAndMoveEntity(gameBoard& board, typeEntity typeEntity)
{
	entityMoveDir newDir;
	int index = 0;
	entityMoveDir currEntityDir = entityDir;
	entityMoveDir possibleDirArray[DIRECTIONS];

	board.setCellBoardEntity(entityPos, typeEntity, false);			// set board ghost cell to false at old position

	board.printSymbolAtPos(entityPos.getPosY(), entityPos.getPosX(), Colors::getFoodColor());

	if (notWallOrTunnel (board, (int)offsets::upOffset, (int)offsets::noOffset ) && (currEntityDir != entityMoveDir::SOUTH))
		possibleDirArray[index++] = entityMoveDir::NORTH;
	if (notWallOrTunnel (board, (int)offsets::downOffset, (int)offsets::noOffset) && (currEntityDir != entityMoveDir::NORTH))
		possibleDirArray[index++] = entityMoveDir::SOUTH;
	if (notWallOrTunnel (board, (int)offsets::noOffset, (int)offsets::rightOffset) && (currEntityDir != entityMoveDir::WEST))
		possibleDirArray[index++] = entityMoveDir::EAST;
	if (notWallOrTunnel (board, (int)offsets::noOffset, (int)offsets::leftOffset) && (currEntityDir != entityMoveDir::EAST))
		possibleDirArray[index++] = entityMoveDir::WEST;

	newDir = randEntityDir(possibleDirArray, index);				//randomizes new position using dirArray
	entityDirection(newDir);										//moves the direction and sets the new pos
	board.setCellBoardEntity(entityPos, typeEntity, true);
}


//checks for entity if a direction is valid
bool gameEntity::notWallOrTunnel (gameBoard& board, int yOffset, int xOffset)const
{
	if (board.getCellBoardSymbol(entityPos, yOffset, xOffset) != gameBoard::WALL)	// check if next step is a wall
	{
		if (entityPos.getPosY() + yOffset != (board.getBoardRow()) &&				// check if any of the frames
			entityPos.getPosY() + yOffset != -1				  	   &&
			entityPos.getPosX() + xOffset != (board.getBoardCol()) &&
			entityPos.getPosX() + xOffset != -1)
			return true;
	}
	return false;
}


// Returns the opposite direction of the entity. 
entityMoveDir gameEntity::oppositeDir(entityMoveDir prevDir)const
{
	if (prevDir == entityMoveDir::NORTH)
		return entityMoveDir::SOUTH;
	else if (prevDir == entityMoveDir::SOUTH)
		return  entityMoveDir::NORTH;
	else if (prevDir == entityMoveDir::WEST)
		return  entityMoveDir::EAST;
	else  //(prevDir == entityMoveDir::EAST)
		return  entityMoveDir::WEST;
}


// Randomize one of the availble directions for the game entity.
entityMoveDir gameEntity::randEntityDir(const entityMoveDir* dirArray, int numAvailDirections)const
{
	int getDirIndex;

	if (numAvailDirections != 0)
	{
		getDirIndex = rand() % numAvailDirections;
		return dirArray[getDirIndex];
	}
	else	// not valid direction was found - go in the opposite direction.
		return oppositeDir(entityDir);
}


//sets new movement position and direction according to chosen dircetion
void gameEntity::entityDirection(entityMoveDir direction)
{
	switch (direction)					// according to direction - updates entity 
	{
		case entityMoveDir::NORTH:
			entityPos.setPosY(entityPos.getPosY() + (int)offsets::upOffset);
			break;
		case entityMoveDir::SOUTH:
			entityPos.setPosY(entityPos.getPosY() + (int)offsets::downOffset);
			break;
		case entityMoveDir::WEST:
			entityPos.setPosX(entityPos.getPosX() + (int)offsets::leftOffset);
			break;
		case entityMoveDir::EAST:
			entityPos.setPosX(entityPos.getPosX() + (int)offsets::rightOffset);
			break;
	};
	entityDir = direction;
	printEntityAtPos();
}


//prints entity's symbol at pos
void gameEntity::printEntityAtPos(ModeType check_silent)const
{
	if (check_silent != ModeType::load_silent)
	{
		gotoxy(entityPos.getPosX(), entityPos.getPosY());
		SetConsoleTextAttribute(console, (int)getEntityColor());
		cout << entity_symbol;
		SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
	}
}


//prints entity at given position
void gameEntity::printEntityAtPos()const
{
	gotoxy(entityPos.getPosX(), entityPos.getPosY());
	SetConsoleTextAttribute(console, (int)getEntityColor());
	cout << entity_symbol;
	SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
}


//converts string to direction 
void gameEntity::setDirFromLoad(const string& direction)
{
	const string dir_up = "UP", dir_down = "DOWN", dir_left = "LEFT", dir_right = "RIGHT", no_dir = "STAY";
	if (direction == dir_up)
		entityDir = entityMoveDir::NORTH;
	else if (direction == dir_down)
		entityDir = entityMoveDir::SOUTH;
	else if (direction == dir_left)
		entityDir = entityMoveDir::WEST;
	else if (direction == dir_right)
		entityDir = entityMoveDir::EAST;
	else if (direction == no_dir)
		entityDir = entityMoveDir::noDir;

}


//convert direction to string for files
string gameEntity::convertDirToString()const
{
	string direction_str;
	switch (entityDir)
	{
	case entityMoveDir::NORTH:
		direction_str = "UP";
		break;
	case entityMoveDir::SOUTH:
		direction_str = "DOWN";
		break;
	case entityMoveDir::WEST:
		direction_str = "LEFT";
		break;
	case entityMoveDir::EAST:
		direction_str = "RIGHT";
		break;
	case entityMoveDir::noDir:
		direction_str = "STAY";
		break;
	}
	return direction_str;
}


//function gets direction after loading and moves the entity according to the direction
void gameEntity::moveEntityFromLoad(gameBoard &board, typeEntity type, ModeType isSilent)
{
	board.setCellBoardEntity(entityPos, type, false);
	if(isSilent != ModeType::load_silent)
		board.printSymbolAtPos(getEntityPos().getPosY(), getEntityPos().getPosX(), Colors::getFoodColor());
	switch (entityDir)					
	{
	case entityMoveDir::NORTH:
		if (notWallOrTunnel(board, (int)offsets::upOffset, (int)offsets::noOffset))
			entityPos.setPosY(entityPos.getPosY() + (int)offsets::upOffset);
		break;
	case entityMoveDir::SOUTH:
		if (notWallOrTunnel(board, (int)offsets::downOffset, (int)offsets::noOffset))
			entityPos.setPosY(entityPos.getPosY() + (int)offsets::downOffset);
		break;
	case entityMoveDir::WEST:
		if (notWallOrTunnel(board, (int)offsets::noOffset, (int)offsets::leftOffset))
			entityPos.setPosX(entityPos.getPosX() + (int)offsets::leftOffset);
		break;
	case entityMoveDir::EAST:
		if (notWallOrTunnel(board, (int)offsets::noOffset, (int)offsets::rightOffset))
			entityPos.setPosX(entityPos.getPosX() + (int)offsets::rightOffset);
		break;
	};
	board.setCellBoardEntity(entityPos, type, true);
	printEntityAtPos(isSilent);
}