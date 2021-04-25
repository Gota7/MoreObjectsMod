#include "StarChip.h"

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
	constexpr short int ROT_SPEED = 0x300;
	constexpr short int ACTOR_ID = 0x191;
	constexpr short int OBJECT_ID = 0x191;
}

SharedFilePtr StarChip::modelFile;
int StarChip::chipCounter;

SpawnInfo<StarChip> StarChip::spawnData =
{
	&StarChip::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

StarChip* StarChip::Spawn()
{
	return new StarChip;
}

void StarChip::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowRadHeight(shadow, model.mat4x3, 0x50000_f, 0x137000_f, 0xc);
}


int StarChip::InitResources()
{
	char* modelF = Model::LoadFile(modelFile);
	model.SetFile(modelF, 1, -1);
	shadow.InitCylinder();
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	cylClsn.Init(this, RADIUS, HEIGHT, 0x00100002, 0x8000);
	
	eventID = param1 & 0xff;
	SAVE_DATA.minigameRabbits1 = 0x00;
	
	return 1;
}

int StarChip::CleanupResources()
{
	modelFile.Release();
	return 1;
}

int StarChip::Behavior()
{
	ang.y += ROT_SPEED;
	UpdateModelTransform();
	HandleClsn();
	cylClsn.Clear();
	cylClsn.Update();
	return 1;
}

int StarChip::Render()
{
	model.Render(nullptr);
	return 1;
}

void StarChip::HandleClsn() {
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

void StarChip::OnTurnIntoEgg(Player& player)
{
	Kill();
	Destroy();
}

unsigned StarChip::OnYoshiTryEat()
{
	return Actor::YE_SWALLOW;
}

void StarChip::Kill()
{
	chipCounter++;
	
	Particle::System::NewSimple(0xD2, pos.x, pos.y + 0x28000_f, pos.z);
	pos.y += 0x20000_f;
	Actor* number = SpawnNumber(pos, chipCounter, false, 0, 0);
	Sound::Play(4, 3, camSpacePos);
	
	if (chipCounter >= 5)
	{
		Event::SetBit(eventID);
		chipCounter = 0;
	}
	
	KillAndTrackInDeathTable();
	UntrackInDeathTable();
}

StarChip::~StarChip() {}
