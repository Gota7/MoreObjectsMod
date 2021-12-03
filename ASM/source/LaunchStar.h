#ifndef LAUNCH_STAR_INCLUDED
#define LAUNCH_STAR_INCLUDED

#include "include/SM64DS_2.h"

struct LaunchStar : public Actor
{
	ModelAnim rigMdl;
	CylinderClsn cylClsn;
	
	Fix12i launchSpeed;
	PathPtr pathPtr;
	uint8_t eventID;
	unsigned particleID;
	bool camSet;
	int camFrames;
	bool usesSave;
	
	Vector3 camOldPos;
	Vector3 camOldTarget;
	Vector3 camNewPos;
	Vector3 playerOldPos;
	Vector3 playerNewPos;
	Vector3_16 playerOldAng;
	Player::State* playerOldState;
	Player::State* playerOldPrevState;
	Player::State* playerOldNextState;
	
	void UpdateModelTransform();

	static LaunchStar* Spawn();
	virtual int InitResources() override;
	virtual int CleanupResources() override;
	virtual int Behavior() override;
	virtual int Render() override;
	virtual ~LaunchStar();

	static SharedFilePtr modelFile;
	static SharedFilePtr animFiles[2];

	static LaunchStar* ls_ptr;
	static BezierPathIter bzIt;
	static Vector3_16 lsDiffAng;
	static Vector3_16 lsInitAng;

	static SpawnInfo<LaunchStar> spawnData;
};

#endif