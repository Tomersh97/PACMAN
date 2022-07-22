#ifndef Ghosts_h
#define Ghosts_h

#include "Position.h"
#include "gameEntity.h"

enum class ghostType {NOVICE, GOOD, SMART, LOAD_MODE};
enum class difficultyTimer{goodGhostTimer = 20};

/* Class handles ghost movement and behaviour. */
class Ghosts : public gameEntity
{
public:
	Ghosts() :difficulty(ghostType::NOVICE), movesCounter(0) {}
private:	
	static const int DIRECTIONS = 4;
	const int goodGhost_novice = 0;
	const int goodGhost_smart = 20;
	int movesCounter;
	ghostType difficulty;

	//methods to handle movement statements
	void		smartGhostMoveNotValid(gameBoard& board, entityMoveDir& currGhostDir);
	bool		checkIfDirValid(gameBoard& board, int yOffset, int xOffset, entityMoveDir oppositeDirection);	
	void		smartGhostMove(gameBoard& board, const Position& pacmanPos);
	void		noviceGhostMove(gameBoard& board);
	bool		IsSameDirValid(gameBoard& board, entityMoveDir currGhostDir)const;

	//getters and setters
	int			getSemiSmartGhost_SmartMovesCounter()const { return movesCounter; };
	int			getEntityColor()const override { return Colors::getGhostColor(); };
	void		setSemiSmartGhost_SmartMovesCounter(int numOfSmartMoves) { movesCounter = numOfSmartMoves; };
	void		move_ghost(gameBoard& board, ModeType mode_type);	

public: 	
	//polymorphic functions to handle movement and reset
	void		reset(gameBoard& board, ModeType isSilent)override;
	void		move(gameBoard& board, GameMode* mode, ModeType mode_type)override;

	//diffuclty getters and setters
	void		setGhostDifficulty(ghostType ghostLevel) { difficulty = ghostLevel; };
	ghostType	getGhostDifficulty()const { return difficulty; };
};

#endif	// Ghosts_h