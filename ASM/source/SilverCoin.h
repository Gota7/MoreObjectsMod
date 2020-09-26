#ifndef SILVERCOIN_INCLUDED
#define SILVERCOIN_INCLUDED

#include "SM64DS_2.h"

struct SilverCoin : public Platform
{	
	void UpdateModelTransform();

	static SilverCoin* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	void HandleClsn();
	void Kill();
	virtual unsigned OnYoshiTryEat() override;
	virtual void OnTurnIntoEgg(Player& player) override;
	virtual ~SilverCoin();
	
	static SharedFilePtr modelFile;
	CylinderClsn cylClsn;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	bool killable = true;

	static SpawnInfo<SilverCoin> spawnData;
};

#endif
