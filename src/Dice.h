#ifndef _DICE_H
#define _DICE_H

class Dice {
protected:
	// Now subclasses can access fields with ease
	bool isSeed;
	int diceSeed;
public:
	Dice(bool seedExist = false, int seedVal = -1);

	void updateDice(bool seed = false, int val = -1);
	bool checkSeed();
	int getSeedVal();

	int rollFairDice();
	int rollLoadedDice();
};

#endif
