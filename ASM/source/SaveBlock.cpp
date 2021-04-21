#include "SaveBlock.h"

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

SharedFilePtr SaveBlock::modelFile;
SharedFilePtr SaveBlock::texSeqFile;
SharedFilePtr SaveBlock::clsnFile;

SpawnInfo<SaveBlock> SaveBlock::spawnData =
{
	&SaveBlock::Spawn,
	0x0034,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

SaveBlock* SaveBlock::Spawn()
{
	return new SaveBlock;
}

asm("SaveGame = 0x02013b9c");
extern "C" bool SaveGame();

void SaveBlock::JumpedUnderBlock()
{
	if (saveable)
	{
		Sound::Play(2, 94, camSpacePos);
		SaveGame();
		Particle::System::NewSimple(0x09, pos.x, pos.y, pos.z);
		Particle::System::NewSimple(0x09, pos.x, pos.y, pos.z);
		stage = 1;
	}
	return;
}

//Jiggles block up then down.
void SaveBlock::Jiggle()
{
	switch (stage)
	{
		case 1:
			pos.y = pos.y + 0x9600_f;
			if (pos.y > oldPos.y + 0x20202_f) {
				stage = 2;
			}
			saveable = false;
			break;
		
		case 2:
			pos.y = pos.y - 0x6400_f;
			if (pos.y < oldPos.y) {
				stage = 3;
			}
			break;
		
		case 3:
			pos.y = oldPos.y;
			stage = 0;
			saveable = true;
			break;
	}
}

void SaveBlock::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowScaleXYZ(shadow, model.mat4x3, 0x85000_f, 0x150000_f, 0x85000_f, 0xc);
}

int SaveBlock::InitResources()
{
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);
	
	TextureSequence::LoadFile(texSeqFile);
	TextureSequence::Prepare(modelFile.filePtr, texSeqFile.filePtr);
	texSeq.SetFile(texSeqFile.filePtr, Animation::LOOP, 0x10000_f, 1);
	
	MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x190_f, ang.y, clpsBlock);
	
	shadow.InitCuboid();
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	saveable = true;
	stage = 0;
	oldPos = pos;
	
	return 1;
}

int SaveBlock::CleanupResources()
{
	clsn.Disable();
	clsnFile.Release();
	
	modelFile.Release();
	texSeqFile.Release();
	
	return 1;
}

int SaveBlock::Behavior()
{
	UpdateModelTransform();
	
	if(IsClsnInRange(0_f, 0_f))
	{
		Player* player = ClosestPlayer();
		
		if (BumpedUnderneathByPlayer(*player) && player->pos.x < pos.x + 0x53500_f && player->pos.x > pos.x - 0x53500_f && player->pos.z < pos.z + 0x53500_f && player->pos.z > pos.z - 0x53500_f)
		{
			JumpedUnderBlock();
		}
		
		if (stage > 0 && stage < 4) {
			Jiggle();
		}
		
		UpdateClsnPosAndRot();
	}
	
	return 1;
}

int SaveBlock::Render()
{
	model.Render(nullptr);
	texSeq.Update(model.data);
	texSeq.Advance();
	return 1;
}

SaveBlock::~SaveBlock() {}
