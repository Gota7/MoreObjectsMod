#ifndef BLUE_ICE_BLOCK_INCLUDED
#define BLUE_ICE_BLOCK_INCLUDED

#include "include/SM64DS_2.h"

struct BlueIceBlock : public Platform
{	
	void UpdateModelTransform();

	static BlueIceBlock* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~BlueIceBlock();
	
	Model model;
	
	static SpawnInfo<BlueIceBlock> spawnData;

	static SharedFilePtr modelFile;
	static SharedFilePtr clsnFile;
};

#endif
