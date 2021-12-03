#include "KuppaFuncs.h"

void RunKuppaScriptIfNotRanYet(char* file)
{
	if (RUNNING_KUPPA_SCRIPT) return;

	char* saveInfo;
    saveInfo = file;

    while(*saveInfo)
    {
    	saveInfo += *saveInfo;
    }

    char* saveByte = &((char*)(&SAVE_DATA))[(unsigned)saveInfo[1]];

    if (*saveByte & saveInfo[2])
    {
    	return;
    }
    *saveByte |= saveInfo[2];

    RunKuppaScript (file);
}

void UpdateScriptFiles(char** file, size_t size)
{
	if (RUNNING_KUPPA_SCRIPT) return;

	for (unsigned i = 0; i < size >> 2; i++)
	{
		if (file[i])
		{
			_ZdlPv(file[i]);
			file[i] = nullptr;
		}
	}
}

char* LoadAndRunKuppaScript(int ov0FileID)
{
	if (RUNNING_KUPPA_SCRIPT) return nullptr;

	char* file = LoadFile(ov0FileID);
	RunKuppaScript(file);
	return file;
}

char* LoadKuppaScriptAndRunIfNotRanYet(int ov0FileID)
{
	if (RUNNING_KUPPA_SCRIPT) return nullptr;

	char* file = LoadFile(ov0FileID);
	RunKuppaScriptIfNotRanYet(file);
	return file;
}