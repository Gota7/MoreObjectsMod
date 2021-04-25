#include "ColoredCoin.h"

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

SharedFilePtr ColoredCoin::modelFile;

SpawnInfo<ColoredCoin> ColoredCoin::spawnData =
{
	&ColoredCoin::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

ColoredCoin* ColoredCoin::Spawn()
{
	return new ColoredCoin;
}

void ColoredCoin::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowRadHeight(shadow, shadowMat, RADIUS, 0x200000_f, 0xf);
}


int ColoredCoin::InitResources()
{
	value = ang.x & 0xff;
	health = ang.x >> 8 & 0xf;
	hurt = (ang.x >> 12 & 0xf) == 1;
	fake = value == 0;
	
	char* modelF = Model::LoadFile(modelFile);
	model.SetFile(modelF, 1, -1);
	shadow.InitCylinder();
	
	model.data.materials[0].difAmb = (param1 & 0x7fff) << 16 | 0x8000;
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	shadowMat = model.mat4x3;
	shadowMat.r1c3 -= 0x1500_f;
	
	cylClsn.Init(this, RADIUS, HEIGHT, 0x00100002, 0x8000);
	
	return 1;
}

int ColoredCoin::CleanupResources()
{
	modelFile.Release();
	return 1;
}

int ColoredCoin::Behavior()
{
	ang.y += ROT_SPEED;
	UpdateModelTransform();
	HandleClsn();
	cylClsn.Clear();
	cylClsn.Update();
	return 1;
}

int ColoredCoin::Render()
{
	model.Render(nullptr);
	return 1;
}

void ColoredCoin::HandleClsn() {
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

void ColoredCoin::OnTurnIntoEgg(Player& player)
{
	Kill();
	Destroy();
}

unsigned ColoredCoin::OnYoshiTryEat()
{
	return Actor::YE_SWALLOW;
}

asm("CollectCoin = 0x0202A7B8");
extern "C" void CollectCoin(int playerID, int numCoins);

void ColoredCoin::Kill()
{
	KillAndTrackInDeathTable();
	
	if (fake)
	{
		Particle::System::NewSimple(0x0B, pos.x, pos.y + 0x28000_f, pos.z);
		Sound::Play(4, 4, camSpacePos);
	}
	else
	{
		Particle::System::NewSimple(0xD2, pos.x, pos.y + 0x28000_f, pos.z);
		Sound::PlayBank3(17, camSpacePos);
	}
	
	if (hurt)
		PLAYER_HEALTH -= (PLAYER_HEALTH - health < 0 ? PLAYER_HEALTH : health);
	else
		PLAYER_HEALTH += (PLAYER_HEALTH + health > 8 ? 8 - PLAYER_HEALTH : health);
	
	
	CollectCoin(0, value);
}

ColoredCoin::~ColoredCoin() {}
