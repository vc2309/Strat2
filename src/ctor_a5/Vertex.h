#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Sideline.h"

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>

class Edge;
class Player;


class Vertex: public Sideline
{
	int level; 
	//-1 not owned, 0 for basement, 1 for house, 2 for tower
	std::vector<std::shared_ptr<Edge>> adjacentEdges;

	public:
	Vertex(int vertexAddress);

	int getlevel();
	// std::string getAddress();
	int getSideline() ; 
	std::shared_ptr<Player> getOwner() ;
	int getsize();	//Number of adjacent edges
	std::shared_ptr<Edge> getEdge(int idx);
	void DistributeResource(int resource); //Increment player resource
	void levelup();	//Increase level of vertex (basement->house->tower)
	int getOwnerID() ;
	int getAddress();
	//Setters

	void setOwner(int player, std::shared_ptr<Player> owner) ;
	void setAddress(int addr) ;

	void setAdjEdges(std::vector<std::shared_ptr<Edge>> e);
};
#endif
