#include "InvisibleWall.h"

namespace
{
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x5, 0x0, 0x00, 0, 0, 0, 0xff)
        }
	};
}

SharedFilePtr InvisibleWall::clsnFile;

SpawnInfo<InvisibleWall> InvisibleWall::spawnData =
{
	&InvisibleWall::Spawn,
	0x0000,
	0x0100,
	0x00000003,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

InvisibleWall* InvisibleWall::Spawn()
{
	return new InvisibleWall;
}

void InvisibleWall::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}


int InvisibleWall::InitResources()
{

	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	char* clsnF = MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnF, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	
	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	
	return 1;
}

int InvisibleWall::CleanupResources()
{
	clsn.Disable();
	clsnFile.Release();
	return 1;
}

int InvisibleWall::Behavior()
{
	IsClsnInRange(0_f, 0_f);
	
	return 1;
}

int InvisibleWall::Render()
{
	return 1;
}

InvisibleWall::~InvisibleWall() {}
