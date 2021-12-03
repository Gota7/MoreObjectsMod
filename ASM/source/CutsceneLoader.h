#ifndef CUTSCENELOADER_INCLUDED
#define CUTSCENELOADER_INCLUDED

#include "include/SM64DS_2.h"
#include "KuppaFuncs.h"

struct CutsceneLoader : public Actor
{	
	void UpdateModelTransform();

	static CutsceneLoader* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~CutsceneLoader();
	void RunScript();
	
	uint8_t trigger;
	uint8_t eventID;
	bool hasLoaded;

	static SpawnInfo<CutsceneLoader> spawnData;
};

#endif