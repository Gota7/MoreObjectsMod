#ifndef BLANKOBJECT_INCLUDED
#define BLANKOBJECT_INCLUDED

#include "include/SM64DS_2.h"

struct BlankObject : public Actor
{	
	void UpdateModelTransform();

	static BlankObject* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~BlankObject();

	static SpawnInfo<BlankObject> spawnData;
};

#endif