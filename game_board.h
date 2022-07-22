#ifndef game_board_h
#define game_board_h

/*==================
Class to handle the game board 
Holds a Cell type in each tile of the board
Is used to read a board from a file and initialize it, receive current data and update occurances during the game
====================*/
#include <fstream>
#include <string>
#include <filesystem>
#include "Cell.h" 
#include "Colors.h"
#include <algorithm>
namespace filesystem = std::filesystem;
using std::filesystem::current_path;
using std::string;
using std::ifstream;
using std::getline;
using std::ios;
using std::ofstream;
namespace fs = std::filesystem; // for readability		

enum class MSG_POS {msgPOSX = 8, msgPOSY = 5};
enum class typeEntity {ghost_entity, pacman_entity, fruit_entity};
enum class entityNumOnPosVec {pacmanNum, ghostNum};

class failedLoadBoard {};

class gameBoard  
{	
	static int constexpr  maxRow = 25;
	static int constexpr  maxCol = 80;

public:
	gameBoard() :row_len(maxRow), col_len(maxCol), numAvailableBoards(0), chosenBoardNumber(0), numOfGhostsOnBoard(0), numOfBreadCrumbs(0),isNoLegitBoard(false)
	{
		try {
			checkAvailableBoards();
			pushBackGhostsPresent();
		}
		catch (failedLoadBoard){ system("cls"); cout << "No loadble boards exist!" << endl; }
	};

private:
	int		  row_len;
	int		  col_len;
	int		  numOfGhostsOnBoard;
	int		  numOfBreadCrumbs;
	int		  chosenBoardNumber;
	int		  numAvailableBoards;
	const int msg_box_col = 20;
	const int msg_box_row = 3;
	bool	  isNoLegitBoard;
	vector<string> boardNames;
	vector<Position> availablePosVec;
	vector<Position> entityPositionsOnBoard;
	vector<bool>     isEntityPresent;
	Position scoreAndLivesPos;
	Position pacmanPos;

	Cell cellBoard[maxRow][maxCol];
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()	

	void setPosOfEntityFromBoard(int col, int row, int index);
	void displayAllBoardNames()const;			
	void checkAvailableBoards();
	bool gameBoardLegitCheck(const string& currFileName, string& errorReason)const;		
	bool problemDetected(int numOfPacmans, int numOfGhosts,int numOfMSGbox ,bool unvalidCharExist, int colCounter, int rowCounter, string& errorReason)const;
	void pushBackGhostsPresent();
	void validateBoardName( string& boardName);
	void loadBoardFromFile();
	void assignCharToBoard(char readChar, int& rowCounter, int &colCounter ,bool& isEnterRead);
	void printBoard()const;
	
public: 
	static const char WALL = '#';
	static const char FOOD_BOARD = '*';
	static const char FOOD_INFILE = ' ';
	static const char PACMAN_SYMBOL = '@';
	static const char GHOST_SYMBOL = '$';
	static const char BLANK_SPACE_INFILE = '%';
	static const char BLANK_SPACE_BOARD = ' ';
	static const char MSG_POS_SYMBOL = '&';
	static const char ENTER_SYMBOL = '\n';

	const int maxNumOfGhosts = 5;
	
	void init_board(bool isSilent);
	
	//Methods to receive and update data at cell 
	char getCellBoardSymbol(Position objectPos, int offsetY, int offsetX) const { return cellBoard[objectPos.getPosY()+ offsetY][objectPos.getPosX()+offsetX].get_cellSymbol(); };
	char getCellBoardSymbol(const Position &objectPos) { return cellBoard[objectPos.getPosY()][objectPos.getPosX()].get_cellSymbol(); };
	char getCellBoardSymbol(int posY, int posX, int offsetY, int offsetX) { return cellBoard[posY + offsetY][posX + offsetX].get_cellSymbol(); }
	void setCellBoardSymbol(int posY, int posX, char symbol) { cellBoard[posY][posX].setcellSymbol(symbol); };
	void setCellBoardEntity(Position entityPos, typeEntity typeEntity, bool entityToSet);
	void setCellBoardGhost( Position ghostPos, bool ghost) { cellBoard[ghostPos.getPosY()][ghostPos.getPosX()].setCellGhost(ghost); };
	void setCellBoardFruit(Position fruitPos, bool fruit) { cellBoard[fruitPos.getPosY()][fruitPos.getPosX()].setCellFruit(fruit); };
	int  getBoardRow()const { return row_len; }
	int  getBoardCol()const { return col_len; }
	Position getRandAvailablePos()const;
	int getNumBreadsCrumbs()const { return numOfBreadCrumbs; };
	void setNextBoard();
	bool checkIfGhostAtCell(Position &entityPos) { return cellBoard[entityPos.getPosY()][entityPos.getPosX()].getCellGhost(); };
	bool checkIfFruitAtCell(Position entityPos) { return cellBoard[entityPos.getPosY()][entityPos.getPosX()].getCellFruit(); }
	void getAndLoadScreenName();
	Position& getMSGboxPos() { return scoreAndLivesPos; };
	void printSymbolAtPos(const Position& objectPos, int color) const;
	void printSymbolAtPos(int yPos, int xPos, int color)const;
	int  getNumGhosts()const { return numOfGhostsOnBoard; };
	bool getIsNoLegitBoard()const { return isNoLegitBoard; };
	Position& getEntityPosOnBoard(int index) { return entityPositionsOnBoard[index]; };
	bool getEntitiesPresent(int index)const { return isEntityPresent[index]; };
	int getCurrChosenBoard()const { return chosenBoardNumber; };
	void setCurrChosenBoard(int chosenBoard) { chosenBoardNumber = chosenBoard; };
	int getNumAvailBoards()const { return numAvailableBoards; };
	void setToNextBoard() { chosenBoardNumber++; };
	void setBoardMessages();
	void setPACMANpos(Position pos) { pacmanPos = pos; }
	Position getPACMANpos()const { return pacmanPos; };

	void resetBoardStats();
	void clearAllCellsData();
	string displayCurrScreen()const { return boardNames[chosenBoardNumber]; };	
};


#endif	// game_board_h