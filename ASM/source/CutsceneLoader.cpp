#include "CutsceneLoader.h"

namespace
{
	
}

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

void CutsceneLoader::RunScript()
{
	LoadAndRunKuppaScript(param1);
}

int CutsceneLoader::InitResources()
{
	trigger = (ang.x & 0xF000) >> 12;
	eventID = ang.x & 0xFF;
	hasLoaded = false;
	flags--;
	
	return 1;
}

int CutsceneLoader::CleanupResources()
{
	return 1;
}

int CutsceneLoader::Behavior()
{
	if (!hasLoaded)
	{
		if (trigger == 0)
		{
			//Only runs if no stars have been collected in the current level
			if (LEVEL_ID < 15)
			{
				if (SAVE_DATA.mainStars[SUBLEVEL_LEVEL_TABLE[LEVEL_ID]] == 0)
				{
					RunScript();
					hasLoaded = true;
				}
			}
			else
			{
				if (SAVE_DATA.secretStars[SUBLEVEL_LEVEL_TABLE[LEVEL_ID] - 15] == 0)
				{
					RunScript();
					hasLoaded = true;
				}
			}
		}
		else if (trigger == 1)
		{
			//Always runs
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 2 && (SAVE_DATA.keysObtained & 1 << 3) == 0)
		{
			//Only runs if Mario's key hasn't been obtained
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 3 && (SAVE_DATA.keysObtained & 1 << 4) == 0)
		{
			//Only runs if Luigi's key hasn't been obtained
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 4 && (SAVE_DATA.keysObtained & 1 << 5) == 0)
		{
			//Only runs if Wario's key hasn't been obtained
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 5 && (SAVE_DATA.keysObtained & 1 << 3) == 0)
		{
			//Only runs if the first Bowser key hasn't been obtained
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 6 && (SAVE_DATA.keysObtained & 1 << 3) == 0)
		{
			//Only runs if the second Bowser key hasn't been obtained
			RunScript();
			hasLoaded = true;
		}
		else if (trigger == 7 && (SAVE_DATA.secretStars[14] & 1 << 7) == 0)
		{
			//Only runs if HUB star 7 hasn't been collected
			RunScript();
			hasLoaded = true;
		}
		else if (eventID < 0x20 && trigger == 8)
		{
			//Runs when an event is set
			if (Event::GetBit(eventID))
			{
				RunScript();
				hasLoaded = true;
			}
		}
	}
	
	return 1;
}

int CutsceneLoader::Render()
{
	return 1;
}

CutsceneLoader::~CutsceneLoader() {}