#include "ObjectLightingModifier.h"

namespace
{
	
}

SpawnInfo<ObjectLightingModifier> ObjectLightingModifier::spawnData =
{
	&ObjectLightingModifier::Spawn,
	0x0000,
	0x0100,
	0x00000002,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

ObjectLightingModifier* ObjectLightingModifier::Spawn()
{
	return new ObjectLightingModifier;
}


int ObjectLightingModifier::InitResources()
{
	GXFIFO::SetLightColor((uint8_t)((param1 & 0xFF00) >> 8), (uint8_t)(param1 & 0xFF), (uint8_t)((ang.x & 0xFF00) >> 8), (uint8_t)(ang.x & 0xFF));
	return 1;
}

int ObjectLightingModifier::CleanupResources()
{
	GXFIFO::SetLightColor(0, 0xFF, 0xFF, 0xFF);
	return 1;
}

int ObjectLightingModifier::Behavior()
{
	return 1;
}

int ObjectLightingModifier::Render()
{
	return 1;
}

ObjectLightingModifier::~ObjectLightingModifier() {}