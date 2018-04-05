#include "Dice.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

void Dice::updateDice(bool seed, int val) {
	if (seed) {
		isSeed = true;
		diceSeed = val;
	}
	else {
		isSeed = false;
		diceSeed = -1;
	}
	srand(diceSeed);
}

bool Dice::checkSeed() {
	return isSeed;
}

int Dice::getSeedVal() {
	return diceSeed;
}

Dice::Dice(bool seedExist, int seedVal):
	isSeed{seedExist}, diceSeed{seedVal} {}

int Dice::rollFairDice() {
	int roll;
	if (isSeed) {// Seed was provided
		int d1 = rand()%6+1;
		int d2 = rand()%6+1;

		roll = d1 + d2;
	}
	else {
		int dd1 = rand()%6+1;
		int dd2 = rand()%6+1;

		roll = dd1 + dd2;
	}
	return roll;
}

int Dice::rollLoadedDice() {
	cin.ignore(); // ignore newline in prev cin
	int roll;
	while (true) {
		bool success = false;
		string inputLine;

		string word;
		cout << "Input a roll between 2 and 12:" << endl;
		cout << "> ";

		// If using getline after cin >>
		//  need to flush newline buffer...
		getline(cin, inputLine);
		istringstream ss{inputLine};

		while (ss >> roll) {
			if (roll >= 2 and roll <= 12) {
				success = true;
				break;	
			}
			else {
				break;
			}
		}
		if (success) {
			break;
		}
		cout << "Invalid roll." << endl;
	}
	return roll;
}

