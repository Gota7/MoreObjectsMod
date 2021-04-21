#ifndef SAVE_BLOCK_INCLUDED
#define SAVE_BLOCK_INCLUDED

#include "SM64DS_2.h"

struct SaveBlock : public Platform
{	
	void UpdateModelTransform();

	static SaveBlock* Spawn();
	virtual void Jiggle();
	virtual void JumpedUnderBlock();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~SaveBlock();
	
	int stage;
	bool saveable;
	Vector3 oldPos;
	
	Model model;
	TextureSequence texSeq;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	
	static SpawnInfo<SaveBlock> spawnData;

	static SharedFilePtr modelFile;
	static SharedFilePtr texSeqFile;
	static SharedFilePtr clsnFile;
};

#endif
