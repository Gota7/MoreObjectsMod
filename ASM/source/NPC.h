#ifndef YOSHI_NPC_INCLUDED
#define YOSHI_NPC_INCLUDED

#include "include/SM64DS_2.h"

struct NPC : public Actor
{
	ModelAnim rigMdl;
	TextureSequence texSeq;
	CylinderClsn cylClsn;
	ShadowVolume shadow;
	uint8_t state;
	uint8_t eventID;
	uint8_t starID;
	bool starSpawned;
	bool shouldTalk;
	Player* listener;
	uint16_t messages[2];
	unsigned npcID;
	
	void UpdateModelTransform();
	
	void State0_Wait();
	void State1_Talk();
	
	static NPC* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~NPC() override;
	
	static SharedFilePtr modelFiles[3];
	static SharedFilePtr animFiles[6];
	static SharedFilePtr texSeqFile;
	
	static SpawnInfo<NPC> spawnData;
};

#endif