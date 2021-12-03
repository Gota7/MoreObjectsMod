#ifndef COLOREDPIPE_INCLUDED
#define COLOREDPIPE_INCLUDED

#include "include/SM64DS_2.h"

struct ColoredPipe : public Platform
{	
	void UpdateModelTransform();
	
	static ColoredPipe* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ColoredPipe();
	
	static SharedFilePtr modelFile;
	static SharedFilePtr clsnFile;

	static SpawnInfo<ColoredPipe> spawnData;
};

#endif
