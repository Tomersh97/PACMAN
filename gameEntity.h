#ifndef GameEntity_h
#define GameEntity_h

#include "Position.h"
#include "game_board.h"
#include "save_mode.h"
#include "load_mode.h"

enum class entityMoveDir { NORTH, SOUTH, EAST, WEST, noDir = 5 };	

class gameEntity
{
public:
	gameEntity() :entity_symbol(' '),entityDir(entityMoveDir::noDir) {};

private: 
	Position		 entityPos;
	char		     entity_symbol;
	static const int DIRECTIONS = 4;
	entityMoveDir	 entityDir;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
		
public:
	//methods to handle available directions and movement 
	virtual void move(gameBoard& board, GameMode* mode, ModeType mode_type) = 0;
	virtual void reset(gameBoard& board,ModeType isSilent) = 0;
	void entityDirection(entityMoveDir direction);
	bool notWallOrTunnel (gameBoard& board, int yOffset, int xOffset)const;		
	void checkAndMoveEntity(gameBoard& board, typeEntity typeEntity);
	entityMoveDir randEntityDir(const entityMoveDir* dirArray, int numAvailDirections)const;
	entityMoveDir oppositeDir(entityMoveDir prevDir)const;
	void setDirFromLoad(const string& direction);
	void moveEntityFromLoad(gameBoard& board, typeEntity type, ModeType isSilent);

	//getters and setters
	HANDLE getEntityConsole()const { return console; };
	void setEntityDir(entityMoveDir moveDir) { entityDir = moveDir; };
	void setEntityPos(const Position& position) { entityPos = position; };
	void setEntityPos(int yPos, int xPos) { entityPos.setPosY(yPos);  entityPos.setPosX(xPos); };
	Position& getEntityPos() { return entityPos; };
	Position getEntityPos()const { return entityPos; };
	void setEntitySymbol(char symbol) { entity_symbol = symbol; };
	char getEntitySymbol()const { return entity_symbol; };
	entityMoveDir getEntityMoveDir()const { return entityDir; };
	virtual int getEntityColor()const = 0;

	string convertDirToString()const;

	void printEntityAtPos(ModeType check_silent)const;
	void printEntityAtPos()const;
};

#endif	//GameEntity_h