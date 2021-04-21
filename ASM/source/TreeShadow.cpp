#include "TreeShadow.h"

namespace {}

SharedFilePtr TreeShadow::modelFile;

SpawnInfo<TreeShadow> TreeShadow::spawnData =
{
	&TreeShadow::Spawn,
	0x0034,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

TreeShadow* TreeShadow::Spawn()
{
	return new TreeShadow;
}

void TreeShadow::UpdateModelTransform()
{
	model.mat4x3.ThisFromRotationY(ang.y);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
	DropShadowRadHeight(shadow, model.mat4x3, 0x150000_f, 0x137000_f, 0xc);
}

int TreeShadow::InitResources()
{
	opacity = param1 & 0xf;
	
	Model::LoadFile(modelFile);
	model.SetFile(modelFile.filePtr, 1, -1);
	
	shadow.InitCylinder();
	
	UpdateModelTransform();
	
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	return 1;
}

int TreeShadow::CleanupResources()
{
	modelFile.Release();
	return 1;
}

int TreeShadow::Behavior()
{
	UpdateModelTransform();
	return 1;
}

int TreeShadow::Render()
{
	//model.Render(nullptr);
	return 1;
}

TreeShadow::~TreeShadow() {}
