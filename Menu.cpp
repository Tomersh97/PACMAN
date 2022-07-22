#include "Menu.h"


//Main menu and validation methods
void Menu::  init_main_menu(gameBoard& board)const 
{
	system("cls");
	cout << "           Welcome to PACMAN!" << endl
		<< "Please choose one of the following options:" << endl
		<< "           1.Start a new game" << endl
		<< "      Current chosen board:" << board.displayCurrScreen() << endl
		<< "       2.Choose different game board" << endl
		<< "          8.View instructions" << endl
		<< "                9.Exit" << endl;
}


//shows instructions
void Menu::  view_instructions() const {
	system("cls");
	std::cout << "As PACMAN your mission is simple:" << endl
		<< "Navigate through the maze and eat as many breadcrumbs as possible! " << endl
		<< "There are ghosts hunting you and your job is to avoid them! " << endl
		<< "Every time the Pacman is eaten by the ghosts the Pacman loses one life." << endl
		<< "Notice! The Pac-Man has 3 lives. " << endl
		<< "If the Pacman has 0 lives - the game is OVER." << endl
		<< "Use the following keys on your keyboard to navigate through the maze!" << endl
		<< "D - Right      A - Left      W - Up      X - Down      S - Stay" << endl
		<< "* You can always pause and unpause your game using the ESC button." << endl
		<< "** Please make sure the your keyboard is set on English." << endl << endl
		<< "GOOD LUCK! :) " << endl << endl;
};


// User's choice validation
void Menu::choice_validation(char& choice, gameBoard& board)const
{
	while (choice != (int)choices::choice_game 
	    && choice != (int)choices::choice_instructions	
		&& choice != (int)choices::choice_exit 
		&& choice != (int)choices::choice_screen)
	{
		system("cls");
		init_main_menu(board);
		cout << "Please enter a valid choice!" << endl;
		choice = _getch();
	}
}


//get difficulty from user
ghostType Menu::readGhostDifficulty(ModeType type)const
{
	if (type == ModeType::load || type == ModeType::load_silent)
		return ghostType::LOAD_MODE;
	else
	{
		const char novice = '1', good = '2', smart = '3';
		system("cls");
		bool isGhostDiffChosen = false;
		char choice;
		cout << "Choose ghost difficulty:" << endl << "1. novice" << endl << "2. good" << endl << "3. smart" << endl;
		choice = _getch();

		do {
			if (choice != novice && choice != good && choice != smart)
			{
				cout << "Please enter a valid choice!" << endl;
				choice = _getch();
			}
		} while (choice != novice && choice != good && choice != smart);
		switch (choice)
		{
		case novice:
			return ghostType::NOVICE;
			break;
		case good:
			return ghostType::GOOD;
			break;
		case smart:
			return ghostType::SMART;
			break;
		}
	}
}


//Method checks user's choice for colors in game and validates the choice
void Menu::decideIfColors(ModeType type)	const			
{
	if (type != ModeType::load && type!=ModeType::load_silent)
	{
		system("cls");
		std::cout << "This screen is for you to decide if you want colors in the game" << endl
			<< "If you want colors press Y." << endl
			<< "If you don't want colors press N." << endl;

		char answer;
		answer = _getch();
		do {
			if (answer != 'Y' && answer != 'N' && answer != 'n' && answer != 'y')
			{
				cout << "Please enter a valid choice!" << endl;
				answer = _getch();
			}
		} while (answer != 'Y' && answer != 'N' && answer != 'n' && answer != 'y');

		if (answer == 'Y' || answer == 'y')
			Colors::withColors = TRUE;
		else
			Colors::withColors = FALSE;
	}
};


//gets program arguments. sets mode according to given arguments
ModeType Menu::validateSet_GameMode(int argc, char** argv)const
{
	if (argc == 1)
		return ModeType::simple;
	else if (argc == 2 )
	{
		if (strcmp(argv[1], "-save") == 0)
			return ModeType::save;
		else if (strcmp(argv[1], "-load") == 0)
			return ModeType::load;
		else 
			throw invalidProgramArguments();
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "-save") == 0&& strcmp(argv[2], "-silent") == 0)			
				return ModeType::save;
			
		else if (strcmp(argv[1], "-load") == 0 && strcmp(argv[2], "-silent") == 0)
				return ModeType::load_silent;
		
		else throw invalidProgramArguments();	
	}
}