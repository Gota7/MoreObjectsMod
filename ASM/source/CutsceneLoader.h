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

	enum Condition
	{
		noStarsInCurrentLevel = 0,
		always = 1,
		noMarioKey = 2,
		noLuigiKey = 3,
		noWarioKey = 4,
		noBasementKey = 5,
		noUpperHallKey = 6,
		noStar7InHub = 7,
		eventSet = 8
	};
	
	uint8_t condition;
	uint8_t eventID;
	bool hasLoaded;

	static SpawnInfo<CutsceneLoader> spawnData;
};

#endif