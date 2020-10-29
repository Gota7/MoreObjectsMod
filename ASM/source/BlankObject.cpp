#include "BlankObject.h"

SpawnInfo<BlankObject> BlankObject::spawnData =
{
	&BlankObject::Spawn,
	0x0000,
	0x0100,
	0x00000003,
	0x00064000_f,
	0x000c8000_f,
	0x01000000_f,
	0x01000000_f
};

BlankObject* BlankObject::Spawn()
{
	return new BlankObject;
}

int BlankObject::InitResources()
{
	return 1;
}

int BlankObject::CleanupResources()
{
	return 1;
}

int BlankObject::Behavior()
{
	Destroy();
	return 1;
}

int BlankObject::Render()
{
	return 1;
}

BlankObject::~BlankObject() {}
