#include "Noteblock.h"

namespace
{

	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x0, 0x6, 0x00, 1, 0, 0, 0xff)
        }
	};
}

SharedFilePtr Noteblock::modelFile;
SharedFilePtr Noteblock::clsnFile;

SpawnInfo<Noteblock> Noteblock::spawnData =
{
	&Noteblock::Spawn,
	0x0034,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

Noteblock* Noteblock::Spawn()
{
	return new Noteblock;
}

void Noteblock::OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor) {

	static_cast<Noteblock*>(clsnActor)->stage = 1;
	static_cast<Noteblock*>(clsnActor)->oldPos = static_cast<Noteblock*>(clsnActor)->pos;
	static_cast<Noteblock*>(clsnActor)->isLaunching = true;

}

void Noteblock::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowScaleXYZ(shadow, model.mat4x3, 0x60000_f, 0x150000_f, 0x60000_f, 0xc);
}

//Jiggles block down then up.
void Noteblock::jiggle() {

	switch (stage) {

		case 1:
			pos.y = pos.y - 0x9600_f;
			if (pos.y < oldPos.y - 0x20202_f) {
				stage = 2;
			}
			break;

		case 2:
			pos.y = pos.y + 0x6400_f;
			if (pos.y > oldPos.y) {
				stage = 3;
			}
			break;

		case 3:
			pos.y = oldPos.y;
			stage = 0;
			break;

	}

}

//Launches player.
void Noteblock::launch() {

	PLAYER_ARR[0]->ChangeState(*(Player::State*)Player::ST_FALL);
	if (boost) {
		PLAYER_ARR[0]->speed.y = 0x1000_f * param1;
		boost = false;
	} else {
		PLAYER_ARR[0]->speed.y = 0x37000_f;	
	}
	isLaunching = false;

}

int Noteblock::InitResources()
{
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);

	MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	
	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	clsn.afterClsnCallback = &OnFloorAfterClsn;
	
	shadow.InitCuboid();
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	return 1;
}

int Noteblock::CleanupResources()
{
	clsn.Disable();
	modelFile.Release();
	clsnFile.Release();
	return 1;
}

int Noteblock::Behavior()
{
	UpdateModelTransform();
	if(IsClsnInRange(0_f, 0_f))
	{

		if (stage > 0 && stage < 4) {
			jiggle();
		}

		if (isLaunching) {
			launch();
			Sound::Play(4, 2, camSpacePos);
		}

		if (PLAYER_ARR[0]->pos.y >= pos.y && INPUT_1_FRAME & Input::B && (unsigned)PLAYER_ARR[0]->currState == Player::ST_FALL && PLAYER_ARR[0]->pos.y <= pos.y + 0x3E8000_f && PLAYER_ARR[0]->speed.y < 0_f) {
			boost = true;
		}

		if (PLAYER_ARR[0]->pos.y >= pos.y) {
			IMMUNE_TO_DAMAGE = true;
		} else {
			IMMUNE_TO_DAMAGE = false;
		}

		UpdateClsnPosAndRot();

	} else {

		IMMUNE_TO_DAMAGE = false;

	}
	
	return 1;
}

int Noteblock::Render()
{
	model.Render(nullptr);
	return 1;
}

Noteblock::~Noteblock() {}
