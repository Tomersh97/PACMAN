#ifndef Save_Mode_h
#define Save_Mode_h

#include "GameMode.h"

class failSaveFile {};			//exception in case of saving malfunction

//Class to handle saving to file
class save_mode  : public GameMode
{	
private:
	ofstream save_file_steps;
	ofstream save_file_result;
	
public:			//methods to handle saving to file
	void _init_mode()override;
	void save_fruit(const Position& fruit_pos, int fruit_number);
	void save_movement(string direction, typeEntity entity);
	void mode_deathtime(int gameTimer)override;
	void mode_finish(int gameTimer)override;
	void open_next_file()override;
	bool getIsSilent()const override { return false; };

	~save_mode()		//dtor
	{
		save_file_steps.close();
		save_file_result.close();
	}
};


#endif	//Save_Mode_h