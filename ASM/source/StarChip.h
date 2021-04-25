#ifndef STAR_CHIP_INCLUDED
#define STAR_CHIP_INCLUDED

#include "SM64DS_2.h"

struct StarChip : public Platform
{	
	void UpdateModelTransform();

	static StarChip* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	void HandleClsn();
	void Kill();
	virtual unsigned OnYoshiTryEat() override;
	virtual void OnTurnIntoEgg(Player& player) override;
	virtual ~StarChip();
	
	uint8_t eventID;
	bool killable = true;
	
	CylinderClsn cylClsn;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	
	static SharedFilePtr modelFile;
	static int chipCounter;

	static SpawnInfo<StarChip> spawnData;
};

#endif
