#ifndef OBJECTLIGHTINGMODIFIER_INCLUDED
#define OBJECTLIGHTINGMODIFIER_INCLUDED

#include "SM64DS_2.h"

struct ObjectLightingModifier : public Actor
{	
	void UpdateModelTransform();

	static ObjectLightingModifier* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ObjectLightingModifier();

	static SpawnInfo<ObjectLightingModifier> spawnData;
};

#endif