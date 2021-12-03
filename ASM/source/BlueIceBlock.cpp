#include "BlueIceBlock.h"

namespace
{
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x07, 0, 0x3f, 0x4, 0x8, 0x0E, 1, 0, 0, 0xff)
        }
	};
}

SharedFilePtr BlueIceBlock::modelFile;
SharedFilePtr BlueIceBlock::clsnFile;

SpawnInfo<BlueIceBlock> BlueIceBlock::spawnData =
{
	&BlueIceBlock::Spawn,
	0x0034,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

BlueIceBlock* BlueIceBlock::Spawn()
{
	return new BlueIceBlock;
}

void BlueIceBlock::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}

int BlueIceBlock::InitResources()
{
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);

	MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	return 1;
}

int BlueIceBlock::CleanupResources()
{
	clsn.Disable();
	clsnFile.Release();
	modelFile.Release();
	return 1;
}

int BlueIceBlock::Behavior()
{
	UpdateModelTransform();
	
	if(IsClsnInRange(0_f, 0_f))
		UpdateClsnPosAndRot();
	
	return 1;
}

int BlueIceBlock::Render()
{
	model.Render(nullptr);
	return 1;
}

BlueIceBlock::~BlueIceBlock() {}
