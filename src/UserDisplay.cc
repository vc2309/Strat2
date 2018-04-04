#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "UserDisplay.h"
#include "Resources.h"
#include "Sideline.h"
#include "Vertex.h"
#include "Edge.h"
#include "Tile.h"


using namespace std;

UserDisplay::UserDisplay(string layout):
	BoardDisplay{layout} {}

int UserDisplay::fetchCurrT(string line,int i, int increment) {
	int currTile;
	int lengthofPlus = increment;
	string tileNum = " ";
	tileNum[0] = line[i+lengthofPlus];
	istringstream ss{tileNum};
	if (ss >> currTile) { // Tile is in the 10s
		int tens = currTile * 10;
		tileNum[0] = line[i+lengthofPlus+1]; // resuse
		istringstream sss{tileNum};
		sss >> currTile;
		currTile += tens;
	}
	else { // in the digits
		tileNum[0] = line[i+lengthofPlus+1]; // resuse
		istringstream sss{tileNum};
		sss >> currTile;
	}
	return currTile;
}

// Algorithm to print the Board throughout the game
void UserDisplay::printBoardDisplay(Board &board) {
	// Need to read layout.txt file one line at a time
	//  each line is read into a string
	//  need to iterate through the string and perform
	//  required checks on VERTEX,EDGE, and CONTENT within the middle
	//  of the tiles
	// 
	ifstream fileInput(BoardLayout);
	string line;

	int horzLine = 0; // Keep track of kind of line from layout 
	// we expect to be be iterating on when the EDGE check occurs
	// follow pattern for line 0,1,2,3 0,1,2,3 0,1,2,3 etc
	// when horzLine == 0 means horz, 2 means vert

	int currVertex = 0; // Keep track of the number of vertexes we've traversed

	int currPath = 0;// Keep track of the number of roads we've traversed

	int currTile = 0;// Represent currTile function has iterated over

	while(getline(fileInput, line)) {
		// Iterate through through the string, print to stdout
		int lSize = line.size();
		for (int i = 0; i < lSize; ++i) {
			// 5 major checks to look for
			///////////////
			// VERTEX CHECK
			if (line[i] == '|') {
				int skip = 3;

				// Check what vertex you've currently landed on
				shared_ptr<Vertex> vertex = board.getVertex(currVertex);


				int level = vertex->getlevel();
				if (level >= 0) { // Someone has built a residence there
					shared_ptr<Player> player = vertex->getOwner();
					char color = printPlayer(player->getColor());

					// set the actual vertex address on the baord 
					//  to represent player
					line[i + 1] = color; // who OWNS address;
					line[i + 2] = printRes(level);
				} // Else don't do anything to that address
				// Need to ++ curr Vertex either way
				++currVertex;
				// Skip next 3 elements since related to currVertex
				i += skip; // Skip irrelevant slash pair
			}
			// VERTEX CHECK
			///////////////

			// EDGE CHECK
			///////////////
			else if (line[i] == '-') {
				shared_ptr<Edge> edge = board.getEdge(currPath);

				shared_ptr<Player> player = edge->getOwner();
				if (horzLine == 0) { // 2 "-" before reach actual path value
					int skipHorz = 5;; // vary on different boards
					if (player) { // A player owns
						char color = printPlayer(player->getColor());
						line[i + 2] = color;
						line[i + 3] = printRoad();
					}
					// Else don't do anything
					i += skipHorz; // Skip next 5
				}
				else { // 1 "-" before reach path value
					line[i] = ' '; // remove "-"
					int skipVert = 2;
					if (player) { // A player has PATH on that road
						char color = printPlayer(player->getColor());
						line[i + 1] = color;
						line[i + 2] = printRoad();
					}
					// else don't do anything
					i += skipVert; // skip next 2	
				}
				++currPath;
			}
			// EDGE CHECK
			///////////////

			// RESOURCE CHECK
			///////////////
			else if (line[i] == '+') {
				int cushion = 8; // 8 +'s in layout.txt as placeholder
				int lengthofPlus = 7; // Can vary on different boards

				// Important!
				// Need to get char & istringstream into currTile
				currTile = fetchCurrT(line,i, lengthofPlus);


				line[i + 8] = ' '; // remove int for prettyprint 
				shared_ptr<Tile> tile = board.getTile(currTile);

				string resource = printResource(tile->getResource(), cushion);

				for (int k = 0; k < cushion; ++k) {
					line[i + k] = resource[k];
				} 
				i += cushion;
			}
			// RESOURCE CHECK
			///////////////

			// TILE VALUE CHECK
			///////////////
			else if (line[i] == '!') {

				int skip = 2; // skip iteration over int placeholder(s)
				int lengthofPlus = 1;

				currTile = fetchCurrT(line,i, lengthofPlus);

				shared_ptr<Tile> tile = board.getTile(currTile);
				int tValue = tile->getTileVal();
				string tValueString;

				ostringstream toStr;
				toStr << tValue;
				tValueString = toStr.str();

				// tValue << tValueString;
				line[i] = ' ';

				if ((tValueString.size()) < 2) {
					line[i + 1] = ' ';
					line [i + 2] = tValueString[0];
				}
				else {
					line[i + 1] = tValueString[0];
					line[i + 2] = tValueString[1];
				}
				i += skip;
				// skip iteration over the int placeholder for tile value
			}
			// TILE VALUE CHECK
			///////////////

			// GEESE CHECK
			///////////////
			else if (line[i] == '#') {
				int skip = 4; // skip iteration over int placeholder
				int geeseLen = 5;
				int lengthofPlus = 3;

				currTile = fetchCurrT(line, i, lengthofPlus);

				shared_ptr<Tile> tile = board.getTile(currTile);
				bool geese = tile->existGeese();
				if (geese) {
					string g = printGeese();
					for (int j = 0; j < geeseLen; ++j) {
						line[i + j] = g[j];
					}
				}
				else {
					for (int j = 0; j < geeseLen; ++j) {
						line[i + j] = ' ';
					}
				}
				i += skip;
			}
			// GEESE CHECK
			///////////////
			else {}// Do nothing 
		}
		// END OF ITERATION THROUGH THE LINE
		cout << line << endl;
		horzLine = horzLine == 3 ? 0 : horzLine + 1;

	}
}

