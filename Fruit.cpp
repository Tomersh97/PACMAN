#include "Fruit.h"

// set fruit symbol according to generated fruit number
void Fruit::checkGeneratedSymbol()
{
	switch (fruitNumber)
	{
	case fruit_num::fruitFive:
		setEntitySymbol('5');  
		break;
	case fruit_num::fruitSix:
		setEntitySymbol('6');
		break;
	case fruit_num::fruitSeven:
		setEntitySymbol('7');
		break;
	case fruit_num::fruitEight:
		setEntitySymbol('8');
		break;
	case fruit_num::fruitNine:
		setEntitySymbol('9');
		break;
	}
}


// set all the starting data that needed for the fruit.
void Fruit:: setFruitStartingStats(gameBoard &board, GameMode* mode, ModeType type)
{
	generateFruitNumber();
	generateFruitPos(board);
	checkGeneratedSymbol();
	isFruitOnBoard = true;
	printEntityAtPos(type);
	if (type == ModeType::save)
		((save_mode*)mode)->save_fruit(getEntityPos(), (int)fruitNumber);
}


//move function of polymorphic entity move function
void Fruit:: move(gameBoard& board, GameMode* mode, ModeType mode_type)
{	
	if (mode_type == ModeType::load || mode_type == ModeType::load_silent)
	{
		setDirFromLoad(((load_mode*)mode)->read_action(typeEntity::fruit_entity));
		moveEntityFromLoad(board, typeEntity::fruit_entity, mode_type);
	}

	else  //mode is simple or save
	{
		checkAndMoveEntity(board, typeEntity::fruit_entity);
		if (mode_type == ModeType::save)
			((save_mode*)mode)->save_movement(convertDirToString(), typeEntity::fruit_entity);
	}
}


//sets spawn and death timer states if are not set
void Fruit::checkAndSetGameTimer(int gameTimer)
{
	if (!isFruitSpawnTimerSet)
	{
		randFruitAppearsTime(gameTimer);
		isFruitSpawnTimerSet = true;
	}
	if (!isFruitDeathTimerSet)
	{
		randFruitDeathTime(gameTimer);
		isFruitDeathTimerSet = true;
	}	
}


// Reset all fruit's data (no fruit now on board).
void Fruit:: reset(gameBoard &board, ModeType isSilent)
{
	whenSpawnFruit = 0;
	whenKillFruit = 0;
	fruitNumber = fruit_num::noFruit;
	setEntitySymbol(' ');	 
	if (isSilent != ModeType::load_silent && isFruitOnBoard)
		board.printSymbolAtPos(getEntityPos(), Colors::getFoodColor());
	isFruitOnBoard = false;
	isFruitSpawnTimerSet = false;
	isFruitDeathTimerSet = false;
	board.setCellBoardFruit(getEntityPos(), false);
}


// Randomize the number for the fruit (between the given values).
void Fruit::generateFruitNumber()
{
	fruitNumber = (fruit_num)((rand() % fivePossibleFruit) + fivePossibleFruit);
}


// Randomize time for the fruit to appear.
void Fruit::randFruitAppearsTime(int gameTimer)
{
	whenSpawnFruit = (rand() % fruitTime_spawn) + fruitTime_spawn;
	whenSpawnFruit += gameTimer;
}


// Randomize life time when fruit is created.
void Fruit:: randFruitDeathTime(int gameTimer)
{
	whenKillFruit = (rand() % fruitTime_kill) + fruitTime_kill;
	whenKillFruit += whenSpawnFruit;
}


// Check if there is a ghost in the new fruit posotion
void Fruit::checkFruitHitGhost(gameBoard& board, ModeType mode_type)
{
	if (board.checkIfGhostAtCell(getEntityPos()))
	{ 
		reset(board, mode_type);			//resets fruit stats
		if (mode_type != ModeType::load_silent)
		{
			gotoxy(getEntityPos().getPosX(), getEntityPos().getPosY());
			SetConsoleTextAttribute(getEntityConsole(), Colors::getGhostColor());
			cout << gameBoard::GHOST_SYMBOL;
			SetConsoleTextAttribute(getEntityConsole(), (int)gameColors::whiteColor);
		}
	}
}


// Check fruit cases
void Fruit:: fruitOccurances(gameBoard& board, int gameTimer, GameMode* mode, ModeType mode_type)
{		
	if (mode_type == ModeType::load || mode_type == ModeType::load_silent)
	{
		Position tmpPos;
		int fruitNum;
		if (!isFruitOnBoard)
		{
			isFruitOnBoard = ((load_mode*)mode)->check_if_fruit(tmpPos, fruitNum, board);
			if (isFruitOnBoard)
			{
				setEntityPos(tmpPos);
				convertIntToFruitNum(fruitNum);
				return;
			}
		}
	}
	else		//if mode is simple or save
	{
		checkAndSetGameTimer(gameTimer);		//sets fruit spawning and death time
		if (gameTimer == whenSpawnFruit)
		{ 
			setFruitStartingStats(board, mode, mode_type);		//sets stats at spawn
			return;
		}
	}	
	FruitOnBoardChecks(board, mode, mode_type, gameTimer);
}


//performs actions and checks when fruit is on the board
void Fruit::FruitOnBoardChecks(gameBoard &board, GameMode* mode, ModeType mode_type, int gameTimer)
{
	if (isFruitOnBoard)
	{
		if (gameTimer == whenKillFruit && mode_type != ModeType::load)
			reset(board, mode_type);					//resets fruit - kills fruit
		
		checkFruitHitGhost(board, mode_type);			//check collision with ghost
		if (gameTimer % 2 == 0 && isFruitOnBoard)
		{
			if (mode_type == ModeType::load || mode_type == ModeType::load_silent)
				isFruitOnBoard = ((load_mode*)mode)->isFruitStillOnBoard();
			if (isFruitOnBoard)
				move(board, mode, mode_type);//moves fruit
			else
				reset(board, mode_type);
		}
		checkFruitHitGhost(board, mode_type);
	}
}


//converts integer to fruit_num type
void Fruit::convertIntToFruitNum(int fruitnum)
{
	switch (fruitnum)
	{
	case 5:
		fruitNumber = fruit_num::fruitFive;
		setEntitySymbol('5');
		break;
	case 6:
		fruitNumber = fruit_num::fruitSix;
		setEntitySymbol('6');
		break;
	case 7:
		fruitNumber = fruit_num::fruitSeven;
		setEntitySymbol('7');
		break;
	case 8:
		fruitNumber = fruit_num::fruitEight;
		setEntitySymbol('8');
		break;
	case 9:
		fruitNumber = fruit_num::fruitNine;
		setEntitySymbol('9');
		break;
	}
}