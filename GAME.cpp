#include "GAME.h"


bool Colors::withColors = false;

//Main Game's ctor. Runs the game from the ctor. Also catches various exceptions
Game::Game(int argc, char** argv) :isInProgram(false), isGamePaused(false), isGameFinished(false), currScore(0), dirKey('S'), tempKey('S'), scoreToWin(0), gameTimer(0), numPhEntities(2), numBoardsWon(0)
{
	{
		try		{	check_mode_start( argc, argv);	}
		catch (failedLoad_EOF)
		{	system("cls");
		if (isLoadSilent)
			cout << "Failed test! failed at: " << mode->get_file_using() << endl;
		cout << "End of load file reached! file: " << mode->get_file_using() << ".steps" << endl;	}
		catch (failedLoad_unexpectedLoadedMove)	
		{system("cls"); cout << "Unexepected movement loaded, loading terminatted at: " << mode->get_file_using() << ".steps" << endl;	}
		catch (failedLoadFile) 
		{ system("cls"); cout << "Failed to load file! load file: " <<mode->get_file_using()<< endl; }
		catch (failSaveFile) { system("cls"); cout << "Failed to save file!"<< mode->get_file_using() << endl; }
		catch (failedTest) { system("cls"); cout << "Failed test! failed at: " << mode->get_file_using() << endl; }
		catch (invalidProgramArguments){system("cls"); cout << "Failed reading command line arguments! " << endl;}
	}
}


// Main game initialization 
void Game::init_pacman_game()                                
{
	while (!isInProgram&&!board.getIsNoLegitBoard())
	{	
		SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
		main_menu.init_main_menu(board);	// show menu  
		char choice = _getch();
		main_menu.choice_validation(choice,board); // set the user's choice to the current Game object
		if (choice == (int)Menu::choices::choice_exit)
			isInProgram = true;
		else if (choice == (int)Menu::choices::choice_screen)//choose different screen
		{ 
			board.getAndLoadScreenName();
			if (mode_type != ModeType::simple)	
			{
				mode->set_file_using(board.displayCurrScreen());
				mode->open_next_file();
			}
		}
		else if (choice == (int)Menu::choices::choice_instructions)
		{
			main_menu.view_instructions();
			system("pause");
		}
		else				// if choice == 1
		{
			prepareForGame();
			start_pacman();  // start game when user entered direction key
		}
	}
}


//initializes various game objects for the start of the game
void Game::prepareForGame()
{
	main_menu.decideIfColors(mode_type);
	ghostType difficulty = main_menu.readGhostDifficulty(mode_type);
	isGameFinished = false;
	board.init_board(isLoadSilent);	// initialize board	
	initialize_game_entities(difficulty , isLoadSilent);
	await_start_movement();
}


//Main game's playing loop
void Game::start_pacman()
{	
	while (!isGameFinished) 
	{ 
		displayScore(isLoadSilent);
		handle_timer();
		if (mode_type != ModeType::load && !isLoadSilent)
		{
			check_movement();	
			((PACMAN*)entities[pacman_ind])->setDirectionFromKey(dirKey);	// check user input from keyboard
		}
		gameTimer++;						  
		checkTimelyOccurances();
		if (!isGameFinished)
			checkGameOccurances();
	}
	isInProgram = FALSE;	
}


//performs actions depended on the game loop timer
void Game::checkTimelyOccurances()
{
	for (int i = 0; i < numPhEntities; i++) 
	{
		if (i == pacman_ind)
		{
			entities[i]->move(board, mode, mode_type);
			checkGameOccurances();
			if (isGameFinished)
				return;
		}
		if (gameTimer % 2 == 0)
		{
			 if (i >= ghost_ind)
				entities[i]->move(board,mode, mode_type);
		}
	}
	((Fruit*)entities[fruit_ind])->fruitOccurances(board, gameTimer, (save_mode*)mode, mode_type);	
}


// Method to handle different game occurances
void Game::checkGameOccurances()					
{                                														  
	if (((PACMAN*)entities[pacman_ind])->isPacmanHitsGhost(board))			// check if there is a ghost at pacman's position
	{
		if (mode_type == ModeType::save || isLoadSilent)
			mode->mode_deathtime(gameTimer);
		((PACMAN*)entities[pacman_ind])->setPacmanLives();					// pacman's life -1
		
		if (((PACMAN*)entities[pacman_ind])->getPacmanLives() == PACMAN::NO_LIVES)
		{
			resetEntities();
			numPhEntities = 2;
			board.setCurrChosenBoard(0);
			isGameFinished = true;
			GameOver_screen();
			resetGame();
		}
		else
		{
			resetEntities();
			await_start_movement();
		}
	}
	else
		checkPacmanScore();					// check if breadcrumb at board on pacmans position
}


