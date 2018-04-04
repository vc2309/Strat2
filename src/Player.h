#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Board.h"

#include <vector>
#include <memory>
#include <string>


enum class Resources;
class Vertex;

class Player {
  int idNum;
  char pColor;
  int resources[5];
  int BuildPoints;
  int residences; 
  bool loadedDice;
  std::vector<std::shared_ptr<Vertex>> PResidences;

  std::shared_ptr<Board> PBoard; // attach after Board is constructed
 public:
  Player(int idNum, char pColor);
  

  int playerStatuses();
  std::string playerResidence();

  bool buildroad(const int EdgeNum);
  bool buildres(const int VertexNum);
  bool improve(const int VertexNum);
  bool trade(const int Playernum, const int ResourceFrom, const int ResourceTo);
  void setResidence(std::shared_ptr<Vertex> v);
  int getID();
  void getResources(const int Resource, const int number);

  char getColor();
  std::string convertColor();

  void setDice(bool d);
  bool isLoad();

  void attachBoard(std::shared_ptr<Board> b);

  //getter methods for saveGame() in Board
  int getRIndex(int r);
  std::shared_ptr<Vertex> getVIndex(int v);
  int ownRes();
};

#endif