string UserDisplay::printResource(Resources resource, int cushion) {
	// Cushion should always be greater than length of resource name
	string r;
	string rCushion;
	Resources park = Resources::PARK;
	Resources brick = Resources::BRICK;
	Resources energy = Resources::ENERGY;
	Resources glass = Resources::GLASS;
	Resources heat = Resources::HEAT;

	if (resource == park) {
		r = "PARK";
	}
	else if (resource == brick) {
		r = "BRICK";
	}
	else if (resource == energy) {
		r = "ENERGY";
	}
	else if (resource == glass) {
		r = "GLASS";
	}
	else if (resource == heat) {
		r = "HEAT";
	}
	else { // WIFI
		r = "WIFI";
	}

	int space = cushion - r.size();
	int spaceRight = space / 2;
	int spaceLeft = space - spaceRight;

	for (int i = 0; i < spaceLeft; ++i) {
		rCushion = rCushion + " ";
	}
	rCushion += r;
	for (int j = 0; j < spaceRight; ++j) {
		rCushion += " ";
	}
	return rCushion;	
}

char UserDisplay::printRes(int res) { // Only call if res exists at a vertex
	// 0, 1, 2 for B,H,T
	// if res val at vertex is -1, there should be no need to call
	// this function
	char v;
	if (res == 0) { // 0 for Basement
		v = 'B';
	}
	else if (res == 1) { // 1 for House
		v = 'H';
	}
	else { // Tower residence
		v = 'T';
	}
	return v;
}

char UserDisplay::printPlayer(char colour) { // Only call once determined
	// that a player has built on a vertex or path
	
	return colour;
}

char UserDisplay::printRoad()  {
	return 'R';
}

string UserDisplay::printGeese() {
	return "GEESE";
}
