#ifndef GAME_h
#define GAME_h

#include "PACMAN.h"
#include "Ghosts.h"
#include "Menu.h"
#include "Fruit.h"
#include "save_mode.h"
#include "load_mode.h"

/*Main game class holds all game objects that function during gameplay*/
class Game 
{
public:
	Game(int argc, char ** argv);
	Game(const Game& game) = delete;
	Game& operator=(const Game & game) = delete;
	static constexpr int NUM_ENTITIES = 6;

private:
	gameEntity*			entities[NUM_ENTITIES];
	Menu				main_menu;
	gameBoard           board;
	bool                isInProgram;
	bool                isGamePaused;
	bool                isGameFinished;
	bool				isLoadSilent = false;
	int                 currScore;
	char				tempKey;
	char				dirKey;
	int					numBoardsWon;
	int					scoreToWin;
	int					gameTimer;
	int					numPhEntities;
	GameMode*			mode;
	ModeType            mode_type=ModeType::simple;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()			

	const int MAX_NUM_GHOSTS = 4;
	const int STARTING_SCORE = 0;
	const int ESC = 27;
	const int pacman_ind = 0;
	const int fruit_ind  = 1;
	const int ghost_ind =  2;
	
	//start and initialization of game methods
	void start_pacman();
	void prepareForGame();
	void initialize_game_entities(ghostType difficulty,bool isSilent);
	void init_entity_memory();


	//User movement related methods
	void await_start_movement();
	bool isDirKeyHit(char key)const;
	void check_movement();
	void  setTempKey(char hitKey) { tempKey = hitKey; };
	void  setDirKey(char key) { dirKey = key; };

	//methods regarding game occurances
	void checkTimelyOccurances();
	void checkGameOccurances();
	void resetEntities();
	void handle_timer()const;

	// Game pausing related methods
	void check_pausing(char tempKey, char dirKey);
	void setGamePaused(bool setPausedState) { isGamePaused = setPausedState; };
	bool getGamePaused() const { return isGamePaused; };

	// User score related methods
	void  checkPacmanScore();
	void  displayScore(bool isSilent);
	
	// Game ends related methods
	void  GameWinner_screen(bool isSilent)const;
	void  GameOver_screen()const;
	void  resetGame();

	void check_mode_start(int argc, char** argv);
	void init_pacman_game();

public:
	~Game() //game d'tor
	{	
		for (int i = 0; i < numPhEntities; i++)
			delete entities[i];
	}
}; 


#endif	// GAME_h