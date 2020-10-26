#include "SkyboxRotator.h"

namespace {
	uint16_t rotX = 0;
	uint16_t rotY = 0;
	uint16_t rotZ = 0;
}

SpawnInfo<SkyboxRotator> SkyboxRotator::spawnData =
{
	&SkyboxRotator::Spawn,
	0x0004,
	0x0007,
	0x00000000,
	0x00000000_f,
	0x7fffffff_f,
	0x7fffffff_f,
	0x7fffffff_f
};

SkyboxRotator* SkyboxRotator::Spawn()
{
	return new SkyboxRotator;
}

int SkyboxRotator::InitResources()
{
	return 1;
}

int SkyboxRotator::CleanupResources()
{
	return 1;
}

int SkyboxRotator::Behavior()
{

	Stage* currStage = static_cast<Stage*>(ROOT_ACTOR_BASE);
	if (currStage != nullptr) {
		currStage->skyBox->mat4x3 = currStage->skyBox->mat4x3.FromRotationXYZExt(rotX, rotY, rotZ);
		rotX += param1;
		rotY += ang.x;
		rotZ += ang.z;
	}

	return 1;
}

int SkyboxRotator::Render()
{
	return 1;
}

SkyboxRotator::~SkyboxRotator() {}
