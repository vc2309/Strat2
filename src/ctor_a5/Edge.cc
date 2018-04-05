#include "Sideline.h"
#include "Vertex.h"
#include "Edge.h"
#include "Player.h"
#include <memory>
#include <vector>

using namespace std;

Edge::Edge(int edgeAddress): Sideline{-1, edgeAddress} {}

int Edge::getSideline()
{
	return address;
}

int Edge::getOwnerID()
{
	return ownerID;
}

shared_ptr<Player> Edge::getOwner()
{
	return ownedBy;
}

shared_ptr<Vertex> Edge::getVertex(int idx)
{
	return adjacentVertex[idx];
}

void Edge::setOwner(int player, shared_ptr<Player> owner)
{
	ownedBy=owner;
	ownerID=player;
}

void Edge::setAddress(int addr)
{
	address=addr;
}

void Edge::setAdjVertex(vector<shared_ptr<Vertex>> av)
{
	adjacentVertex=av;
}
