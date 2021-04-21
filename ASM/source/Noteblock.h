#ifndef NOTEBLOCK_INCLUDED
#define NOTEBLOCK_INCLUDED

#include "SM64DS_2.h"

struct Noteblock : public Platform
{	
	void UpdateModelTransform();

	static Noteblock* Spawn();
	static void OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor);
	virtual void jiggle();
	virtual void launch();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~Noteblock();

	int stage;
	Vector3 oldPos;
	bool isLaunching;
	bool boost;
	
	ShadowVolume shadow;
	Matrix4x3 shadowMat;

	static SpawnInfo<Noteblock> spawnData;

	static SharedFilePtr modelFile;
	static SharedFilePtr clsnFile;
};

#endif
