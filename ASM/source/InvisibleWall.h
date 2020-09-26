#ifndef INVISIBLEWALL_INCLUDED
#define INVISIBLEWALL_INCLUDED

#include "SM64DS_2.h"

struct InvisibleWall : public Platform
{	
	void UpdateModelTransform();

	static InvisibleWall* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~InvisibleWall();
	
	static SharedFilePtr clsnFile;

	static SpawnInfo<InvisibleWall> spawnData;
};

#endif
