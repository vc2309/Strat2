#ifndef _USERDISPLAY_H
#define _USERDISPLAY_H

#include "BoardDisplay.h"
#include "Resources.h"
#include <string>

class Board;

class UserDisplay: public BoardDisplay {
public:
	UserDisplay(std::string layout);

	void printBoardDisplay(Board &board) override;
	std::string printResource(Resources resource, int cushion) override;
	
	char printRes(int res) override;
	char printPlayer(char colour) override;

	int fetchCurrT(std::string line,int i, int increment);

	char printRoad() override;

	std::string printGeese();
};

#endif
