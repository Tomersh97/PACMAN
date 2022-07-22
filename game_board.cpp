#include "game_board.h"


// Initialize board from a locally created board.
void gameBoard::init_board(bool isSilent)
{
	system("cls");
	loadBoardFromFile();
	if (!isSilent)
		printBoard();
}


// Print the object's symbol in the object's location.
void gameBoard::printSymbolAtPos(const Position& objectPos, int color)const
{
	SetConsoleTextAttribute(console, color);
	gotoxy(objectPos.getPosX(), objectPos.getPosY());
	cout << getCellBoardSymbol(objectPos,0,0);
	SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
}


// Randomize a number for the object's moving function.
Position gameBoard::getRandAvailablePos()const
{
	return availablePosVec[rand() % availablePosVec.size()];
}


//checks if the entity depending on typeEntity is at the position on the board
void gameBoard::setCellBoardEntity(Position entityPos, typeEntity typeEntity, bool entityToSet)
{
	switch (typeEntity)
	{
		case typeEntity::ghost_entity:
			setCellBoardGhost(entityPos, entityToSet);
			break;
		case typeEntity::fruit_entity:
			setCellBoardFruit(entityPos, entityToSet);
			break;
	}
}


// Let the user to choose a specific game board.
void gameBoard::getAndLoadScreenName()
{
	string boardNameFromUser;
	displayAllBoardNames();
	if (numAvailableBoards > 0)
	{
		cout << "Please enter the screen name you would like to load" << endl;
		std::cin >> boardNameFromUser;
		validateBoardName(boardNameFromUser);		
	}
	else
	{
		cout << "No available boards to load from files!" 
			 << "Press any key to return"<< endl;
		system("pause");
		return;
	}
}


// Print all the valid game boards.
void gameBoard::displayAllBoardNames()const
{
	for (int i = 0; i < numAvailableBoards; i++)
		cout << boardNames[i] << endl;
}


// Check which game boards are valid and which are not.
void gameBoard::checkAvailableBoards()
{
	bool badFileFound = false;
	string gamePath = current_path().string();
	string extention = ".screen", currFileName, errorReason;
	std::ofstream myfile("log.txt");	// open a file in the same folder - log file

	for (auto& i : filesystem::recursive_directory_iterator(gamePath)) //for ranged loop - goes through files in gamePath directory
	{
		if (i.path().extension() == extention)
		{
			currFileName = i.path().stem().string();
			if (gameBoardLegitCheck(currFileName, errorReason))			//returns true for valid file
			{
				boardNames.push_back(currFileName + ".screen");			
				numAvailableBoards++;
			}
			else
			{
				badFileFound = true;
				myfile << currFileName << ".screen: " << errorReason << endl; //writes to log file invalid board + reason
				errorReason.clear();
			}
		}
	}
	if (!badFileFound)
		myfile << "All files are good." << endl;

	myfile.close();
	if (numAvailableBoards == 0)					//the case in which there are no available screens
	{
		isNoLegitBoard = true;
		throw failedLoadBoard();	
	}
	else
		std::sort(boardNames.begin(),boardNames.end());
}


// Check if the given game board is legit.
bool gameBoard::gameBoardLegitCheck(const string& currFileName, string& errorReason)const	
{
	int numOfPacmans = 0, numOfGhosts = 0, numOfMSGbox = 0, rowCounter = 0, colCounter = 0, maxCol = 0, firstRowLen=0;
	bool unvalidCharExist = false, fileIsEmpty = false, firstRow = true;
	string firstLine;
	ifstream boardToLoad;
	boardToLoad.open(currFileName + ".screen");
	char ch = boardToLoad.get();

	if (boardToLoad.eof())
	{
		errorReason = "This file is empty.";
		fileIsEmpty = true;
	}

	while (!boardToLoad.eof())				//checks every character read from the file 
	{										
		colCounter++;
			if (ch == PACMAN_SYMBOL)
				numOfPacmans++;
			else if (ch == MSG_POS_SYMBOL)
				numOfMSGbox++;
			else if (ch == GHOST_SYMBOL)
				numOfGhosts++;
			else if (ch == ENTER_SYMBOL)
			{
				colCounter--;
				if (colCounter > maxCol)
					maxCol = colCounter;
				
				colCounter = 0;
				rowCounter++;
			}
			else if (ch == BLANK_SPACE_INFILE || ch == WALL || ch == MSG_POS_SYMBOL || ch == FOOD_INFILE);	
			else {
				if (ch != EOF)
					unvalidCharExist = true;
			}
			ch = boardToLoad.get();
	}

	boardToLoad.close();
			//returns false if file is not valid - empty or from problemDetected
	if  (fileIsEmpty || problemDetected(numOfPacmans, numOfGhosts,numOfMSGbox, unvalidCharExist, maxCol, rowCounter, errorReason))
		return false;
	else
		return true;
}


