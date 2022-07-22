#ifndef Load_Mode_h
#define Load_Mode_h

#include "GameMode.h"

using std::cin;
using std::getline;

//Exceptions in case of loading malfunction
class failedLoad_unexpectedLoadedMove {};
class failedLoad_EOF {};
class failedLoadFile {};
class failedTest {};

//global function to check if a string is a number
bool isStrNumber(const string& string);

//class handles loading a game stored in .steps file
class load_mode : public GameMode
{
private:
	ifstream load_steps;
	ifstream load_result;
	bool isSilent = false;		

	void validate_entity(const string& entity_type, typeEntity type)const;
	void validate_action(const string& action)const;
	void validate_fruit_num(const string& fruitNum, int& fruit_num)const;
	void validate_yPos(const string& yPos, Position& fruit_pos, gameBoard& board)const;
	void validate_xPos(const string& xPos, Position& fruit_pos, gameBoard& board)const;
	bool validate_load()const;
	
public:
	const int empty_Str_size = 0;
	void _init_mode()override;
	string read_action(typeEntity type);
	void mode_finish(int gameTimer)override;
	void mode_deathtime(int gameTimer)override;
	void open_next_file()override;
	void setIsSilent(bool silent) { isSilent = silent; };
	bool getIsSilent()const override { return isSilent; };
	bool check_if_fruit(Position& fruit_pos, int& fruit_num, gameBoard& board);
	bool isFruitStillOnBoard();


	~load_mode()
	{
		if (isSilent)
			load_result.close();
		load_steps.close();
	}
};

#endif	// Load_Mode_h