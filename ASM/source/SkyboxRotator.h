#ifndef SKYBOXROTATOR_INCLUDED
#define SKYBOXROTATOR_INCLUDED

#include "SM64DS_2.h"

struct SkyboxRotator : public Actor
{	
	void UpdateModelTransform();

	static SkyboxRotator* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~SkyboxRotator();

	static SpawnInfo<SkyboxRotator> spawnData;
};

#endif