// Check for errors in the given game board.
bool gameBoard::problemDetected(int numOfPacmans, int numOfGhosts,int numOfMSGbox, bool unvalidCharExist, int colCounter, int rowCounter, string& errorReason)const
{
	bool res = false;

	if (numOfPacmans != 1)
	{
		errorReason += "There are more/less then one pacman. ";
		res = true;
	}
	if (numOfGhosts > 4)
	{
		errorReason += "There are too many ghosts. ";
		res = true;
	}
	if (numOfMSGbox != 1)
	{
		errorReason += "There are more/less then one & for the msg box. ";
		res = true;
	}
	if (unvalidCharExist)
	{
		errorReason += "There is at least one unvalid char. ";
		res = true;
	}
	if (colCounter > maxCol || rowCounter > maxRow)
	{
		errorReason += "Exceeding the boundaries of the field. ";
		res = true;
	}
	return res;
}


// Check if the user enter a valid game board name.
void gameBoard:: validateBoardName( string &boardName)
{
	bool validBoardChosen = false;
	while (!validBoardChosen)
	{
		for (int i=0; i < numAvailableBoards; i++)
		{
			if (boardNames[i] == boardName)
			{ 
				validBoardChosen = true;
				chosenBoardNumber = i;
			}
		}
		if (!validBoardChosen)
		{ 
			cout << "Please enter a valid name for an available board!" << endl;
			std::cin >> boardName;
		}
	}
}


// Load game board from a given file.
void gameBoard:: loadBoardFromFile()
{
	bool isEnterRead = false, isFirstRowFound = false, isRowLenFound = false;
	int  rowCounter = 0, colCounter = 0;
	char readChar = ' ';
	ifstream boardToLoad;
	boardToLoad.open(boardNames[chosenBoardNumber]);
	if (boardToLoad.good())
	{
		while (!boardToLoad.eof())
		{
			if (colCounter >= col_len)				// Function that ignore all read characters after chosen column length
			{
				string nextline;
				getline(boardToLoad, nextline);
				colCounter = 0;
				rowCounter++;
				isEnterRead = true;
			}
			readChar = boardToLoad.get();

			if (((isEnterRead && readChar == '\n')) && !isRowLenFound)
			{
				row_len = rowCounter;
				isRowLenFound = true;
			}
			if ((readChar == EOF) && !isRowLenFound)
			{
				row_len = rowCounter;
			}
			if (isEnterRead)
				isEnterRead = false;

			assignCharToBoard(readChar, rowCounter, colCounter, isEnterRead);

			if (!isFirstRowFound)
			{
				if (isEnterRead)
				{
					col_len = colCounter;
					isFirstRowFound = true;
					colCounter = 0;
				}
			}
		}
		setBoardMessages();
		boardToLoad.close();
	}
	else
		throw failedLoadBoard();
}