// Method to handle current score situation
void Game::checkPacmanScore()			 
{
	if (((PACMAN*)entities[pacman_ind])->isPacmanEatFood(board))
	{
		currScore++;		         // score +1
		board.setCellBoardSymbol(entities[pacman_ind]->getEntityPos().getPosY(), entities[pacman_ind]->getEntityPos().getPosX(), ' ');  // update board cell to ' '
	}
	if (((PACMAN*)entities[pacman_ind])->isPacmanEatFruit(board) && ((Fruit*)entities[fruit_ind])->getFruitOnBoard())
	{
		scoreToWin += (int)((Fruit*)entities[fruit_ind])->getFruitNum();
		currScore  += (int)((Fruit*)entities[fruit_ind])->getFruitNum();
		entities[fruit_ind]->reset(board, mode_type);
		entities[pacman_ind]->printEntityAtPos(mode_type);
	}
	if (currScore == scoreToWin)		 // when reaching the highest score - show winning screen and reset game                                        
	{
		if (mode_type == ModeType::save || mode_type == ModeType::load_silent)
			mode->mode_finish(gameTimer);
		numBoardsWon++;
		board.resetBoardStats();
		resetEntities();
		numPhEntities = 2;
		entities[fruit_ind]->reset(board, mode_type);	
		if (numBoardsWon == board.getNumAvailBoards())  //if all screens were cleared
		{
			board.setCurrChosenBoard(0);
			isGameFinished = true;
			GameWinner_screen(isLoadSilent);
			resetGame();
		}
		else											// a single screen was cleared			
		{	
			gameTimer = 0;
			board.setNextBoard();
			board.init_board(isLoadSilent);
			if (mode_type != ModeType::simple)
			{
				mode->set_file_using(board.displayCurrScreen());
				mode->open_next_file();
			}
			initialize_game_entities(((Ghosts*)entities[ghost_ind])->getGhostDifficulty(), isLoadSilent);
			await_start_movement();
		}
	}
}


// Check if ESC was pressed
void Game::check_pausing(char tempKey, char dirKey)		
{
	if (tempKey == ESC)
	{
		setGamePaused(true);									//Sets to 'pause' state
		entities[pacman_ind]->printEntityAtPos();				// printFunction
		for (int i = ghost_ind; i < numPhEntities; i++)
			entities[i]->printEntityAtPos();
		gotoxy(board.getMSGboxPos().getPosX(), board.getMSGboxPos().getPosY());
		cout << "Game paused!       ";							//Message for user
		while (getGamePaused())									//Loop until unpaused	
		{	
			if (_kbhit())
			{
				tempKey = _getch();
				if (tempKey == ESC)
				{
					setGamePaused(false);					    //If next pressed key is ESC -> sets state to unpause
					setTempKey(dirKey);							//tempKey is reassigned to previous direction
				}
			}
		}
		gotoxy(board.getMSGboxPos().getPosX(), board.getMSGboxPos().getPosY());
		cout << "                   ";
	}
}


// Reset the game objects and their stats in preperation for a new game started
void Game::resetGame()	
{	
	numBoardsWon = 0;
	scoreToWin = 0;
	((PACMAN*)entities[pacman_ind])->resetLives();
	currScore = 0;
	isGameFinished = true;
	board.resetBoardStats();
}


// Awaits initial input to start movement
void Game::await_start_movement()
{
	if (mode_type != ModeType::load && mode_type!=ModeType::load_silent)
	{
		bool noKeyHit = false;
		while (!noKeyHit)
		{
			if (_kbhit())
			{
				setTempKey(_getch());
				noKeyHit = isDirKeyHit(tempKey);
			}
		}
	}
}


// Return true if a valid movement key was hit
bool Game::isDirKeyHit(char key) const
{
	return (key == 'w' || key == 'W' ||
	      	key == 'x' || key == 'X' ||
			key == 'a' || key == 'A' ||
			key == 'd' || key == 'D' ||
			key == 's' || key == 'S');
}


// Function to handle movement keys
void Game::check_movement()                  
{
	if (_kbhit())
		setTempKey(_getch());
	if (isDirKeyHit(tempKey))
		setDirKey(tempKey);
	check_pausing(tempKey, dirKey);
}


//If colors option is chosen, cycles through different colors printing the victory mesage in different colors
void Game::GameWinner_screen(bool isSilent)const
{
	if (!isSilent)
	{
		system("cls");
		if (Colors::withColors)
		{
			int i = 0;
			do {
				i = (i + 1) % Colors::MAX_COLORS;
				SetConsoleTextAttribute(console, i);
				gotoxy((int)MSG_POS::msgPOSX, (int)MSG_POS::msgPOSY);
				cout << "Congratulations! YOU ARE THE WINNER!" << endl << endl << endl << endl;
				Sleep(500);
				system("cls");
			} while (!_kbhit());
			SetConsoleTextAttribute(console, (int)gameColors::whiteColor); // set text color back to white	
		}
		else													  //colors not chosen
		{
			gotoxy((int)MSG_POS::msgPOSX, (int)MSG_POS::msgPOSY);
			cout << "Congratulations! YOU ARE THE WINNER!" << endl << endl << endl << endl;
		}
		SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
		system("pause");
	}
	else
		cout << "Test successful!" << endl;
}


