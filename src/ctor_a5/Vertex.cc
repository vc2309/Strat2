#include "Sideline.h"
#include "Vertex.h"
#include "Edge.h"
#include "Player.h"
#include <memory>
#include <vector>

using namespace std;

Vertex::Vertex(int vertexAddress): Sideline{-1, vertexAddress}, level{-1} {}

int Vertex:: getlevel()
{
	return level;
}

int Vertex::getSideline()
{
	return address;
}

shared_ptr<Edge> Vertex::getEdge(int idx)
{
	return adjacentEdges[idx]; //Zero indexed
}

void Vertex::levelup()
{
	level++;
}

void Vertex::DistributeResource(int resource)
{
	int giveResource = (level+1);
	ownedBy->getResources(resource,giveResource);
}

int Vertex::getsize()
{
	return adjacentEdges.size();
}


shared_ptr<Player> Vertex::getOwner()
{
	return ownedBy;
}

int Vertex::getOwnerID()
{
	return ownerID;
}

void Vertex::setOwner(int player, shared_ptr<Player> owner)
{
	ownedBy = owner;
	ownerID=player;
}

void Vertex::setAddress(int addr)
{
	address=addr;
}

void Vertex::setAdjEdges(vector<shared_ptr<Edge>> e)
{
	adjacentEdges=e;
}
int Vertex::getAddress() {
	return address;
}
