#ifndef _BOARD_H
#define _BOARD_H

#include <memory>
#include <vector>
#include <string>
#include "Board.h"
#include "SourceData.h"
#include "Tile.h"
#include "Sideline.h"
#include "Vertex.h"
#include "Resources.h"
#include "Edge.h"
#include "Dice.h"
#include "Player.h"

class SourceData;
class Tile;
class Sideline;
class Vertex;
class Edge;
class Dice;
class Player;
class UserDisplay;

class Board {
  std::vector<std::shared_ptr<Tile>> tiles;// double check syntax
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Edge>> edges;
  int playerTurn;

  std::shared_ptr<Dice> diceComponent; // 1 dice suffices to simulate set
  std::shared_ptr<UserDisplay> textBoard;
  std::vector<std::shared_ptr<Player>> pPlayers;
  std::shared_ptr<SourceData> data;

public:
  // Idea in main.cc create a unique_ptr to a Board Object, 3 Ctor
  // New game with random board
  Board(bool seed , int seedVal, std::string boardLayout);   
  // Load a SAved game or else load layout.txt default
  Board(bool save, bool seed, int seedVal, std::string saved , std::string boardLayout);

	void printBoard();

  // if isFair = true, rollFairDice, else rollLoadedDice
	int diceRoll(bool isFair);
	void next();
  int getPlayerTurn();

  void updateResources(int diceRoll);
  void updateBoard();

  // Setters
  void updateCurrTurn(int currTurn);
  // int for resrouce index, int for number of that resource
  void updatePlayerResource(int resource, int amount, int players);
  void updatePlayerVertex(int address, int level, int players);
  void updatePlayerRoad(int path, int players);

  void updateTile(int t);
  // Setters

  // convert String level to int level
  //. used in ctor
  int convertLevel(std::string level);

  // Soley used for printing the board
  std::shared_ptr<Tile> getTile(int t);
  std::shared_ptr<Vertex> getVertex(int v);
  std::shared_ptr<Edge> getEdge(int e);
  std::shared_ptr<Player> getPlayer(int p);

  // Methods to set tiles vertices, adj edges, adj vertices
  void setTileVertices();
  void setAdjEdges();
  void setAdjVertex();

  // Method to attach Board share_ptr in SourceData and
  //  Player objects
  void attachBoard(std::shared_ptr<Board> b);

  //main.cc methods
  std::string builderData(int player);
  std::string convertResource(Resources re);
  void saveGame(std::string file);
  std::string convertResToString(int level);
  // end of helper func for saveGame

  void chosestarting();
  void playturn(int Players);
	
  void moveGeese();
};

#endif
