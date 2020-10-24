#ifndef TOXIC_LEVEL_INCLUDED
#define TOXIC_LEVEL_INCLUDED

#include "SM64DS_2.h"

struct ToxicLevel : public Actor
{
	static ToxicLevel* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ToxicLevel() override;

	unsigned toxicCounter = 0;
	unsigned frameForDamage = 79;
	
	static SpawnInfo<ToxicLevel> spawnData;
	
};

#endif