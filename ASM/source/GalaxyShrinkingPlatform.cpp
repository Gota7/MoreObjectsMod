#include "GalaxyShrinkingPlatform.h"

namespace
{
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x0, 0x6, 0, 0, 0, 0, 0xff)
        }
	};
}

SharedFilePtr GalaxyShrinkingPlatform::modelFile;
SharedFilePtr GalaxyShrinkingPlatform::clsnFile;
SharedFilePtr GalaxyShrinkingPlatform::frameModelFile;

SpawnInfo<GalaxyShrinkingPlatform> GalaxyShrinkingPlatform::spawnData =
{
	&GalaxyShrinkingPlatform::Spawn,
	0x0030,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

GalaxyShrinkingPlatform* GalaxyShrinkingPlatform::Spawn()
{
	return new GalaxyShrinkingPlatform;
}


void GalaxyShrinkingPlatform::OnFloorAfterClsn(MeshCollider& clsn, Actor* clsnActor, Actor* otherActor) {

	static_cast<GalaxyShrinkingPlatform*>(clsnActor)->shrinkActivated = true;
	//static_cast<GalaxyShrinkingPlatform*>(clsnActor)->playSound = true;
}


void GalaxyShrinkingPlatform::Shrink() {

	if (skl.x == 0x1000_f) {
		Sound::Play(4, 1, camSpacePos);
	}

	if (skl.x > 0_f) {

		skl.x = skl.x - 100_f;
		skl.z = skl.z - 100_f;

	} else {

		skl.x = 0_f;
		skl.z = 0_f;
		clsn.Disable();

	}

}

void GalaxyShrinkingPlatform::UnShrink() {

	skl.x = 0x1000_f;
	skl.z = 0x1000_f;
	clsn.Enable(this);
	shrinkActivated = false;

}

void GalaxyShrinkingPlatform::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;

	frameModel.mat4x3.ThisFromRotationY(ang.y);
	frameModel.mat4x3.r0c3 = pos.x >> 3;
	frameModel.mat4x3.r1c3 = pos.y >> 3;
	frameModel.mat4x3.r2c3 = pos.z >> 3;
}


int GalaxyShrinkingPlatform::InitResources()
{

	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);

	Model::LoadFile(frameModelFile);
	frameModel.SetFile(frameModelFile.filePtr, 1, -1);

	char* clsnF = MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnF, clsnNextMat, 0x1000_f, ang.y, clpsBlock);

	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;	
	clsn.afterClsnCallback = &OnFloorAfterClsn;

	UpdateModelTransform();
	UpdateClsnPosAndRot();
	
	return 1;
}

int GalaxyShrinkingPlatform::CleanupResources()
{
	clsn.Disable();
	modelFile.Release();
	clsnFile.Release();
	frameModelFile.Release();
	return 1;
}

int GalaxyShrinkingPlatform::Behavior()
{
	UpdateModelTransform();
	if(IsClsnInRange(0_f, 0_f))
	{
		//Sound::Play(2, 17, camSpacePos);
		UpdateClsnPosAndRot();

		if (shrinkActivated) {
			Shrink();
		}

		/*
		if (playSound) {
			//Sound::Play(5, 0, camSpacePos);
			//Sound::PlayBank3(2, camSpacePos);
			playSound = false;
		}*/

	} else {
		if (shrinkActivated) { UnShrink(); }
	}

	UpdateModelPosAndRotY();
	
	return 1;
}

int GalaxyShrinkingPlatform::Render()
{
	if (skl.x != 0_f) {
		model.Render(&skl);
	}
	frameModel.Render(nullptr);
	return 1;
}

GalaxyShrinkingPlatform::~GalaxyShrinkingPlatform() {}