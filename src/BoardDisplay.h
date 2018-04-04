#ifndef _BOARDDISPLAY_H
#define _BOARDDISPLAY_H
#include "Resources.h"
#include <string>

class Board;

class BoardDisplay {
protected:
	// Have a field that represents Layout.txt program reads
	// to construct the board
	std::string BoardLayout;
public:
	BoardDisplay(std::string layout);
	~BoardDisplay();

	void virtual printBoardDisplay(Board &board) = 0;

	std::string virtual printResource(Resources resource, int cushion) = 0;
	
	char virtual printRes(int res) = 0;
	char virtual printPlayer(char colour) = 0;

	
	char virtual printRoad() = 0; // Prints 'R' for Road
	// used in combination with printPlayer

};

#endif
