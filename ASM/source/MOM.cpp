#include "DeleteFile.h"
#include "SM64DS_2.h"
#include "GalaxyShrinkingPlatform.h"
#include "SilverCoin.h"
#include "InvisibleWall.h"
#include "Berry.h"
#include "YoshiRide.h"
#include "ObjectLightingModifier.h"
#include "BlankObject.h"
#include "GoombaColored.h"
#include "ToxicLevel.h"
#include "Noteblock.h"
#include "ShyGuy.h"
#include "LaunchStar.h"
#include "Magikoopa.h"
#include "SkyboxRotator.h"
#include "FallingIcicle.h"
#include "GoombaColored2.h"

namespace {

	//Object IDs.
	constexpr short int BASE_OBJECT_ID = 								0x0200;
	constexpr short int GALAXY_SHRINKING_PLATFORM_ID = 					BASE_OBJECT_ID;
	constexpr short int SILVER_COIN_ID = 								BASE_OBJECT_ID + 0x1;
	constexpr short int INVISIBLE_WALL_ID = 							BASE_OBJECT_ID + 0x2;
	constexpr short int BERRY_ID = 										BASE_OBJECT_ID + 0x3;
	constexpr short int YOSHI_RIDE_ID = 								BASE_OBJECT_ID + 0x4;
	constexpr short int OBJECT_LIGHTING_MODIFIER_ID = 					BASE_OBJECT_ID + 0x5;
	constexpr short int TWO_DEE_LEVEL_ID = 								BASE_OBJECT_ID + 0x6;
	constexpr short int TWO_DEE_CAMERA_LIMITER = 						BASE_OBJECT_ID + 0x7;
	constexpr short int COLORED_GOOMBA_SMALL = 							BASE_OBJECT_ID + 0x8;
	constexpr short int COLORED_GOOMBA =	 							BASE_OBJECT_ID + 0x9;
	constexpr short int COLORED_GOOMBA_LARGE = 							BASE_OBJECT_ID + 0xA;
	constexpr short int TOXIC_LEVEL = 									BASE_OBJECT_ID + 0xB;
	constexpr short int NOTEBLOCK = 									BASE_OBJECT_ID + 0xC;
	constexpr short int SHY_GUY = 										BASE_OBJECT_ID + 0xD;
	constexpr short int LAUNCH_STAR = 									BASE_OBJECT_ID + 0xE;
	constexpr short int KAMEK_SHOT = 									BASE_OBJECT_ID + 0xF;
	constexpr short int KAMEK = 										BASE_OBJECT_ID + 0x10;
	constexpr short int KAMELLA = 										BASE_OBJECT_ID + 0x11;
	constexpr short int SKYBOX_ROTATOR = 								BASE_OBJECT_ID + 0x12;
	constexpr short int FALLING_ICICLE = 								BASE_OBJECT_ID + 0x13;
	constexpr short int GRAVITY_MODIFIER = 								BASE_OBJECT_ID + 0x14;
	constexpr short int COLORED_GOOMBA_2_SMALL = 						BASE_OBJECT_ID + 0x15;
	constexpr short int COLORED_GOOMBA_2 =	 							BASE_OBJECT_ID + 0x16;
	constexpr short int COLORED_GOOMBA_2_LARGE = 						BASE_OBJECT_ID + 0x17;
	//constexpr short int CONDITIONAL_CUTSCENE_LOADER_ID = 0x0165;

	//Assets.
	constexpr short int GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID = 		0x0000;
	constexpr short int GALAXY_SHRINKING_PLATFORM_MODEL_ID = 			0x0001;
	constexpr short int GALAXY_SHRINKING_PLATFORM_COLLISION_ID = 		0x0002;

	constexpr short int SILVER_COIN_MODEL_ID = 							0x0003;

	constexpr short int INVISIBLE_WALL_COLLISION_ID = 					0x0005;

	constexpr short int BERRY_MODEL_ID = 								0x0006;
	constexpr short int BERRY_STEM_MODEL_ID = 							0x0007;

	constexpr short int YOSHI_RIDE_ANIM_ID =							0x0008;

	constexpr short int COLORED_GOOMBA_MODEL_ID = 						0x0009;
	constexpr short int COLORED_GOOMBA_ANIM_ID = 						0x000A;

