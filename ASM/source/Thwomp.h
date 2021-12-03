#ifndef THWOMP_INCLUDED
#define THWOMP_INCLUDED

#include "include/SM64DS_2.h"

struct Thwomp : public Platform
{	
	enum ThwompType
	{
		NORMAL = 0,
		MAGMA = 1,
		GIGA = 2
	};
	
	//Resources* resPtr; 
	TextureSequence texSeq;
	ShadowVolume shadow;
	Matrix4x3 shadowMat;
	Fix12i maxPosY;
	Fix12i minPosY;
	Fix12i maxPosYSide;
	Fix12i minPosYSide;
	unsigned state;
	uint8_t waitTime;
	uint16_t actionWaitTime;
	unsigned thwompType;
	
	//Giga Thwomp
	uint8_t starID;
	uint8_t health;
	bool battleStarted;
	bool justHit;
	Vector3 originalPos;
	Player* listener;
	uint8_t curSlam;
	
	void UpdateModelPosAndRotXYZ();
	void GoUp();
	void WaitUp();
	void HitGround();
	void WaitGround();
	void WaitGround2();
	
	void DropShadow();
	
	static Thwomp* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~Thwomp();
	virtual void OnHitByMegaChar(Player& megaChar);
	virtual void OnGroundPounded(Actor& groundPounder) override;
	
	void AttemptTalkStartIfNotStarted();
	void Talk();
	
	static SharedFilePtr modelFiles[3];
	static SharedFilePtr texSeqFile;
	static SharedFilePtr clsnFile;

	static SpawnInfo<Thwomp> spawnData;
};

#endif