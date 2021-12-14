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
	if (!deathStarted || !deathCoin)
		DropShadowRadHeight(shadow, shadowMat, RADIUS, 0x200000_f, 0xf);
}


int ColoredCoin::InitResources()
{
	value = ang.x & 0xff;
	health = ang.x >> 8 & 0xf;
	hurt = (ang.x >> 12 & 0xf) == 1;
	fake = value == 0;
	deathCoin = ang.z != 0;
	
	deathFrames = ang.z & 0x0fff;
	starID = ang.z & 0xf000;
	frameCounter = 0;
	
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
	
	if (deathStarted)
	{
		frameCounter++;
		
		if (PLAYER_ARR[0]->pos == spawnedStar->pos)
		{
			deathStarted = false;
			KillAndTrackInDeathTable();
		}
		
		else if (frameCounter == deathFrames)
		{
			//Make sure to kill the player properly
			if (PLAYER_HEALTH == 1)
			{
				ClosestPlayer()->Hurt(pos, 1, 0xc000_f, 1, 0, 1);
				deathStarted = false;
				KillAndTrackInDeathTable();
			}
			else if (PLAYER_HEALTH <= 8 && PLAYER_HEALTH > 1)
				PLAYER_HEALTH -= 1;
			
			frameCounter = 0;
		}
	}
	
	return 1;
}

int ColoredCoin::Render()
{
	if (!deathStarted || !deathCoin)
		model.Render(nullptr);
	return 1;
}

void ColoredCoin::HandleClsn()
{
	if (!deathStarted || !deathCoin)
	{
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
}

void ColoredCoin::OnTurnIntoEgg(Player& player)
{
	Kill();
	if (!deathCoin)
		Destroy();
}

unsigned ColoredCoin::OnYoshiTryEat()
{
	if (deathStarted && deathCoin)
		return Actor::YE_DONT_EAT;
	return Actor::YE_SWALLOW;
}

asm("CollectCoin = 0x0202A7B8");
extern "C" void CollectCoin(int playerID, int numCoins);

void ColoredCoin::Kill()
{
	if (!deathCoin)
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
			ClosestPlayer()->Hurt(pos, health, 0xc000_f, 1, 0, 1);
		else
			PLAYER_HEALTH += (PLAYER_HEALTH + health > 8 ? 8 - PLAYER_HEALTH : health);
		
		
		CollectCoin(0, value);
	}
	else
	{
		PLAYER_HEALTH -= 1;
		deathStarted = true;
		
		Actor* starMarker = nullptr;
		spawnedStar = nullptr;
		
		while (starMarker = FindWithActorID(0x00b4, starMarker))
		{
			if (starMarker->param1 == 0x20 + starID && spawnedStar == nullptr)
				spawnedStar = Actor::Spawn(0x00b2, 0x0040 + starID, starMarker->pos, nullptr, areaID, -1);
		}
		
		Particle::System::NewSimple(0xD2, pos.x, pos.y + 0x28000_f, pos.z);
		Sound::PlayBank3(17, camSpacePos);
	}
}

ColoredCoin::~ColoredCoin() {}