	constexpr short int NOTEBLOCK_MODEL_ID = 							0x000B;
	constexpr short int NOTEBLOCK_COLLISION_ID = 						0x000C;

	constexpr short int SHY_GUY_MODEL_ID = 								0x000D;
	constexpr short int SHY_GUY_WAIT_ANIM_ID = 							0x000E;
	constexpr short int SHY_GUY_WALK_ANIM_ID = 							0x000F;
	constexpr short int SHY_GUY_RUN_ANIM_ID = 							0x0010;
	constexpr short int SHY_GUY_FREEZE_ANIM_ID = 						0x0011;

	constexpr short int LAUNCH_STAR_MODEL_ID =							0x0012;
	constexpr short int LAUNCH_STAR_WAIT_ANIM_ID =						0x0013;
	constexpr short int LAUNCH_STAR_LAUNCH_ANIM_ID =					0x0014;

	constexpr short int KAMEK_MODEL_ID = 								0x0015;
	constexpr short int KAMELLA_MODEL_ID = 								0x0016;
	constexpr short int KAMEK_APPEAR_ANIM_ID = 							0x0017;
	constexpr short int KAMEK_WAVE_ANIM_ID = 							0x0018;
	constexpr short int KAMEK_SHOOT_ANIM_ID = 							0x0019;
	constexpr short int KAMEK_POOF_ANIM_ID = 							0x001A;
	constexpr short int KAMEK_WAIT_ANIM_ID = 							0x001B;
	constexpr short int KAMEK_HURT_ANIM_ID = 							0x001C;
	constexpr short int KAMEK_DEFEAT_ANIM_ID = 							0x001D;
	constexpr short int KAMEK_MAGIC_PARTICLE_ID = 						0x001E;

	constexpr short int ICICLE_MODEL_ID = 								0x001F;

	constexpr short int COLORED_GOOMBA_2_MODEL_ID = 					0x0020;
	constexpr short int COLORED_GOOMBA_2_ANIM_ID = 						0x0021;

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

	//Shrinking platforms.
	modTable(GALAXY_SHRINKING_PLATFORM_ID, (unsigned)&GalaxyShrinkingPlatform::spawnData);
	GalaxyShrinkingPlatform::modelFile.Construct(GALAXY_SHRINKING_PLATFORM_MODEL_ID);
    GalaxyShrinkingPlatform::clsnFile .Construct(GALAXY_SHRINKING_PLATFORM_COLLISION_ID);
    GalaxyShrinkingPlatform::frameModelFile.Construct(GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID);

    //Silver coins.
	modTable(SILVER_COIN_ID, (unsigned)&SilverCoin::spawnData);
	SilverCoin::modelFile.Construct(SILVER_COIN_MODEL_ID);

	//Invisible walls.
	modTable(INVISIBLE_WALL_ID, (unsigned)&InvisibleWall::spawnData);
	InvisibleWall::clsnFile.Construct(INVISIBLE_WALL_COLLISION_ID);

	//Berries.
	modTable(BERRY_ID, (unsigned)&Berry::spawnData);
	Berry::modelFile.Construct(BERRY_MODEL_ID);
	Berry::stemFile.Construct(BERRY_STEM_MODEL_ID);

	//Rideable yoshis.
	modTable(YOSHI_RIDE_ID, (unsigned)&YoshiRide::spawnData);
	YoshiRide::ridingAnim.Construct(YOSHI_RIDE_ANIM_ID);

	//Object lighting modifier.
	modTable(OBJECT_LIGHTING_MODIFIER_ID, (unsigned)&ObjectLightingModifier::spawnData);

	//2D level.
	modTable(TWO_DEE_LEVEL_ID, (unsigned)&BlankObject::spawnData);
	modTable(TWO_DEE_CAMERA_LIMITER, (unsigned)&BlankObject::spawnData);

	//Colored goombas.
	modTable(COLORED_GOOMBA_SMALL, (unsigned)&Goomba::spawnDataSmall);
	modTable(COLORED_GOOMBA, (unsigned)&Goomba::spawnDataNormal);
	modTable(COLORED_GOOMBA_LARGE, (unsigned)&Goomba::spawnDataBig);
	Goomba::modelFile.Construct(COLORED_GOOMBA_MODEL_ID);
	Goomba::texSeqFile.Construct(COLORED_GOOMBA_ANIM_ID);
	Goomba::animFiles[0].Construct(0x0388);
	Goomba::animFiles[1].Construct(0x0389);
	Goomba::animFiles[2].Construct(0x038a);
	Goomba::animFiles[3].Construct(0x038b);
	Goomba::animFiles[4].Construct(0x038c);
	Goomba::animFiles[5].Construct(0x038d);

