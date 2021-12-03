#ifndef TOX_BOX_INCLUDED
#define TOX_BOX_INCLUDED

#include "include/SM64DS_2.h"

//@02130f00
struct ToxBox : public Platform
{	
	unsigned state;
	unsigned stateTimer;
	
	unsigned numPathPts;
	uint8_t currPathPtID;
	bool backAndForth;
	bool backward;
	uint8_t turnDir;
	short turnAng;
	short destTurnAng;
	Vector3 prevPathPt;
	Vector3 currPathPt;
	PathPtr pathPtr;
	
	Matrix4x3 baseMat;
	Matrix4x3 orientMat;
	Matrix4x3 turnMat;
	Matrix4x3 baseMatInv;
	Vector3 normal;
	Vector3 radiusNormal;
	Vector3 targetPos;
	Vector3 pivotCenter;
	Vector3 pivotHorzDist;
	int numTurns;
	
	
	void UpdateClsnTransform();
	void UpdateModelTransform();
	void AdvancePath();
	void InitMoveState(); //returns whether to move or jump
	Matrix4x3 GetTurnMat(short ang);
	void SetTurnMat(Matrix4x3& theTurnMat, short ang);
	void SetBaseMat();
	void Move();
	
	int InitCommonResources();
	
	void StHitGround();
	void StStop();
	void StMove();
	void StJump();
	void StAntigravity();
	
	static ToxBox* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~ToxBox();
	
	static SharedFilePtr modelFiles[2];
	static SharedFilePtr clsnFiles[2];

	static SpawnInfo<ToxBox> spawnData;
};

struct PlayerToxBox : public ToxBox
{
	TextureSequence texSeq;
	Vector3 downDir;
	Vector3 rightDir;
	Player* player;
	CylinderClsnWithPos playerClsn;
	bool shouldDestroy;
	
	void CalcMoveDirs();
	
	static PlayerToxBox* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual void Virtual30() override;
	virtual ~PlayerToxBox();
	
	static SharedFilePtr modelFile;
	static SharedFilePtr texSeqFile;
	
	static Player* playerToPowerUp;
	static Player::State ST_TOX_BOX;
	static PlayerToxBox* playerToxBox;

	static SpawnInfo<PlayerToxBox> spawnData;
};

#endif