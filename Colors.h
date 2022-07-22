#ifndef Colors_h
#define Colors_h

enum class gameColors { pacmanColor = 14, ghostColor = 4, foodColor = 2, wallColor = 1,  fruitColor = 3 ,whiteColor = 7 };

//Class to handle colors on board
class Colors
{
public:
	static bool	withColors;
	static const int MAX_COLORS = 15;

	static int getGhostColor() {
		if (withColors) return (int)gameColors::ghostColor;
		else return (int)gameColors::whiteColor;
	}

	static int getPacmanColor() {
		if (withColors) return (int)gameColors::pacmanColor;
		else return (int)gameColors::whiteColor;
	}

	static int getFoodColor() {
		if (withColors) return (int)gameColors::foodColor;
		else return (int)gameColors::whiteColor;
	}

	static int getWallColor() {
		if (withColors) return (int)gameColors::wallColor;
		else return (int)gameColors::whiteColor;
	}

	static int getFruitColor() {
		if (withColors) return (int)gameColors::fruitColor;
		else return (int)gameColors::whiteColor;
	}
};

#endif // Colors_h