	//Toxic level.
	modTable(TOXIC_LEVEL, (unsigned)&ToxicLevel::spawnData);

	//Noteblocks.
	modTable(NOTEBLOCK, (unsigned)&Noteblock::spawnData);
	Noteblock::modelFile.Construct(NOTEBLOCK_MODEL_ID);
	Noteblock::clsnFile.Construct(NOTEBLOCK_COLLISION_ID);

	//Shy guys.
	modTable(SHY_GUY, (unsigned)&ShyGuy::spawnData);
	ShyGuy::modelFile.Construct(SHY_GUY_MODEL_ID);
	ShyGuy::animFiles[0].Construct(SHY_GUY_WAIT_ANIM_ID);
	ShyGuy::animFiles[1].Construct(SHY_GUY_WALK_ANIM_ID);
	ShyGuy::animFiles[2].Construct(SHY_GUY_RUN_ANIM_ID);
	ShyGuy::animFiles[3].Construct(SHY_GUY_FREEZE_ANIM_ID);

	//Launch stars.
	modTable(LAUNCH_STAR, (unsigned)&LaunchStar::spawnData);
	LaunchStar::modelFile.Construct(LAUNCH_STAR_MODEL_ID);
	LaunchStar::animFiles[0].Construct(LAUNCH_STAR_WAIT_ANIM_ID);
	LaunchStar::animFiles[1].Construct(LAUNCH_STAR_LAUNCH_ANIM_ID);

	//Kamek.
	modTable(KAMEK_SHOT, (unsigned)&Magikoopa::Shot::spawnData);
	modTable(KAMEK, (unsigned)&Magikoopa::spawnData);
	modTable(KAMELLA, (unsigned)&Magikoopa::bossSpawnData);
	Magikoopa::modelFiles[0].Construct(KAMEK_MODEL_ID);
	Magikoopa::modelFiles[1].Construct(KAMELLA_MODEL_ID);
	Magikoopa::animFiles[0].Construct(KAMEK_APPEAR_ANIM_ID);
	Magikoopa::animFiles[1].Construct(KAMEK_WAVE_ANIM_ID);
	Magikoopa::animFiles[2].Construct(KAMEK_SHOOT_ANIM_ID);
	Magikoopa::animFiles[3].Construct(KAMEK_POOF_ANIM_ID);
	Magikoopa::animFiles[4].Construct(KAMEK_WAIT_ANIM_ID);
	Magikoopa::animFiles[5].Construct(KAMEK_HURT_ANIM_ID);
	Magikoopa::animFiles[6].Construct(KAMEK_DEFEAT_ANIM_ID);

	//Skybox rotator.
	modTable(SKYBOX_ROTATOR, (unsigned)&SkyboxRotator::spawnData);

	//Falling icicle.
	modTable(FALLING_ICICLE, (unsigned)&FallingIcicle::spawnData);
	FallingIcicle::modelFile.Construct(ICICLE_MODEL_ID);

	//Gravity modifer.
	modTable(GRAVITY_MODIFIER, (unsigned)&BlankObject::spawnData);

	//Colored goombas 2.
	modTable(COLORED_GOOMBA_2_SMALL, (unsigned)&Goomba2::spawnDataSmall);
	modTable(COLORED_GOOMBA_2, (unsigned)&Goomba2::spawnDataNormal);
	modTable(COLORED_GOOMBA_2_LARGE, (unsigned)&Goomba2::spawnDataBig);
	Goomba2::modelFile.Construct(COLORED_GOOMBA_2_MODEL_ID);
	Goomba2::texSeqFile.Construct(COLORED_GOOMBA_2_ANIM_ID);
	Goomba2::animFiles[0].Construct(0x0388);
	Goomba2::animFiles[1].Construct(0x0389);
	Goomba2::animFiles[2].Construct(0x038a);
	Goomba2::animFiles[3].Construct(0x038b);
	Goomba2::animFiles[4].Construct(0x038c);
	Goomba2::animFiles[5].Construct(0x038d);
}