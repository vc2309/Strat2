#include "Tile.h"
#include "Sideline.h"
#include "Resources.h"
#include <memory>
#include <vector>
#include "Vertex.h"

using namespace std;

// Every tile initialliy initialized with NETFLIX placeholder resource
Tile::Tile(int tileNum): tileNum{tileNum}, geese{false}, Resource{Resources::PARK}, tileVal{0} {}

shared_ptr<Vertex> Tile::getVertices(int idx)
{
	return vertices[idx];
}

Resources Tile::getResource()
{
	return Resource;
}

int Tile::getTileVal()
{
	return tileVal;
}

bool Tile::existGeese()
{
	return geese;
}

void Tile::setTileNum(int num)
{
	tileNum=num;
}

void Tile::setTileVal(int num)
{
	tileVal=num;
}

void Tile::setResource(Resources res)
{
	Resource=res;
}
void Tile::setGeese(bool g)
{
	geese=g;
}
void Tile::setVertices(vector<std::shared_ptr<Vertex>> v)
{
	vertices=v;
}
void Tile::DistributeResource() {
	// Can only run function on vertices with an owner

	// cout << "vertices size: " << vertices.size() << endl;

	int vSize = vertices.size();
	for (int i = 0; i < vSize; ++i) {
		if (vertices[i]->getOwnerID() != -1) {
			vertices[i]->DistributeResource(convertResourceToInt(getResource()));
		}
	}
}

int Tile::convertResourceToInt(Resources re) {
	// int converted;
	int r;
	Resources brick = Resources::BRICK;
	Resources energy = Resources::ENERGY;
	Resources glass = Resources::GLASS;
	Resources heat = Resources::HEAT;
	// Netflix resource doesn't distribute any resources
	if (re == brick) {
		r = 0;
	}
	else if (re == energy) {
		r = 1;
	}
	else if (re == glass) {
		r = 2;
	}
	else if (re == heat) {
		r = 3;
	}
	else { // WIFI
		r = 4;
	}
	return r;
}
