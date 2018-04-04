#ifndef _SIDELINE_H
#define _SIDELINE_H

#include <memory>

class Player;

class Sideline
{
protected:
	std::shared_ptr<Player> ownedBy;
	int ownerID; 
	int address;
public:
	// -1 for placeholders
	Sideline(int ownerID = -1, int address = -1);
/*
	virtual int getSideline() = 0;
	virtual std::shared_ptr<Player> getOwner() = 0;
	virtual int getOwnerID() = 0;

	//Write code to go through procedure of setting up these fields of the classes
	virtual void setOwner(int player, std::shared_ptr<Player> owner) = 0;
	virtual void setAddress(int addr) = 0; */

	
}; 
#endif
