#include "Sideline.h"
#include <memory>
#include <vector>

using namespace std;

//ownedBy will be nullptr by default
Sideline::Sideline(int ownerID, int address):
	ownerID{ownerID}, address{address} {}
