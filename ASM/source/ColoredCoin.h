#ifndef COLORED_COIN_INCLUDED
#define COLORED_COIN_INCLUDED

#include "SM64DS_2.h"

struct ColoredCoin : public Platform
{	
	void UpdateModelTransform();

	static ColoredCoin* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	void HandleClsn();
	void Kill();
	virtual unsigned OnYoshiTryEat() override;
	virtual void OnTurnIntoEgg(Player& player) override;
	virtual ~ColoredCoin();
	
	static SharedFilePtr modelFile;
	CylinderClsn cylClsn;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	bool killable = true;
	bool fake;
	bool hurt;
	int health;
	int value;

	static SpawnInfo<ColoredCoin> spawnData;
};

#endif