//game over message 
void  Game::GameOver_screen()const
{
	if (!isLoadSilent)
	{
		system("cls");
		gotoxy((int)MSG_POS::msgPOSX, (int)MSG_POS::msgPOSY);
		cout << "You lost all your lives! game over :(" << endl << endl << endl << endl;
		system("pause");
	}
	else 
		cout << "Test successful!" << endl;
}


// Display the score and life box.
void Game::displayScore(bool isSilent)
{
	if (!isSilent)
	{
		gotoxy(board.getMSGboxPos().getPosX() + 1, board.getMSGboxPos().getPosY());
		cout << "SCORE:" << currScore << " LIVES: " << ((PACMAN*)entities[pacman_ind])->getPacmanLives();
	}
}


//Resets the entities on the board (pacman, fruit,ghosts)
void Game::resetEntities()
{
	entities[pacman_ind]->reset(board, mode_type);
	entities[fruit_ind]->reset(board, mode_type);
	for (int i = ghost_ind; i < numPhEntities; i++)
	{
		entities[i]->reset(board, mode_type);
		entities[i]->setEntityPos(board.getEntityPosOnBoard(i - 1));
		entities[i]->printEntityAtPos(mode_type);
	}				 
}


//initializes memory for entity pointers
void Game:: init_entity_memory()
{
	PACMAN* _pac = new PACMAN;
	entities[pacman_ind] = _pac;
	_pac = nullptr;

	Fruit* _fruit = new Fruit;
	entities[fruit_ind] = _fruit;
	_fruit = nullptr;

	for (int i = ghost_ind; i < NUM_ENTITIES; i++)
	{
		Ghosts* _ghost = new Ghosts;
		entities[i] = _ghost;
		_ghost = nullptr;
	}
}


//initializes the game entities with stats from the game board
void Game:: initialize_game_entities(ghostType difficulty, bool isSilent)
{
	scoreToWin += board.getNumBreadsCrumbs();
	for (int i = 0; i < NUM_ENTITIES; i++)
	{
		if (i == pacman_ind)					//init pacman
		{
			entities[i]->setEntityPos(board.getEntityPosOnBoard((int)entityNumOnPosVec::pacmanNum));
			entities[i]->setEntitySymbol(gameBoard::PACMAN_SYMBOL);
		}
		else if (i == fruit_ind);
		else
		{
			if (board.getEntitiesPresent(i - 1)) //init the ghosts
			{
				numPhEntities++;
				((Ghosts*)entities[i])->setGhostDifficulty(difficulty);
				entities[i]->setEntityPos(board.getEntityPosOnBoard(i - 1));
				entities[i]->setEntitySymbol(gameBoard::GHOST_SYMBOL);
				entities[i]->printEntityAtPos(mode_type);
			}
		}
	}				
	entities[pacman_ind]->printEntityAtPos(mode_type);
}


//gets program arguments and sets the mode according to arguments
void Game::check_mode_start(int argc, char** argv)
{
	mode_type = main_menu.validateSet_GameMode(argc , argv);
	init_entity_memory();
	switch (mode_type)
	{
		case ModeType::simple:
		case ModeType::save:
		{
			srand((unsigned int)time(NULL));
			if (mode_type == ModeType::save)	//sets game for save mode
			{
				save_mode* _mode_save = new save_mode;
				mode = _mode_save;
				_mode_save = nullptr;
				mode->set_file_using(board.displayCurrScreen());
				mode->_init_mode();
			}
			init_pacman_game();
			break;
		}
		case ModeType::load_silent:		//sets game for loading mode
		case ModeType::load:
		{
			load_mode* _mode_load = new load_mode;
			mode = _mode_load;
			_mode_load = nullptr;
			if (mode_type == ModeType::load_silent)
			{ 
				((load_mode*)mode)->setIsSilent(true);
				isLoadSilent = true;
			}
			mode->set_file_using(board.displayCurrScreen());
			mode->_init_mode();
			prepareForGame();
			start_pacman();
			break;
		}
	}
}


//sets time periods according to mode
void Game::handle_timer()const
{
	if (!isLoadSilent)		//if silent mode no sleep to get the fastest result
	{
		if (mode_type == ModeType::load)
			Sleep(80);
		else Sleep(300);
	}
}



