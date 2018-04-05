#include "Player.h"
#include "Resources.h"
#include <string>
#include "Edge.h"
#include "Vertex.h"
#include "Board.h"

using namespace std;

Player::Player(int idNum, char pColor):
  idNum{idNum}, pColor{pColor}, BuildPoints{0}, residences{0}, loadedDice{true} {}

   
int Player::playerStatuses(){ 

  return BuildPoints;
}

string Player::playerResidence(){
  string residences = "Player Has Residences At Vertices: ";
  int presSize = PResidences.size();
  for (int i = 0; i < presSize; i++){
    if (!this->PResidences[i]){ // nullptr
      break;
    }
    else {
      stringstream ss;
      ss << PResidences[i]->getSideline();
      residences.append(ss.str());
      residences.append("(Level ");
      ss << PResidences[i]->getlevel();
      residences.append(ss.str());
      residences.append("), ");
    }
  }
  return residences; 
}

bool Player::buildroad(const int EdgeNum){
  bool canBuild = false;
  shared_ptr<Edge> edges = this->PBoard->getEdge(EdgeNum);
  for (int i = 0; i < 2; i++){
    shared_ptr<Vertex> vertices = edges->getVertex(i);
    if (vertices->getOwnerID() == this->idNum){
      canBuild = true;
    }
    for (int j = 0; j < 3; j++){
      shared_ptr<Edge> edges1 = vertices->getEdge(j);
      if (edges1->getOwnerID() == this->idNum){
        canBuild = true;
      }
    }
  }
  bool enoughResources = true;
   if (this->resources[3] < 1 or this->resources[4] < 1){
    enoughResources = false;
  }
  else{
    this->resources[3] -= 1;
    this->resources[4] -= 1;
  }
  if (canBuild && enoughResources){
    edges->setOwner(this->idNum, PBoard->getPlayer(idNum));
    return true;
  }
  return false;
}

bool Player::buildres(const int VertexNum){
  bool canBuild = false;
  shared_ptr<Vertex> vertices = PBoard->getVertex(VertexNum);
  for (int j = 0; j < 3; j++){
    shared_ptr<Edge> edges1 = vertices->getEdge(j);
    if (edges1->getOwnerID() == this->idNum){
      canBuild = true;
    }
  }
  bool enoughResources = true;
  if (this->resources[0] < 1 || this->resources[1] < 1 || this->resources[2] < 1 || this->resources[4] < 1){
    enoughResources = false;
  }
  else{
    this->resources[0] -= 1;
    this->resources[1] -= 1;
    this->resources[2] -= 1;
    this->resources[4] -= 1;
  }
  if (canBuild && enoughResources){ // PBoard CANNOT be nullptr
    vertices->setOwner(this->idNum, PBoard->getPlayer(idNum));
    BuildPoints++;
    return true;
  }
  return false;
}

bool Player::improve(const int VertexNum){
  shared_ptr<Vertex> vertices = this->PBoard->getVertex(VertexNum);
  bool enoughResources = true;
  if ((vertices->getOwnerID() == this->idNum) && enoughResources){
    int levels = vertices->getlevel();
    if (levels == 0){
      buildres(vertices->getSideline());
    }
    else if (levels == 1){
      if (this->resources[2] < 2 || this->resources[3] < 3){
        return false;
      }
      else{
        vertices->levelup();
        this->resources[2] -= 2;
        this->resources[3] -= 3;
        return true;
      }
    }
    else if (levels == 2){
      if (this->resources[0] < 3 || this->resources[1] < 2 || this->resources[2] < 2 || this->resources[3] < 1 || this->resources[4] < 2){
        return false;
      }
      else{
        vertices->levelup();
        this->resources[0] -= 3;     
        this->resources[1] -= 2;
        this->resources[2] -= 2;
        this->resources[3] -= 1;
        this->resources[4] -= 2;
        return true;
      }
    }
  }
  return false;
}

bool Player::trade(const int Playernum, const int ResourceFrom, const int ResourceTo){
  shared_ptr<Player> players = PBoard->getPlayer(Playernum);
  if (this->resources[ResourceTo] > 0 && players->resources[ResourceFrom] > 0){
    this->getResources(ResourceFrom, 1);
    this->getResources(ResourceTo, -1);
    players->getResources(ResourceTo, 1);
    players->getResources(ResourceFrom, -1);
    return true;
  }
  return false;
}

void Player::setResidence(shared_ptr<Vertex> v){
  PResidences.push_back(v);
  residences++;
}
  

// Update resources for given player
void Player::getResources(const int Resource, const int number){
  this->resources[number] += number;
}

int Player::getID()
{
  return idNum;
}

char Player::getColor()
{
  return pColor;
}
string Player::convertColor() {
  string col;
  if (pColor == 'B') {
    col = "Blue";
  }
  else if (pColor == 'R') {
    col = "Red";
  }
  else if (pColor == 'O') {
    col = "Orange";
  }
  else { // Yellow
    col = "Yellow";
  }
  return col;
}

void Player::setDice(bool d) {
  loadedDice = d;
}
bool Player::isLoad() {
  return loadedDice;
}

// attach Board to sourcedata obj AFTER it has been constructed
void Player::attachBoard(shared_ptr<Board> b) {
  PBoard = b;
}

// getter method for save game
int Player::getRIndex(int r) {
  return resources[r];
}
std::shared_ptr<Vertex> Player::getVIndex(int v) {
  return PResidences[v];
}
int Player::ownRes() {
  return PResidences.size();
}


