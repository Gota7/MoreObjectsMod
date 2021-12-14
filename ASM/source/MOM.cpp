#include "include/DeleteFile.h"
#include "include/SM64DS_2.h"
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
#include "NPC.h"
#include "ColoredPipe.h"
#include "CharacterBlock.h"
#include "TreeShadow.h"
#include "SaveBlock.h"
#include "StarChip.h"
#include "DoorBlocker.h"
#include "ColoredCoin.h"
#include "BlueIceBlock.h"
#include "MegaBlock.h"
#include "CutsceneLoader.h"
#include "CustomBlock.h"
#include "TreasureChest.h"
#include "Thwomp.h"
/*#include "ToxBox.h"
#include "ToxBoxFlower.h"*/

namespace {

	//Object IDs.
	
	//Object IDs.
	enum ObjectIDs:short
	{
		GALAXY_SHRINKING_PLATFORM = 0x0200,
		SILVER_COIN,
		INVISIBLE_WALL,
		BERRY,
		YOSHI_RIDE,
		OBJECT_LIGHTING_MODIFIER,
		TWO_DEE_LEVEL,
		TWO_DEE_CAMERA_LIMITER,
		COLORED_GOOMBA_SMALL,
		COLORED_GOOMBA,
		COLORED_GOOMBA_LARGE,
		TOXIC_LEVEL,
		NOTEBLOCK,
		SHY_GUY,
		LAUNCH_STAR,
		KAMEK_SHOT,
		KAMEK, //210
		KAMELLA,
		SKYBOX_ROTATOR,
		FALLING_ICICLE,
		GRAVITY_MODIFIER,
		COLORED_GOOMBA_2_SMALL,
		COLORED_GOOMBA_2,
		COLORED_GOOMBA_2_LARGE,
		YOSHI_NPC,
		COLORED_PIPE,
		CHARACTER_BLOCK,
		TREE_SHADOW,
		SAVE_BLOCK,
		STAR_CHIP,
		DOOR_BLOCKER,
		COLORED_COIN,
		COLORED_TOAD_NPC, //220
		PEACH_NPC,
		BLUE_ICE_BLOCK,
		MEGA_BLOCK,
		CUTSCENE_LOADER,
		CUSTOM_BLOCK,
		TREASURE_CHEST,
		THWOMP,
		MAGMA_THWOMP,
		GIGA_THWOMP/*,
		TOX_BOX,
		PLAYER_TOX_BOX,
		TOX_BOX_FLOWER*/
	};
	
	//constexpr short int CONDITIONAL_CUTSCENE_LOADER_ID = 0x0165;

	//Assets.
	enum AssetIDs:short
	{
		GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID, //0
		GALAXY_SHRINKING_PLATFORM_MODEL_ID,
		GALAXY_SHRINKING_PLATFORM_COLLISION_ID,
		
		SILVER_COIN_MODEL_ID,
		
		INVISIBLE_WALL_MODEL_ID,
		INVISIBLE_WALL_COLLISION_ID,
		
		BERRY_MODEL_ID,
		BERRY_STEM_MODEL_ID,
		
		YOSHI_RIDE_ANIM_ID,
		
		COLORED_GOOMBA_MODEL_ID,
		COLORED_GOOMBA_ANIM_ID,
		
		NOTEBLOCK_MODEL_ID,
		NOTEBLOCK_COLLISION_ID,
		
		SHY_GUY_MODEL_ID,
		SHY_GUY_WAIT_ANIM_ID,
		SHY_GUY_WALK_ANIM_ID,
		SHY_GUY_RUN_ANIM_ID, //10
		SHY_GUY_FREEZE_ANIM_ID,
		
		LAUNCH_STAR_MODEL_ID,
		LAUNCH_STAR_WAIT_ANIM_ID,
		LAUNCH_STAR_LAUNCH_ANIM_ID,
		
		KAMEK_MODEL_ID,
		KAMELLA_MODEL_ID,
		KAMEK_APPEAR_ANIM_ID,
		KAMEK_WAVE_ANIM_ID,
		KAMEK_SHOOT_ANIM_ID,
		KAMEK_POOF_ANIM_ID,
		KAMEK_WAIT_ANIM_ID,
		KAMEK_HURT_ANIM_ID,
		KAMEK_DEFEAT_ANIM_ID,
		KAMEK_MAGIC_MODEL_ID,
		KAMEK_MAGIC_ANIM_ID,
		
		ICICLE_MODEL_ID, //20
		
		YOSHI_NPC_MODEL_ID,
		YOSHI_NPC_ANIM_ID,
		
		COLORED_PIPE_MODEL_ID,
		COLORED_PIPE_COLLISION_ID,
		
