#ifndef _EDGE_H_
#define _EDGE_H_

#include <memory>
#include <vector>
#include "Sideline.h"

class Vertex;

class Player; 

class Edge: public Sideline{

	std::vector<std::shared_ptr<Vertex>> adjacentVertex;
public:
	Edge(int edgeAddress);
	// Sideline *getSideline(); //CHANGE UML
	// int getOwner();		//CHANGE UML

	std::shared_ptr<Vertex> getVertex(int idx);
	int getSideline(); 
	std::shared_ptr<Player> getOwner() ;
	int getOwnerID() ;

	//Setters
	void setOwner(int player, std::shared_ptr<Player> owner) ;
	void setAddress(int addr) ;
	void setAdjVertex(std::vector<std::shared_ptr<Vertex>> av);
};

#endif
