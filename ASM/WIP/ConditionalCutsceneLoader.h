#ifndef CONDITIONALCUTSCENELOADER_INCLUDED
#define CONDITIONALCUTSCENELOADER_INCLUDED

#include "SM64DS_2.h"

struct ConditionalCutsceneLoader : public Actor
{	
	void UpdateModelTransform();

	static ConditionalCutsceneLoader* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ConditionalCutsceneLoader();
	void RunScript(bool canRun);

	char* KuppaFile[1] = {nullptr};

	#define mode = ang.x & 0xFF
	#define flags = (ang.x & 0xFF00) >> 8

	static SpawnInfo<ConditionalCutsceneLoader> spawnData;
};

#endif