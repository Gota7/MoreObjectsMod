#ifndef POWER_UP_BLOCK_INCLUDED
#define POWER_UP_BLOCK_INCLUDED

#include "include/SM64DS_2.h"

struct CustomBlock : public Platform
{
	enum BlockType
	{
		YELLOW = 0,
		RED = 1
	};
	
	void UpdateModelTransform();

	static CustomBlock* Spawn();
	virtual void Jiggle();
	virtual void JumpedUnderBlock();
	virtual bool CheckUnlock();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual void OnGroundPounded(Actor& groundPounder) override;
	virtual void OnAttacked1(Actor& attacker) override;
	virtual void OnAttacked2(Actor& attacker) override;
	virtual void OnKicked(Actor& kicker) override;
	virtual void OnHitByMegaChar(Player& megaChar) override;
	virtual ~CustomBlock();
	
	int stage;
	bool canRender;
	bool hasClsn;
	bool canBeHit;
	bool spawnPowerUp;
	bool renderFrame;
	bool respawn;
	int frameCounter;
	Vector3 oldPos;
	ModelAnim modelAnim;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	
	Fix12i originalScaleXr0;
	Fix12i originalScaleXr1;
	Fix12i originalScaleXr2;
	Fix12i originalScaleYr0;
	Fix12i originalScaleYr1;
	Fix12i originalScaleYr2;
	Fix12i originalScaleZr0;
	Fix12i originalScaleZr1;
	Fix12i originalScaleZr2;
	
	Fix12i prevScaleXr0;
	Fix12i prevScaleXr1;
	Fix12i prevScaleXr2;
	Fix12i prevScaleYr0;
	Fix12i prevScaleYr1;
	Fix12i prevScaleYr2;
	Fix12i prevScaleZr0;
	Fix12i prevScaleZr1;
	Fix12i prevScaleZr2;
	
	char silverStarID;
	unsigned starID;
	unsigned blockType;
	unsigned itemType;
	unsigned myParticle;
	bool isUnlocked;
	
	static SpawnInfo<CustomBlock> spawnData;

	static SharedFilePtr modelFiles[3];
	static SharedFilePtr clsnFile;
	static SharedFilePtr animFiles[2];
};

#endif
