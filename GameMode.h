#ifndef Game_Mode_h
#define Game_Mode_h

#include <string>
#include "game_board.h"

using std::string;

enum class ModeType{simple, save, load, load_silent};

//class to handle different game modes
class GameMode
{
private:
	string file_name;

public:
	const string dir_up = "UP", dir_down = "DOWN", dir_left = "LEFT", dir_right = "RIGHT", no_dir = "STAY";
	const string steps_type = ".steps", result_type = ".result";
	const string timer_death_str = "Time of pacman death: ", timer_finish_str = "Screen finish time:";
	const string fruit_str = "FRUIT", pacman_str = "PACMAN", ghost_str = "GHOST";
	const char action_delim = '|', arg_delim = ' ';

	virtual void mode_deathtime(int gameTimer) = 0;
	virtual void mode_finish(int gameTimer) = 0;
	virtual void _init_mode() = 0;
	virtual bool getIsSilent()const = 0;
	virtual void open_next_file() = 0;
	void set_file_using(string name);
	string get_file_using() { return file_name; }

	~GameMode()
	{
		delete this;
	}
};

#endif //Game_Mode_h