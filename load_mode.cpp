#include "load_mode.h"

//attempts to open files for loading.
void load_mode::_init_mode()
{
	load_steps.open(get_file_using() + steps_type);
	if (!load_steps.good())
		throw failedLoadFile();	

	if (isSilent)			//if silent mode
	{
		load_result.open(get_file_using() + result_type);
		if (!load_result.good())
			throw failedLoadFile();
	}
}


//closes current files and opens next for loading
void load_mode::open_next_file()
{
	if (isSilent)
		load_result.close();
	load_steps.close();
	_init_mode();
}


//read single action of entity
string load_mode::read_action(typeEntity type)
{
	int ind=0;
	if (load_steps.eof())
		throw failedLoad_EOF();
	string action_str, action, entity_type;
	getline(load_steps, action_str, action_delim);		//read entire action
	if (action_str.size() == empty_Str_size)
		throw failedTest();

	while (action_str[ind] != arg_delim)
		entity_type += action_str[ind++];		//read type entity
	validate_entity(entity_type, type);
	
	while (action_str[++ind] != '\0')			//read direction
		action += action_str[ind];
	validate_action(action);

	return action;
}


//check if a fruit has appeared and update its traits
bool load_mode::check_if_fruit(Position& fruit_pos, int& fruit_num, gameBoard& board)
{
	int ind = 0;
	string action_str, entity_type, fruitNum, yPos, xPos;
	getline(load_steps, action_str, action_delim);		//read entity and action
	if (load_steps.eof())
		throw failedLoad_EOF();
	while (action_str[ind] != arg_delim)				//read type
		entity_type += action_str[ind++];
	if (entity_type != fruit_str)					//if isnt fruit, roll back to previous file location
	{
		int loc_file = load_steps.tellg();
		int str_size = (action_str.size() + 1);
		load_steps.seekg(loc_file - str_size, ios::beg); 
		return false;
	}
	ind++;

	while (action_str[ind] != arg_delim && action_str[ind] != '\0')	//read fruit number
		fruitNum += action_str[ind++];
	validate_fruit_num(fruitNum, fruit_num);
	ind++;

	while (action_str[ind] != arg_delim)								//read y coord
		yPos += action_str[ind++];
	validate_yPos(yPos, fruit_pos, board);
	ind++;

	while (action_str[ind] != '\0')								//read x coord
		xPos += action_str[ind++];
	validate_xPos(xPos, fruit_pos, board);

	//ensures position in file is not in a wall or ghost (possible if file changed manually
	if (board.getCellBoardSymbol(fruit_pos) == gameBoard::WALL || board.checkIfGhostAtCell(fruit_pos))//equals to wall
		return false;
	return true;
}


//checks if the fruit still exists on the board
bool load_mode::isFruitStillOnBoard()
{
	const char fruit_letter = 'F';
	char ch;
	if (load_steps.eof())
		throw failedLoad_EOF();
	load_steps.get(ch);
	load_steps.seekg(-1, ios::cur);
	if (ch != fruit_letter)
		return false;
	else return true;
}


//validates if given y coord for fruit is valid
void load_mode::validate_yPos(const string& yPos, Position& fruit_pos, gameBoard &board)const
{
	const int min_board_ycoord = 0;
	if (isStrNumber(yPos))
	{
		int yCoord = std::stoi(yPos);
		if (yCoord >= board.getBoardRow() || yCoord < min_board_ycoord)
			throw failedLoadFile();
		else
			fruit_pos.setPosY(yCoord);
	}
	else
		throw failedLoadFile();
}


//validates if given x coord for fruit is valid
void load_mode::validate_xPos(const string& xPos, Position& fruit_pos, gameBoard& board)const
{
	const int min_board_xcoord = 0;
	if (isStrNumber(xPos))
	{
		int xCoord = std::stoi(xPos);
		if (xCoord >= board.getBoardCol() || xCoord < min_board_xcoord)
			throw failedLoadFile();
		else
			fruit_pos.setPosX(xCoord);
	}
	else
		throw failedLoadFile();
}


//validates if given entity is valid entity
void load_mode::validate_entity(const string& entity_type, typeEntity type)const
{
	if (type == typeEntity::pacman_entity && entity_type != pacman_str)
		throw failedLoad_unexpectedLoadedMove();

	else if (type == typeEntity::ghost_entity && entity_type != ghost_str)
		throw failedLoad_unexpectedLoadedMove();

	else if (type == typeEntity::fruit_entity && entity_type != fruit_str)
		throw failedLoad_unexpectedLoadedMove();
}


//validates if given number is valid for the fruit
void load_mode::validate_fruit_num(const string& fruitNum, int &fruit_num)const
{
	const string fruit_9 = "9", fruit_8 = "8", fruit_7 = "7", fruit_6 = "6", fruit_5 = "5";
	if (fruitNum != fruit_9 && fruitNum != fruit_8 && fruitNum != fruit_7 && fruitNum != fruit_6 && fruitNum != fruit_5)
		throw failedLoad_unexpectedLoadedMove();
	else
		fruit_num = std::stoi(fruitNum);
}


//validates if given string is a valid action
void load_mode::validate_action(const string& action)const
{
	if (action != dir_up && action != dir_down && action != dir_right && action != dir_left && action != no_dir)
		throw failedLoadFile();
}


//validates if successfully loaded
bool load_mode:: validate_load()const
{
	return(load_steps.fail()); 
}


//Loads time of screen finished from result file. If the given timer doesnt match the loaded, throws as failed test
void load_mode::mode_finish(int gameTimer)
{
	string gametime_str = std::to_string(gameTimer);
	const string game_string = timer_finish_str + gametime_str;
	string cmp_string;
	getline(load_result, cmp_string);
	if (game_string != cmp_string)
		throw failedTest();
}


//Loads time of death from result file. If the given timer doesnt match the loaded, throws as failed test
void load_mode::mode_deathtime(int gameTimer)
{
	string gametime_str = std::to_string(gameTimer);
	const string game_string = timer_death_str +gametime_str;
	string cmp_string;
	getline(load_result, cmp_string);
	if (game_string != cmp_string)
		throw failedTest();
}


//global func, checks if given string is a number
bool isStrNumber(const string& string)
{
	int ind = 0;
	for (ind; ind < string.size(); ind++)	//validate each character
	{
		if (!std::isdigit(string[ind]))
			return false;
	}
	return true;
}
