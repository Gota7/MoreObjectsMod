#include "ToxBoxFlower.h"
#include "ToxBox.h"

namespace
{
	constexpr Fix12i SIDE_LEN = 0x80000_f;
	constexpr Fix12i SQRT_HALF = 0xb50_f;
	constexpr Fix12i HEIGHT = 0xaaaaa_f;
	
	constexpr short GONE_TIME = 150;
};

SharedFilePtr ToxBoxFlower::modelFile;

SpawnInfo<ToxBoxFlower> ToxBoxFlower::spawnData =
{
	&ToxBoxFlower::Spawn,
	0x016d,
	0x00aa,
	0x00000002,
	0x00032000_f,
	0x00060000_f,
	0x01000000_f,
	0x01000000_f
};

ToxBoxFlower* ToxBoxFlower::Spawn()
{
	return new ToxBoxFlower;
}

void ToxBoxFlower::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	
	if(goneTimer == 0 && !(flags & Actor::IN_YOSHI_MOUTH))
	{
		shadowMat = model.mat4x3;
		Vector3 shadowPosAsr3 = (Vector3{shadowMat.r0c1, shadowMat.r1c1, shadowMat.r2c1} * 0x14000_f + pos) >> 3;
		shadowMat.r0c3 = shadowPosAsr3.x;
		shadowMat.r1c3 = shadowPosAsr3.y;
		shadowMat.r2c3 = shadowPosAsr3.z;
		DropShadowScaleXYZ(shadow, shadowMat, SIDE_LEN, 0x32000_f, SIDE_LEN, 0xf);
	}
}

void ToxBoxFlower::PowerUp(Player& player)
{
	if(player.currState != &PlayerToxBox::ST_TOX_BOX)
	{
		PlayerToxBox::playerToPowerUp = &player;
		Actor::Spawn(0x22B, player.param1 << 8, pos, &ang, player.areaID, -1);
		Sound::PlayBank2_2D(23);
	}
	goneTimer = GONE_TIME;
}

int ToxBoxFlower::InitResources()
{
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);
	
	cylClsn.Init(this, SIDE_LEN * SQRT_HALF, HEIGHT, 0x00200000, 0x00008000);
	shadow.InitCuboid();
	
	UpdateModelTransform();
	origPos = pos;
	
	Model::LoadFile(PlayerToxBox::modelFile);
	TextureSequence::LoadFile(PlayerToxBox::texSeqFile);
	
	return 1;
}

int ToxBoxFlower::CleanupResources()
{
	PlayerToxBox::modelFile.Release();
	PlayerToxBox::texSeqFile.Release();
	modelFile.Release();
	return 1;
}

int ToxBoxFlower::Behavior()
{
	bool here = DecIfAbove0_Short(goneTimer) == 0;
	
	if(here)
	{
		Actor* actor = Actor::FindWithID(cylClsn.otherObjID);
		if(actor && actor->actorID == 0x00bf)
			PowerUp(*(Player*)actor);
	}
	
	cylClsn.Clear();
	if(here)
		cylClsn.Update();	
	UpdateModelTransform();
	
	return 1;
}

int ToxBoxFlower::Render()
{
	if(goneTimer == 0 && !(flags & Actor::IN_YOSHI_MOUTH))
		model.Render(nullptr);
	return 1;
}

ToxBoxFlower::~ToxBoxFlower() {}

unsigned ToxBoxFlower::OnYoshiTryEat()
{
	return Actor::YE_SWALLOW;
}

void ToxBoxFlower::OnTurnIntoEgg(Player& player)
{
	flags &= ~Actor::IN_YOSHI_MOUTH;
	pos = origPos;
	PowerUp(player);
}