		MARIO_BLOCK_MODEL_ID,
		LUIGI_BLOCK_MODEL_ID,
		WARIO_BLOCK_MODEL_ID,
		YOSHI_BLOCK_MODEL_ID,
		CHARACTER_BLOCK_COLLSION_ID,
		CHARACTER_BLOCK_ANIMATION_ID,
		MARIO_BLOCK_TRANSPARENT_MODEL_ID,
		LUIGI_BLOCK_TRANSPARENT_MODEL_ID,
		WARIO_BLOCK_TRANSPARENT_MODEL_ID,
		YOSHI_BLOCK_TRANSPARENT_MODEL_ID,
		CHARACTER_BLOCK_TRANSPARENT_ANIMATION_ID,
		
		SAVE_BLOCK_MODEL_ID, //30
		SAVE_BLOCK_ANIM_ID,
		SAVE_BLOCK_COLLISION_ID,
		
		STAR_CHIP_MODEL_ID,
		
		DOOR_BLOCKER_MODEL_ID,
		DOOR_BLOCKER_COLLISION_ID,
		
		COLORED_COIN_MODEL_ID,
		
		COLORED_TOAD_NPC_MODEL_ID,
		COLORED_TOAD_NPC_TEXANIM_ID,
		COLORED_TOAD_NPC_ANIM_1_ID,
		COLORED_TOAD_NPC_ANIM_2_ID,
		
		PEACH_NPC_MODEL_ID,
		PEACH_NPC_ANIM_1_ID,
		PEACH_NPC_ANIM_2_ID,
		
		BLUE_ICE_BLOCK_MODEL_ID,
		BLUE_ICE_BLOCK_COLLISION_ID,
		
		MEGA_BLOCK_MODEL_ID, //40
		MEGA_BLOCK_COLLISION_ID,
		
		YELLOW_BLOCK_MODEL_ID,
		RED_BLOCK_MODEL_ID,
		RED_BLOCK_TRANSPARENT_MODEL_ID,
		BLOCK_COLLSION_ID,
		RED_BLOCK_TRANSPARENT_ANIMATION_ID,
		
		TREASURE_CHEST_MODEL_ID,
		TREASURE_CHEST_ANIM_ID,
		
		THWOMP_MODEL_ID,
		THWOMP_TEXANIM_ID,
		THWOMP_COLLSION_ID,
		MAGMA_THWOMP_MODEL_ID,
		GIGA_THWOMP_MODEL_ID,
		GIGA_THWOMP_COLLSION_ID,
		
		TOX_BOX_MODEL_ID,
		TOX_BOX_COLLSION_ID, //50
		TOX_BOX_SWITCH_MODEL_ID,
		TOX_BOX_SWITCH_COLLSION_ID,
		TOX_BOX_PLAYER_MODEL_ID,
		TOX_BOX_PLAYER_TEXANIM_ID,
		TOX_BOX_FLOWER_MODEL_ID,
		
		COLORED_GOOMBA_MODEL_ID_2
	};

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
	modTable(GALAXY_SHRINKING_PLATFORM, (unsigned)&GalaxyShrinkingPlatform::spawnData);
	GalaxyShrinkingPlatform::modelFile.Construct(GALAXY_SHRINKING_PLATFORM_MODEL_ID);
    GalaxyShrinkingPlatform::clsnFile .Construct(GALAXY_SHRINKING_PLATFORM_COLLISION_ID);
    GalaxyShrinkingPlatform::frameModelFile.Construct(GALAXY_SHRINKING_PLATFORM_FRAME_MODEL_ID);

    //Silver coins.
	modTable(SILVER_COIN, (unsigned)&SilverCoin::spawnData);
	SilverCoin::modelFile.Construct(SILVER_COIN_MODEL_ID);

	//Invisible walls.
	modTable(INVISIBLE_WALL, (unsigned)&InvisibleWall::spawnData);
	InvisibleWall::clsnFile.Construct(INVISIBLE_WALL_COLLISION_ID);

	//Berries.
	modTable(BERRY, (unsigned)&Berry::spawnData);
	Berry::modelFile.Construct(BERRY_MODEL_ID);
	Berry::stemFile.Construct(BERRY_STEM_MODEL_ID);

	//Rideable yoshis.
	modTable(YOSHI_RIDE, (unsigned)&YoshiRide::spawnData);
	YoshiRide::ridingAnim.Construct(YOSHI_RIDE_ANIM_ID);

	//Object lighting modifier.
	modTable(OBJECT_LIGHTING_MODIFIER, (unsigned)&ObjectLightingModifier::spawnData);

	//2D level.
	modTable(TWO_DEE_LEVEL, (unsigned)&BlankObject::spawnData);
	modTable(TWO_DEE_CAMERA_LIMITER, (unsigned)&BlankObject::spawnData);

