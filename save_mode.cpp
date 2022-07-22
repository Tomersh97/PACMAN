#include "save_mode.h"


//opens files for saving data, validates if successfully opened
void save_mode::_init_mode()
{
	save_file_steps.open(get_file_using() + steps_type, ios::trunc);
	save_file_result.open(get_file_using() + result_type, ios::trunc);
	if (!save_file_steps.good() || !save_file_result.good())
		throw failSaveFile();
}


//depending on which entity is sent, saves the entity type and direction
void save_mode::save_movement(string direction_str, typeEntity entity)
{
	switch (entity)
	{
	case (typeEntity::fruit_entity):
		save_file_steps << fruit_str<<arg_delim << direction_str << action_delim;
		break;
	case (typeEntity::ghost_entity):
		save_file_steps << ghost_str<< arg_delim << direction_str << action_delim;
		break;
	case (typeEntity::pacman_entity):
		save_file_steps << pacman_str<< arg_delim << direction_str << action_delim;
		break;
	}
}


//saves fruit appearance - [fruit num_fruit ypos xpos]
void save_mode:: save_fruit(const Position& fruit_pos, int fruit_number)
{
	save_file_steps << fruit_str<<arg_delim << fruit_number << arg_delim << fruit_pos.getPosY() << arg_delim << fruit_pos.getPosX() << action_delim;
}


//saves timer of death in current screen
void save_mode::mode_deathtime(int gameTimer)
{
	save_file_result << timer_death_str << gameTimer <<endl;
}


//saves timer of current finished screen
void save_mode::mode_finish(int gameTimer)
{
	save_file_result << timer_finish_str << gameTimer <<endl;
}


//closes files currently worked on and iniates opening of next files
void save_mode::open_next_file()
{
	save_file_result.close();
	save_file_steps.close();
	_init_mode();
}