#ifndef MOMHOUSEKEEPER_INCLUDED
#define MOMHOUSEKEEPER_INCLUDED

#include "SM64DS_2.h"

struct HouseKeeperObject
{
	short int actorID;
	bool foundInLevel;
	void (*setupFunc)(Actor* actor);
	void (*cleanupFunc)();
};

#endif