	//Colored goombas.
	modTable(COLORED_GOOMBA_SMALL, (unsigned)&Goomba::spawnDataSmall);
	modTable(COLORED_GOOMBA, (unsigned)&Goomba::spawnDataNormal);
	modTable(COLORED_GOOMBA_LARGE, (unsigned)&Goomba::spawnDataBig);
	Goomba::modelFiles[0].Construct(COLORED_GOOMBA_MODEL_ID);
	Goomba::modelFiles[1].Construct(COLORED_GOOMBA_MODEL_ID_2);
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
	Magikoopa::Shot::magicModelFile.Construct(KAMEK_MAGIC_MODEL_ID);
	Magikoopa::Shot::magicTexSeqFile.Construct(KAMEK_MAGIC_ANIM_ID);

	//Skybox rotator.
	modTable(SKYBOX_ROTATOR, (unsigned)&SkyboxRotator::spawnData);

	//Falling icicle.
	modTable(FALLING_ICICLE, (unsigned)&FallingIcicle::spawnData);
	FallingIcicle::modelFile.Construct(ICICLE_MODEL_ID);

	//Gravity modifer.
	modTable(GRAVITY_MODIFIER, (unsigned)&BlankObject::spawnData);

	//Colored goombas 2.
	modTable(COLORED_GOOMBA_2_SMALL, (unsigned)&Goomba::spawnDataSmall);
	modTable(COLORED_GOOMBA_2, (unsigned)&Goomba::spawnDataNormal);
	modTable(COLORED_GOOMBA_2_LARGE, (unsigned)&Goomba::spawnDataBig);
	
	//NPCs.
	modTable(YOSHI_NPC, (unsigned)&NPC::spawnData);
	modTable(COLORED_TOAD_NPC, (unsigned)&NPC::spawnData);
	modTable(PEACH_NPC, (unsigned)&NPC::spawnData);
	NPC::modelFiles[0].Construct(YOSHI_NPC_MODEL_ID);
	NPC::modelFiles[1].Construct(COLORED_TOAD_NPC_MODEL_ID);
	NPC::modelFiles[2].Construct(PEACH_NPC_MODEL_ID);
	NPC::animFiles[0].Construct(YOSHI_NPC_ANIM_ID);
	NPC::animFiles[1].Construct(YOSHI_NPC_ANIM_ID);
	NPC::animFiles[2].Construct(COLORED_TOAD_NPC_ANIM_1_ID);
	NPC::animFiles[3].Construct(COLORED_TOAD_NPC_ANIM_2_ID);
	NPC::animFiles[4].Construct(PEACH_NPC_ANIM_1_ID);
	NPC::animFiles[5].Construct(PEACH_NPC_ANIM_2_ID);
	NPC::texSeqFile.Construct(COLORED_TOAD_NPC_TEXANIM_ID);

	//Colored Pipes.
	modTable(COLORED_PIPE, (unsigned)&ColoredPipe::spawnData);
	ColoredPipe::modelFile.Construct(COLORED_PIPE_MODEL_ID);
	ColoredPipe::clsnFile.Construct(COLORED_PIPE_COLLISION_ID);

	//Character Blocks.
	modTable(CHARACTER_BLOCK, (unsigned)&CharacterBlock::spawnData);
	CharacterBlock::modelFiles[0].Construct(MARIO_BLOCK_MODEL_ID);
	CharacterBlock::modelFiles[1].Construct(LUIGI_BLOCK_MODEL_ID);
	CharacterBlock::modelFiles[2].Construct(WARIO_BLOCK_MODEL_ID);
	CharacterBlock::modelFiles[3].Construct(YOSHI_BLOCK_MODEL_ID);
	CharacterBlock::modelFilesTrans[0].Construct(MARIO_BLOCK_TRANSPARENT_MODEL_ID);
	CharacterBlock::modelFilesTrans[1].Construct(LUIGI_BLOCK_TRANSPARENT_MODEL_ID);
	CharacterBlock::modelFilesTrans[2].Construct(WARIO_BLOCK_TRANSPARENT_MODEL_ID);
	CharacterBlock::modelFilesTrans[3].Construct(YOSHI_BLOCK_TRANSPARENT_MODEL_ID);
	CharacterBlock::clsnFile.Construct(CHARACTER_BLOCK_COLLSION_ID);
	CharacterBlock::animFiles[0].Construct(CHARACTER_BLOCK_ANIMATION_ID);
	CharacterBlock::animFiles[1].Construct(CHARACTER_BLOCK_TRANSPARENT_ANIMATION_ID);

	//Tree Shadows.
	modTable(TREE_SHADOW, (unsigned)&TreeShadow::spawnData);
	TreeShadow::modelFile.Construct(WARIO_BLOCK_TRANSPARENT_MODEL_ID);
	
