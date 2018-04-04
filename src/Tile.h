#ifndef _TILE_H_
#define _TILE_H_

#include <memory>
#include <vector>
#include "Sideline.h"
#include "Resources.h"


class Vertex;

class Tile
{
	std::vector<std::shared_ptr<Vertex>> vertices;
	int tileNum;
	bool geese;
	Resources Resource;
	int tileVal;

public:
	Tile(int tileNum);

	std::shared_ptr<Vertex> getVertices(int idx);
	Resources getResource();
	int getTileVal();
	bool existGeese();

	//Setters
	void setTileNum(int num);
	void setTileVal(int num);
	void setResource(Resources res);
	void setGeese(bool g);
	void setVertices(std::vector<std::shared_ptr<Vertex>> v);

	void DistributeResource();
	int convertResourceToInt(Resources re);

};

#endif
