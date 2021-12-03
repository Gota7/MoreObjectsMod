#ifndef COLORED_COIN_INCLUDED
#define COLORED_COIN_INCLUDED

#include "include/SM64DS_2.h"

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
	Actor* spawnedStar;
	bool killable = true;
	bool fake;
	bool hurt;
	bool deathCoin;
	bool deathStarted;
	int deathFrames;
	int frameCounter;
	int health;
	int value;
	uint8_t starID;

	static SpawnInfo<ColoredCoin> spawnData;
};

#endif