	//Save Blocks.
	modTable(SAVE_BLOCK, (unsigned)&SaveBlock::spawnData);
	SaveBlock::modelFile.Construct(SAVE_BLOCK_MODEL_ID);
	SaveBlock::texSeqFile.Construct(SAVE_BLOCK_ANIM_ID);
	SaveBlock::clsnFile.Construct(SAVE_BLOCK_COLLISION_ID);
	
	//Star Chips.
	modTable(STAR_CHIP, (unsigned)&StarChip::spawnData);
	StarChip::modelFile.Construct(STAR_CHIP_MODEL_ID);
	
	//Save Blocks.
	modTable(DOOR_BLOCKER, (unsigned)&DoorBlocker::spawnData);
	DoorBlocker::modelFile.Construct(DOOR_BLOCKER_MODEL_ID);
	DoorBlocker::clsnFile.Construct(DOOR_BLOCKER_COLLISION_ID);

    //Colored Coins.
	modTable(COLORED_COIN, (unsigned)&ColoredCoin::spawnData);
	ColoredCoin::modelFile.Construct(COLORED_COIN_MODEL_ID);
	
	//Blue Ice Blocks.
	modTable(BLUE_ICE_BLOCK, (unsigned)&BlueIceBlock::spawnData);
	BlueIceBlock::modelFile.Construct(BLUE_ICE_BLOCK_MODEL_ID);
	BlueIceBlock::clsnFile.Construct(BLUE_ICE_BLOCK_COLLISION_ID);
	
	//Mega Blocks.
	modTable(MEGA_BLOCK, (unsigned)&MegaBlock::spawnData);
	MegaBlock::modelFile.Construct(MEGA_BLOCK_MODEL_ID);
	MegaBlock::clsnFile.Construct(MEGA_BLOCK_COLLISION_ID);
	
	//Cutscene Loader.
	modTable(CUTSCENE_LOADER, (unsigned)&CutsceneLoader::spawnData);
	
	//Custom Blocks.
	modTable(CUSTOM_BLOCK, (unsigned)&CustomBlock::spawnData);
	CustomBlock::modelFiles[0].Construct(YELLOW_BLOCK_MODEL_ID);
	CustomBlock::modelFiles[1].Construct(RED_BLOCK_MODEL_ID);
	CustomBlock::modelFiles[2].Construct(RED_BLOCK_TRANSPARENT_MODEL_ID);
	CustomBlock::clsnFile.Construct(BLOCK_COLLSION_ID);
	CustomBlock::animFiles[0].Construct(CHARACTER_BLOCK_ANIMATION_ID);
	CustomBlock::animFiles[1].Construct(RED_BLOCK_TRANSPARENT_ANIMATION_ID);
	
	//Treasure chest
	modTable(TREASURE_CHEST, (unsigned)&TreasureChest::spawnData);
	TreasureChest::modelFile.Construct(TREASURE_CHEST_MODEL_ID);
	TreasureChest::animFiles[0].Construct(TREASURE_CHEST_ANIM_ID);
	
	//Thwomp, Magma Thwomp and Giga Thwomp
	modTable(THWOMP, (unsigned)&Thwomp::spawnData);
	modTable(MAGMA_THWOMP, (unsigned)&Thwomp::spawnData);
	modTable(GIGA_THWOMP, (unsigned)&Thwomp::spawnData);
	Thwomp::modelFiles[0].Construct(THWOMP_MODEL_ID);
	Thwomp::modelFiles[1].Construct(MAGMA_THWOMP_MODEL_ID);
	Thwomp::modelFiles[2].Construct(GIGA_THWOMP_MODEL_ID);
	Thwomp::texSeqFile.Construct(THWOMP_TEXANIM_ID);
	Thwomp::clsnFile.Construct(THWOMP_COLLSION_ID);
	
	/*//Tox Box
	modTable(TOX_BOX, (unsigned)&ToxBox::spawnData);
	ToxBox::modelFiles[0].Construct(TOX_BOX_MODEL_ID);
	ToxBox::clsnFiles [0].Construct(TOX_BOX_COLLSION_ID);
	ToxBox::modelFiles[1].Construct(TOX_BOX_SWITCH_MODEL_ID);
	ToxBox::clsnFiles [1].Construct(TOX_BOX_SWITCH_COLLSION_ID);
	
	//Player Tox Box
	modTable(PLAYER_TOX_BOX, (unsigned)&PlayerToxBox::spawnData);
	PlayerToxBox::modelFile .Construct(TOX_BOX_PLAYER_MODEL_ID);
	PlayerToxBox::texSeqFile.Construct(TOX_BOX_PLAYER_TEXANIM_ID);
	
	//Tox Box Flower
	modTable(TOX_BOX_FLOWER, (unsigned)&ToxBoxFlower::spawnData);
	ToxBoxFlower::modelFile.Construct(TOX_BOX_FLOWER_MODEL_ID);*/
}