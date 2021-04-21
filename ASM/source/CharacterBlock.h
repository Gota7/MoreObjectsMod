#ifndef CHARACTER_BLOCK_INCLUDED
#define CHARACTER_BLOCK_INCLUDED

#include "SM64DS_2.h"

struct CharacterBlock : public Platform
{	
	void UpdateModelTransform();

	static CharacterBlock* Spawn();
	virtual void Jiggle();
	virtual void JumpedUnderBlock();
	virtual bool CheckUnlock();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~CharacterBlock();
	
	int stage;
	bool healPlayer;
	Vector3 oldPos;
	ModelAnim modelAnim;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	
	unsigned blockType;
	unsigned myParticle;
	unsigned startingCharacter;
	bool needsUnlock;
	bool isUnlocked;
	int soundIDs[4];
	
	static SpawnInfo<CharacterBlock> spawnData;

	static SharedFilePtr modelFiles[4];
	static SharedFilePtr modelFilesTrans[4];
	static SharedFilePtr clsnFile;
	static SharedFilePtr animFiles[2];
};

#endif
