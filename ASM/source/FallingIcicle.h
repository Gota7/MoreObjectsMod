#ifndef FALLINGICICLE_INCLUDED
#define FALLINGICICLE_INCLUDED

#include "SM64DS_2.h"

struct FallingIcicle : public Platform
{
	void UpdateModelTransform();

	static FallingIcicle* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~FallingIcicle() override;

	void Reset();
	void Shake();
	void Fall();
	void Break();

	static void OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor);

	//void(*beforeClsnCallback)(MeshCollider& clsn, Actor* clsnActor, ClsnResult& wmClsnResult, Vector3* posToUpdate, Vector3_16* motionAngToUpdate, Vector3_16* angToUpdate) doExternClsnStuff;

	void HandleClsn();
	//void Kill();

	unsigned currState = 0; //0 - Still, 1 - Shaking, 2 - Falling, 3 - Breaking.
	Vector3 origPos;

	CylinderClsn cylClsn;
	
	static SharedFilePtr modelFile;

	static SpawnInfo<FallingIcicle> spawnData;

};

#endif