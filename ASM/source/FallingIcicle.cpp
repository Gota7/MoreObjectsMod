#include "FallingIcicle.h"

namespace {

	constexpr Fix12i TRIGGER_RADIUS = 0x180000_f;
	constexpr Fix12i RESPAWN_RADIUS = 0x700000_f;
	constexpr Fix12i FALL_SPEED_MAX = 0x50000_f;
	constexpr Fix12i FALL_ACCEL = 0x6000_f;
	constexpr Fix12i GROUND = 0x10000_f;
	constexpr unsigned DAMAGE = 1;
	constexpr Fix12i PLAYER_KNOCKBACK = 0xc000_f;
	//constexpr Fix12i RADIUS = 0x64000_f;
	//constexpr Fix12i HEIGHT = 0xA4000_f;

	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x0, 0x6, 0, 0, 0, 0, 0xff)
        }
	};
}

SharedFilePtr FallingIcicle::modelFile;

SpawnInfo<FallingIcicle> FallingIcicle::spawnData =
{
	&FallingIcicle::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

FallingIcicle* FallingIcicle::Spawn()
{
	return new FallingIcicle;
}

/*void FallingIcicle::OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor)
{
	//Player.
	if (otherActor->actorID == 0x00bf) {
		PLAYER_ARR[0]->Hurt(clsnActor->pos, DAMAGE, PLAYER_KNOCKBACK, 1, 0, 1);
	}
}*/

void FallingIcicle::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationXYZExt(ang.x, ang.y, ang.z);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}

int FallingIcicle::InitResources()
{
	origPos = pos;
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);

	//MovingMeshCollider::LoadFile(clsnFile);
	//clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x1000_f, ang.y, clpsBlock);

	//clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	//lsn.afterClsnCallback = &OnFloorAfterClsn;

	//cylClsn.Init(this, RADIUS, HEIGHT, 0x00200000, 0x00a6efe0);

	UpdateModelTransform();
	//UpdateClsnPosAndRot();

	return 1;
}

int FallingIcicle::CleanupResources()
{
	//clsn.Disable();
	modelFile.Release();
	//clsnFile.Release();
	return 1;
}

int FallingIcicle::Behavior()
{
	/*IsClsnInRange(0_f, 0_f);
	if (PLAYER_ARR[0]->pos.x >= pos.x - RESPAWN_RADIUS && PLAYER_ARR[0]->pos.x <= pos.x + RESPAWN_RADIUS) {
		switch (currState) {

			//Still.
			case 0:
				if (PLAYER_ARR[0]->pos.x >= pos.x - TRIGGER_RADIUS && PLAYER_ARR[0]->pos.x <= pos.x + TRIGGER_RADIUS) {
					currState++;
				}
				break;

			//Shaking.
			case 1:
				Shake();
				break;

			//Falling.
			case 2:
				Fall();
				break;

			//Breaking.
			case 3:
				Break();
				break;

		}
		UpdatePos(nullptr);
		UpdateModelTransform();
		//HandleClsn();
		//cylClsn.Clear();
		//cylClsn.Update();
		UpdateModelPosAndRotY();
		UpdateClsnPosAndRot();
	} else if (!(PLAYER_ARR[0]->pos.x >= pos.x - TRIGGER_RADIUS && PLAYER_ARR[0]->pos.x <= pos.x + TRIGGER_RADIUS)) {
		currState = 0;
		Reset();
	} else {
		Reset();
	}*/
	return 1;
}

int FallingIcicle::Render()
{
	model.Render(nullptr);
	return 1;
}

void FallingIcicle::Reset()
{
	pos = origPos;
	ang.z = 0;
	vertAccel = 0_f;
	termVel = 0_f;
}

void FallingIcicle::Shake()
{
	currState++;
}

void FallingIcicle::Fall()
{
	if (pos.y < GROUND) {
		vertAccel = 0_f;
		termVel = 0_f;
	} else {
		vertAccel = -FALL_ACCEL;
		termVel = -FALL_SPEED_MAX;
	}
}

void FallingIcicle::Break()
{

}

//void FallingIcicle::HandleClsn()
//{
	//Actor* other = Actor::FindWithID(cylClsn.otherObjID);
	//if(!other)
	//	return;
	//if(other->actorID != 0x00bf)
	//	return;
	//PLAYER_ARR[0]->Hurt(pos, DAMAGE, PLAYER_KNOCKBACK, 1, 0, 1);
//}

/*void FallingIcicle::Kill()
{
	KillAndTrackInDeathTable();
	Destroy();
}*/

FallingIcicle::~FallingIcicle() {}