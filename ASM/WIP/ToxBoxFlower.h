#ifndef TOX_BOX_FLOWER_INCLUDED
#define TOX_BOX_FLOWER_INCLUDED

#include "include/SM64DS_2.h"

struct ToxBoxFlower : public Actor
{
	Model model;
	CylinderClsn cylClsn;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	Vector3 origPos;
	uint16_t goneTimer;
	
	void UpdateModelTransform();
	void PowerUp(Player& player);
	
	static ToxBoxFlower* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ToxBoxFlower() override;
	virtual unsigned OnYoshiTryEat() override;
	virtual void OnTurnIntoEgg(Player& player) override;
	
	static SharedFilePtr modelFile;
	
	static SpawnInfo<ToxBoxFlower> spawnData;
};

#endif