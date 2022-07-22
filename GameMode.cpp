#include "GameMode.h"

//erases .screen part from string
void GameMode::set_file_using(string name)
{
	const int erase_screen_size = 7;
	name.erase(name.end() - erase_screen_size, name.end());
	file_name = name;
}
