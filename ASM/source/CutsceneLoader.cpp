#include "CutsceneLoader.h"

SpawnInfo<CutsceneLoader> CutsceneLoader::spawnData =
{
	&CutsceneLoader::Spawn,
	0x0000,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

CutsceneLoader* CutsceneLoader::Spawn()
{
	return new CutsceneLoader;
}

/*

CutsceneLoader parameters:

Parameter 1:
TFFF

Parameter 2:
XXEE

T: Trigger
0: On level start if no stars have been collected in the level
1: On level start
2: When event is triggered

F: Ov0 File ID

E: Event ID

*/

int CutsceneLoader::InitResources()
{
	condition = (ang.x & 0xF000) >> 12;
	eventID = ang.x & 0xFF;
	hasLoaded = false;
	
	return 1;
}

int CutsceneLoader::CleanupResources()
{
	return 1;
}

int CutsceneLoader::Behavior()
{
	if (hasLoaded) return 1;

	if (condition == always || // Run the script always if condition == always

		// If condition == 0, run the script if no stars have been collected in the current level
		condition == noStarsInCurrentLevel && SAVE_DATA.stars[SUBLEVEL_LEVEL_TABLE[LEVEL_ID]] == 0 ||

		// If condition == noMarioKey,     run the script if Mario's key      (0x08) hasn't been collected
		// If condition == noLuigiKey,     run the script if Luigi's key      (0x10) hasn't been collected
		// If condition == noWarioKey,     run the script if Wario's key      (0x20) hasn't been collected
		// If condition == noBasementKey,  run the script if the basement key (0x02) hasn't been collected
		// If condition == noUpperHallKey, run the script if the upper hall   (0x04) hasn't been collected
		condition <= noUpperHallKey && !(SAVE_DATA.keysObtained & 1 << ((condition - 1 & 3) + (-condition >> 2) + 3)) ||

		// If condition == 7, run the script if a specific star in the hub hasn't been collected
		condition == noStar7InHub && (SAVE_DATA.stars[29] & 1 << 7) == 0 ||

		//If condition == 8, run the script if an event is set
		condition == eventSet && eventID < 0x20 && Event::GetBit(eventID)
	)
	{
		LoadAndRunKuppaScript(param1);

		hasLoaded = true;
	}

	return 1;
}

int CutsceneLoader::Render()
{
	return 1;
}

CutsceneLoader::~CutsceneLoader() {}