#include "SilverCoin.h"

namespace
{
	/*FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x0, 0x0, 0x00, 0, 0, 0, 0xff)
        }
	};*/
	constexpr Fix12i RADIUS = 0x64000_f;
	constexpr Fix12i HEIGHT = 0x64000_f;
	constexpr short int ROT_SPEED = 0xC00;
	constexpr short int ACTOR_ID = 0x191;
	constexpr short int OBJECT_ID = 0x191;
}

SharedFilePtr SilverCoin::modelFile;

SpawnInfo<SilverCoin> SilverCoin::spawnData =
{
	&SilverCoin::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

SilverCoin* SilverCoin::Spawn()
{
	return new SilverCoin;
}

void SilverCoin::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowRadHeight(shadow, shadowMat, RADIUS, 0x200000_f, 0xf);
}


int SilverCoin::InitResources()
{
	char* modelF = Model::LoadFile(modelFile);
	model.SetFile(modelF, 1, -1);
	shadow.InitCylinder();
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	cylClsn.Init(this, RADIUS, HEIGHT, 0x00100002, 0x8000);
	//char* clsnF = MovingMeshCollider::LoadFile(clsnFile);
	//clsn.SetFile(clsnF, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	
	//clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	
	return 1;
}

int SilverCoin::CleanupResources()
{
	modelFile.Release();
	return 1;
}

int SilverCoin::Behavior()
{
	ang.y += ROT_SPEED;
	UpdateModelTransform();
	HandleClsn();
	cylClsn.Clear();
	cylClsn.Update();
	return 1;
}

int SilverCoin::Render()
{
	model.Render(nullptr);
	return 1;
}

void SilverCoin::HandleClsn() {
	Actor* other = Actor::FindWithID(cylClsn.otherObjID);
	if(!other)
		return;
	if(other->actorID != 0x00bf)
		return;
	unsigned hitFlags = cylClsn.hitFlags;
	if ((hitFlags & 0x8000) == 0 && killable) {
		Kill();
	} else {
		killable = false;
	}
}

void SilverCoin::OnTurnIntoEgg(Player& player)
{
	Kill();
	Destroy();
}

unsigned SilverCoin::OnYoshiTryEat()
{
	return Actor::YE_SWALLOW;
}

void SilverCoin::Kill()
{
	Particle::System::NewSimple(0xD2, pos.x, pos.y + 0x28000_f, pos.z);
	Sound::PlayBank3(17, camSpacePos);
	KillAndTrackInDeathTable();
	asm("mov r0, #0"); //Player ID.
	asm("mov r1, #1"); //Number of coins.
	asm("bl 0x0202A7B8"); //Collect coin function.
}

SilverCoin::~SilverCoin() {}
