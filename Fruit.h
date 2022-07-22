#ifndef fruit_h
#define fruit_h

#include "Position.h"
#include "gameEntity.h"

enum class fruit_num { fruitFive = 5, fruitSix = 6, fruitSeven = 7, fruitEight = 8, fruitNine = 9, noFruit = 0 };

class Fruit : public gameEntity
{
public:
	Fruit() : isFruitOnBoard(false), fruitNumber(fruit_num::noFruit),whenSpawnFruit(0),whenKillFruit(0){};
private:
	bool		 isFruitSpawnTimerSet = false;
	bool		 isFruitDeathTimerSet = false;
	bool		 isFruitOnBoard = false;
	fruit_num	 fruitNumber;
	int			 whenSpawnFruit;
	int			 whenKillFruit;
	const int	 fivePossibleFruit = 5;				
	const int    fruitTime_spawn= 20;
	const int    fruitTime_kill = 45;	
	
	//fruit stat random generating methods
	void generateFruitNumber();
	void generateFruitPos(gameBoard& board) { setEntityPos(board.getRandAvailablePos()); };
	void randFruitAppearsTime(int gameTimer);
	void randFruitDeathTime(int gameTimer);
	void checkGeneratedSymbol();
	void convertIntToFruitNum(int fruit_num);

	//private getters setters
	void setFruitStartingStats(gameBoard& board, GameMode* mode, ModeType type);
	int  getWhenFruitSpawn()const { return whenSpawnFruit; };
	int getWhenFruitDies()const { return whenKillFruit; };
	int  getEntityColor()const override { return Colors::getFruitColor(); };
	
	//checkers
	void FruitOnBoardChecks(gameBoard& board, GameMode* mode, ModeType mode_type, int gameTimer);
	void checkFruitHitGhost(gameBoard& board, ModeType mode_type);
	void checkAndSetGameTimer(int gameTimer);
	
public:
	//occurances with fruit in the game
	fruit_num getFruitNum()const { return fruitNumber; };
	bool getFruitOnBoard()const { return isFruitOnBoard; };
	void fruitOccurances(gameBoard& board, int gameTimer, GameMode* mode, ModeType mode_type);
	void reset(gameBoard& board, ModeType isSilent)override;
	void move(gameBoard& board, GameMode* mode, ModeType mode_type)  override;
};

#endif // !fruit_h