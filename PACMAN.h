#ifndef PACMAN_h
#define PACMAN_h

#include "Position.h"
#include "gameEntity.h"

/*Class handles pacman's attributes - movement and on board occurances*/
class PACMAN : public gameEntity
{
public:
	PACMAN() : lives(PACMAN_LIVES) 	{ setEntitySymbol('@'); };
	enum pacman_stats { PACMAN_LIVES = 3, NO_LIVES = 0 };

private:
	int lives;

	bool checkWall(gameBoard& board, int yOffset, int xOffset)const;
	void setValidPACMANmovement(int entityPosXY, int compareXY, gameBoard& board, Position tmpPos, int setter, bool isX);
	int getEntityColor()const override { return Colors::getPacmanColor(); };
	void move_pacman(gameBoard& board, ModeType mode_type);
	
public:		
	//methods to change pacman's attributes:
	void move(gameBoard& board, GameMode* mode, ModeType mode_type)override;
	void setPacmanPos(Position pacPos) { setEntityPos(pacPos); };
	Position& getCurrPos() { return getEntityPos(); };
	Position getCurrPos()const { return getEntityPos(); };
	int getPacmanLives()const { return lives; };
	void setPacmanLives() { --lives; };
	void resetLives() { lives = PACMAN_LIVES; };
	virtual void reset(gameBoard& board, ModeType isSilent)override;

	//checks pacman occurances at board
	bool isPacmanEatFood(gameBoard& board)const;
	bool isPacmanEatFruit(gameBoard& board)const;
	bool isPacmanHitsGhost(gameBoard& board);
	void setDirectionFromKey(char dirKey);
};

#endif	// PACMAN_h