#ifndef Menu_h
#define Menu_h

#include <conio.h>
#include <windows.h>
#include <iostream>
#include "game_board.h"
#include "Ghosts.h"
#include "GameMode.h"

using std::endl;
using std::cout;
using std::getline;
using std::cin;

//class to handle user input 
class invalidProgramArguments {};

class Menu
{
public:
	enum class choices { choice_game = (int)'1', choice_instructions = (int)'8', choice_exit = (int)'9', choice_screen = (int)'2' };
	void init_main_menu(gameBoard& board)const;
	void view_instructions() const;
	void choice_validation(char& choice, gameBoard& board)const;
	ghostType readGhostDifficulty(ModeType type)const;
	void decideIfColors(ModeType type)const;
	ModeType validateSet_GameMode (int argc, char** argv)const;
};

#endif //Menu_h