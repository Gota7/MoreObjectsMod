#include "DeleteFile.h"
#include "SM64DS_2.h"
#include "GalaxyShrinkingPlatform.h"
#include "SilverCoin.h"
#include "InvisibleWall.h"
#include "Berry.h"
#include "YoshiRide.h"

namespace {

	//MOMObjectBanks.bin OV0 ID.
	constexpr int MOM_BANKS_OV0_ID = 0x080C;

	//Object IDs.
	constexpr short int BASE_OBJECT_ID = 0x015E;
	constexpr short int GALAXY_SHRINKING_PLATFORM_ID = 0x015E;
	constexpr short int SILVER_COIN_ID = 0x015F;
	constexpr short int INVISIBLE_WALL_ID = 0x0160;
	constexpr short int BERRY_ID = 0x0161;
	constexpr short int YOSHI_RIDE_ID = 0x0162;

	//Assets.
	constexpr short int GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID = 0x0000;
	constexpr short int GALAXY_SHRINKING_PLATFORM_MODEL_ID = 0x0001;
	constexpr short int GALAXY_SHRINKING_PLATFORM_COLLISION_ID = 0x0002;

	constexpr short int SILVER_COIN_MODEL_ID = 0x0003;

	constexpr short int INVISIBLE_WALL_COLLISION_ID = 0x0005;

	constexpr short int BERRY_MODEL_ID = 0x0006;
	constexpr short int BERRY_STEM_MODEL_ID = 0x0007;

	constexpr short int YOSHI_RIDE_ANIM_ID = 0x0008;

	//Get the flag ID for an object.
	int flagId(short int val)
	{
		return val - BASE_OBJECT_ID;
	}

	//If assets should be loaded for an object.
	bool shouldLoadAssets(char* banks, int offset, short int val)
	{
		return banks[flagId(val) / 8 + offset] & (0b1 << (flagId(val) % 8));
	}

	//Modify the object and actor tables.
	void modTable(short int val, unsigned newFunc)
	{
		OBJ_TO_ACTOR_ID_TABLE[val] = val;
		ACTOR_SPAWN_TABLE[val] = newFunc;
	}

}

//Initialize the objects.
void init()
{

	//Load the MOM banks file.
	char* banks = LoadFile(MOM_BANKS_OV0_ID);

	//Get level offset.
	int offset = LEVEL_ID * 0x10;

	//Check for assets to load.
	if (shouldLoadAssets(banks, offset, GALAXY_SHRINKING_PLATFORM_ID)) {
		modTable(GALAXY_SHRINKING_PLATFORM_ID, (unsigned)&GalaxyShrinkingPlatform::spawnData);
		GalaxyShrinkingPlatform::modelFile.Construct(GALAXY_SHRINKING_PLATFORM_MODEL_ID);
    	GalaxyShrinkingPlatform::clsnFile .Construct(GALAXY_SHRINKING_PLATFORM_COLLISION_ID);
    	GalaxyShrinkingPlatform::frameModelFile.Construct(GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID);
	}
	if (shouldLoadAssets(banks, offset, SILVER_COIN_ID)) {
		modTable(SILVER_COIN_ID, (unsigned)&SilverCoin::spawnData);
		SilverCoin::modelFile.Construct(SILVER_COIN_MODEL_ID);
	}
	if (shouldLoadAssets(banks, offset, INVISIBLE_WALL_ID)) {
		modTable(INVISIBLE_WALL_ID, (unsigned)&InvisibleWall::spawnData);
		InvisibleWall::clsnFile.Construct(INVISIBLE_WALL_COLLISION_ID);
	}
	if (shouldLoadAssets(banks, offset, BERRY_ID)) {
		modTable(BERRY_ID, (unsigned)&Berry::spawnData);
		Berry::modelFile.Construct(BERRY_MODEL_ID);
		Berry::stemFile.Construct(BERRY_STEM_MODEL_ID);
	}
	if (shouldLoadAssets(banks, offset, YOSHI_RIDE_ID)) {
		modTable(YOSHI_RIDE_ID, (unsigned)&YoshiRide::spawnData);
		YoshiRide::ridingAnim.Construct(YOSHI_RIDE_ANIM_ID);
	}

	//Unload the MOM banks file.
	_ZdlPv(banks);
	banks = nullptr;

}

//Nothing is needed here.
void cleanup()
{


}