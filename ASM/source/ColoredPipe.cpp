#include "ColoredPipe.h"

namespace
{
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x0, 0x0, 0x3f, 0x0, 0x1, 0x0, 0, 0, 0, 0xff)
        }
	};
}

SharedFilePtr ColoredPipe::modelFile;
SharedFilePtr ColoredPipe::clsnFile;

SpawnInfo<ColoredPipe> ColoredPipe::spawnData =
{
	&ColoredPipe::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

ColoredPipe* ColoredPipe::Spawn()
{
	return new ColoredPipe;
}

void ColoredPipe::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	
	//Pipe rotation for parameters 2 & 3
	model.mat4x3.ApplyInPlaceToRotationX(ang.x);
	model.mat4x3.ApplyInPlaceToRotationZ(ang.z);
}

int ColoredPipe::InitResources()
{
	
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);
	model.data.materials[0].difAmb = 
		model.data.materials[1].difAmb = (param1 & 0x7fff) << 16 | 0x8000;

	char* clsnF = MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnF, clsnNextMat, 0x0163_f, ang.y, clpsBlock); //0x0175_f
	
	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;

	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	return 1;
}

int ColoredPipe::CleanupResources()
{
	clsn.Disable();
	modelFile.Release();
	clsnFile.Release();
	return 1;
}

int ColoredPipe::Behavior()
{
	UpdateModelTransform();
	if(IsClsnInRange(0_f, 0_f))
	{
		Player* player = ClosestPlayer();
		
		if(!player)
			return 1;
		
		//If the player jumped into the pipe
		/*if (player->pos.x < pos.x + 0x50000_f && player->pos.x > pos.x - 0x50000_f && player->pos.z < pos.z + 0x50000_f && player->pos.z > pos.z - 0x50000_f && player->pos.y < pos.y + 0x50000_f && player->pos.y > pos.y + 0x30000_f)
		{
			Sound::Play(2, 71, camSpacePos);
		}*/
		
		UpdateClsnPosAndRot();
	}
	
	return 1;
}

int ColoredPipe::Render()
{
	model.Render(nullptr);
	return 1;
}

ColoredPipe::~ColoredPipe() {}
