#include "ConditionalCutsceneLoader.h"

namespace
{
	
}

SpawnInfo<ConditionalCutsceneLoader> ConditionalCutsceneLoader::spawnData =
{
	&ConditionalCutsceneLoader::Spawn,
	0x0000,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

ConditionalCutsceneLoader* ConditionalCutsceneLoader::Spawn()
{
	return new ConditionalCutsceneLoader;
}

/*

Just a friendly reminder of the parameters:

Parameter 1: Cutscene OV0 ID.

Parameter 2:
FFMM

FF - Flags (Bitflags)
0: Save Dependent (Requires both bytes in parameter 3 to be used (1st byte is bit number, 2nd is save byte number), so nothing else with data can be used)
1: Star Not Retrieved (Current mission for star)
2: Star Retrieved (Current mission for star)
3: Can be ran multiple times.
4: Skippable

MM - Activation Mode
00: On Enter
01: In Range (Data)
02: For CLPS (Data)
03: Player walks past plane with normal being way the object faces.
04: Player is higher than object.
05: Player is lower than object.
06: Enemy Dead (Data where 1st 6 bits is the count, and the other 10 are for the object ID).

Parameter 3: Extra data, depends on mode and flags.
XXXX

*/

void ConditionalCutsceneLoader::RunScript(bool canRun)
{
	if (canRun) {
		KuppaFile[0] = LoadAndRunKuppaScript(param1);
	}
}

int ConditionalCutsceneLoader::InitResources()
{
	if (mode == 0) {
		bool canRun = true;
		RunScript(canRun);
	}
	return 1;
}

int ConditionalCutsceneLoader::CleanupResources()
{
	return 1;
}

int ConditionalCutsceneLoader::Behavior()
{
	UpdateScriptFiles(KuppaFile, sizeof(KuppaFile));
	return 1;
}

int ConditionalCutsceneLoader::Render()
{
	return 1;
}

ConditionalCutsceneLoader::~ConditionalCutsceneLoader() {}