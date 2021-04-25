#include "DoorBlocker.h"

namespace
{
	FixedSizeCLPS_Block<2> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
			CLPS(0x00, 0, 0x3f, 0x0, 0x0, 0x00, 0, 0, 0, 0xff)
		}
	};
}

SharedFilePtr DoorBlocker::modelFile;
SharedFilePtr DoorBlocker::clsnFile;

SpawnInfo<DoorBlocker> DoorBlocker::spawnData =
{
	&DoorBlocker::Spawn,
	0x016e,
	0x00cc,
	0x00000002,
	0x0007d000_f,
	0x0012c000_f,
	0x01000000_f,
	0x01000000_f,
};

DoorBlocker* DoorBlocker::Spawn()
{
	return new DoorBlocker;
}

void DoorBlocker::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	//shadowMat = model.mat4x3;
	//shadowMat.r1c3 += 0x8000_f;
	
	//DropShadowScaleXYZ(shadow, shadowMat, 0x8e000_f, 0x32000_f, 0x5a000_f, 0xf);
}

int DoorBlocker::InitResources()
{
	char* modelF = Model::LoadFile(modelFile);
	model.SetFile(modelF, 1, -1);
	
	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	
	//shadow.InitCuboid();
	
	eventID = param1 & 0xff;
	gone = eventID < 0x20 && Event::GetBit(eventID);
	camSet = true;
	
	center = Vector3{0x50000_f * Sin(ang.y), 0x7d000_f, 0x50000_f * Cos(ang.y)};
	
	return 1;
}

int DoorBlocker::CleanupResources()
{
	clsn.Disable();
	modelFile.Release();
	clsnFile.Release();
	return 1;
}

int DoorBlocker::Behavior()
{
	if(eventID < 0x20 && Event::GetBit(eventID))
	{
		clsn.Disable();
		if(!gone)
		{
			gone = true;
			camSet = false;
			DisappearPoofDustAt(center);
		}
	}
	else
	{
		UpdateModelTransform();
		IsClsnInRange(0_f, 0_f);
		if(gone)
		{
			gone = false;
			camSet = false;
			PoofDustAt(center);
		}
	}
	
	return 1;
}

int DoorBlocker::Render()
{
	if(eventID >= 0x20 || !Event::GetBit(eventID))
		model.Render(nullptr);
	
	return 1;
}

DoorBlocker::~DoorBlocker() {}