// Assign each given char with his specific characteristics in the game board.
void gameBoard::assignCharToBoard(char readChar, int& rowCounter, int& colCounter, bool &isEnterRead)
{
	switch (readChar)					
	{
		case WALL:
			cellBoard[rowCounter][colCounter++].setcellSymbol(WALL);
			break;
		case PACMAN_SYMBOL:
			setPosOfEntityFromBoard(colCounter, rowCounter, (int)entityNumOnPosVec::pacmanNum);
			colCounter++;
			break;
		case BLANK_SPACE_INFILE:
			cellBoard[rowCounter][colCounter++].setcellSymbol(BLANK_SPACE_BOARD);
			break;
		case GHOST_SYMBOL:
			setPosOfEntityFromBoard(colCounter, rowCounter, (int)entityNumOnPosVec::ghostNum + numOfGhostsOnBoard);
			++numOfGhostsOnBoard;
			colCounter++;
			break;
		case ENTER_SYMBOL:						
			isEnterRead = true;
			rowCounter++;
			break;
		case MSG_POS_SYMBOL:
			scoreAndLivesPos.setPosX(colCounter);
			scoreAndLivesPos.setPosY(rowCounter);
			colCounter++;
			break;
		case FOOD_INFILE:
		{
			numOfBreadCrumbs++;
			Position currPositon(colCounter, rowCounter);
			availablePosVec.push_back(currPositon);
			cellBoard[rowCounter][colCounter++].setcellSymbol(FOOD_BOARD);
			break;
		}
	}
}


//sets single entities pos from posvector of board
void gameBoard::setPosOfEntityFromBoard(int col, int row, int index)
{
	isEntityPresent[index] = true;
	entityPositionsOnBoard[index].setPosX(col);
	entityPositionsOnBoard[index].setPosY(row);
}


//clears area for score and lives message box 
void gameBoard::setBoardMessages()		
{
	for (int i = 0; i < msg_box_row; i++)
	{
		for (int j = 0; j < msg_box_col; j++)
		{
			if (cellBoard[scoreAndLivesPos.getPosY() + i][scoreAndLivesPos.getPosX() + j].get_cellSymbol() == FOOD_BOARD)
			{
				numOfBreadCrumbs--;
				cellBoard[scoreAndLivesPos.getPosY() + i][scoreAndLivesPos.getPosX() + j].setcellSymbol(BLANK_SPACE_BOARD);
			}
		}
	}
}


//pushes max amount of ghosts to boolean vector, to be set later
void gameBoard::pushBackGhostsPresent()
{
	Position emptyPos;

	for (int i = 1; i <= maxNumOfGhosts; i++)
	{
		entityPositionsOnBoard.push_back(emptyPos);
		isEntityPresent.push_back(false);
	}	
}


// Print game board.
void gameBoard::printBoard()const
{
	for (int i = 0; i < maxRow; i++)
	{
		for (int j = 0; j < maxCol; j++)
		{
			gotoxy(j, i);

			if (cellBoard[i][j].get_cellSymbol() == WALL)
				printSymbolAtPos(i, j, Colors::getWallColor());
			
			else if (cellBoard[i][j].get_cellSymbol() == FOOD_BOARD)
				printSymbolAtPos(i, j, Colors::getFoodColor());
			
			else
				cout << cellBoard[i][j].get_cellSymbol();
		}
	}
}


//prints symbol at given position
void gameBoard:: printSymbolAtPos(int yPos, int xPos, int color)const
{
	gotoxy(xPos, yPos);
	SetConsoleTextAttribute(console, color); 
	cout << cellBoard[yPos][xPos].get_cellSymbol();
	SetConsoleTextAttribute(console, (int)gameColors::whiteColor);
}


// sets next board number
void gameBoard::setNextBoard()
{
	if (chosenBoardNumber + 1 == numAvailableBoards)
		chosenBoardNumber = 0;
	else
		chosenBoardNumber++;
}


// Reset all game board's stats.
void gameBoard:: resetBoardStats()
{
	col_len = maxCol;
	row_len = maxRow;
	availablePosVec.clear();
	numOfGhostsOnBoard = 0;
	numOfBreadCrumbs = 0;
	entityPositionsOnBoard.clear();
	isEntityPresent.clear();
	pushBackGhostsPresent();
	clearAllCellsData();
}


//sets all cells to contain blank space character
void gameBoard::clearAllCellsData()
{
	for (int y = 0; y < maxRow; y++)
		for (int x = 0; x < maxCol; x++)
			cellBoard[y][x].setcellSymbol(' ');
}
