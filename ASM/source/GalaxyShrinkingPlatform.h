#ifndef GALAXYSHRINKINGPLATFORM_INCLUDED
#define GALAXYSHRINKINGPLATFORM_INCLUDED

#include "include/SM64DS_2.h"

struct GalaxyShrinkingPlatform : public Platform
{	
	void UpdateModelTransform();

	static GalaxyShrinkingPlatform* Spawn();
	static void OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor);
	void Shrink();
	void UnShrink();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~GalaxyShrinkingPlatform();

	Vector3 skl = Vector3{0x1000_f, 0x1000_f, 0x1000_f};
	bool shrinkActivated;
	//bool playSound;
	Model frameModel;

	static SharedFilePtr modelFile;
	static SharedFilePtr clsnFile;
	static SharedFilePtr frameModelFile;

	static SpawnInfo<GalaxyShrinkingPlatform> spawnData;
};

#endif
