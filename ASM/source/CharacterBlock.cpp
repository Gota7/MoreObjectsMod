#include "CharacterBlock.h"

namespace
{
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
        	CLPS(0x00, 0, 0x3f, 0x0, 0x6, 0x00, 1, 0, 0, 0xff)
        }
	};
}

SharedFilePtr CharacterBlock::modelFiles[4];
SharedFilePtr CharacterBlock::modelFilesTrans[4];
SharedFilePtr CharacterBlock::clsnFile;
SharedFilePtr CharacterBlock::animFiles[2];

SpawnInfo<CharacterBlock> CharacterBlock::spawnData =
{
	&CharacterBlock::Spawn,
	0x0034,
	0x0100,
	0x00000002,
	0x00000000_f,
	0x005dc000_f,
	0x01000000_f,
	0x01000000_f
};

CharacterBlock* CharacterBlock::Spawn()
{
	return new CharacterBlock;
}

bool CharacterBlock::CheckUnlock()
{
	//Check if the player has unlocked the character if they need to be unlocked
	if (needsUnlock && blockType != startingCharacter)
	{
		//First check if the player is Yoshi (code only gets reached if the starting character has been changed) and then check if the flag for unlocking the starting character = 0
		if (blockType == 3 && (SAVE_DATA.characterState & 1 << startingCharacter) == 0)
			return false;
		//If the player is not Yoshi check if the flag for the blocktype character = 0
		else if ((SAVE_DATA.characterState & 1 << blockType) == 0)
			return false;
	}
	
	return true;
}

void CharacterBlock::JumpedUnderBlock()
{
	//Check if the player is the same character as the blocktype
	if (SAVE_DATA.currentCharacter == blockType)
		return;
	
	//Change the character into the character of the blocktype
	uint8_t prev_health = PLAYER_HEALTH;
	PLAYER_ARR[0]->SetRealCharacter(blockType);
	SAVE_DATA.currentCharacter = blockType;
	
	if (!healPlayer)
		PLAYER_HEALTH = prev_health;
	
	//Play the sound effect of the character saying their name
	Sound::Play(1, soundIDs[blockType], camSpacePos);
	
	Particle::System::NewSimple(0x09, pos.x, pos.y, pos.z);
	
	stage = 1;
	
	return;
}

//Jiggles block up then down.
void CharacterBlock::Jiggle()
{
	switch (stage)
	{
		case 1:
			pos.y = pos.y + 0x9600_f;
			if (pos.y > oldPos.y + 0x20202_f) {
				stage = 2;
			}
			break;
		
		case 2:
			pos.y = pos.y - 0x6400_f;
			if (pos.y < oldPos.y) {
				stage = 3;
			}
			break;
		
		case 3:
			pos.y = oldPos.y;
			stage = 0;
			break;
	}
}

void CharacterBlock::UpdateModelTransform()
{
	modelAnim.mat4x3.ThisFromRotationY(ang.y);
	modelAnim.mat4x3.r0c3 = pos.x >> 3;
	modelAnim.mat4x3.r1c3 = pos.y >> 3;
	modelAnim.mat4x3.r2c3 = pos.z >> 3;
	DropShadowScaleXYZ(shadow, modelAnim.mat4x3, 0x85000_f, 0x150000_f, 0x85000_f, 0xc);
}

int CharacterBlock::InitResources()
{
	blockType = ((param1 & 0xf) > 3 ? 0 : param1 & 0xf);
	needsUnlock = ((param1 >> 4 & 0xf) != 1);
	startingCharacter = ((param1 >> 8 & 0xf) == 0 ? 3 : (param1 >> 8 & 0xf) - 1);
	healPlayer = ((param1 >> 12 & 0xf) == 1);
	
	isUnlocked = CheckUnlock();
	
	if (isUnlocked)
	{
		soundIDs[0] = 27;
		soundIDs[1] = 91;
		soundIDs[2] = 155;
		soundIDs[3] = 219;
		
		Model::LoadFile(modelFiles[blockType]);
		modelAnim.SetFile(modelFiles[blockType].filePtr, 1, -1);
		
		BoneAnimation::LoadFile(animFiles[0]);
		modelAnim.SetAnim(animFiles[0].filePtr, Animation::NO_LOOP, 0x1000_f, 0);

		MovingMeshCollider::LoadFile(clsnFile);
		clsn.SetFile(clsnFile.filePtr, clsnNextMat, 0x190_f, ang.y, clpsBlock);
		
		shadow.InitCuboid();
		
		UpdateModelTransform();
		UpdateClsnPosAndRot();
	}
	else
	{
		Model::LoadFile(modelFilesTrans[blockType]);
		modelAnim.SetFile(modelFilesTrans[blockType].filePtr, 1, -1);
		
		BoneAnimation::LoadFile(animFiles[1]);
		modelAnim.SetAnim(animFiles[1].filePtr, Animation::LOOP, 0x1000_f, 0);
		
		shadow.InitCuboid();
		
		UpdateModelTransform();
	}
	
	shadowMat = model.mat4x3 * Matrix4x3::IDENTITY;
	shadowMat.r1c3 -= 0x14000_f >> 3;
	
	stage = 0;
	oldPos = pos;
	
	return 1;
}

int CharacterBlock::CleanupResources()
{
	clsn.Disable();
	clsnFile.Release();
	
	modelFiles[0].Release();
	modelFiles[1].Release();
	modelFiles[2].Release();
	modelFiles[3].Release();
	
	modelFilesTrans[0].Release();
	modelFilesTrans[1].Release();
	modelFilesTrans[2].Release();
	modelFilesTrans[3].Release();
	
	animFiles[0].Release();
	animFiles[1].Release();
	
	return 1;
}

int CharacterBlock::Behavior()
{
	UpdateModelTransform();
	
	if (isUnlocked)
	{
		if(IsClsnInRange(0_f, 0_f))
		{
			Player* player = ClosestPlayer();
			
			if (BumpedUnderneathByPlayer(*player) && player->pos.x < pos.x + 0x53500_f && player->pos.x > pos.x - 0x53500_f && player->pos.z < pos.z + 0x53500_f && player->pos.z > pos.z - 0x53500_f)
			{
				JumpedUnderBlock();
			}
			
			if (stage > 0 && stage < 4) {
				Jiggle();
			}
			
			UpdateClsnPosAndRot();
		}
	}
	else
	{
		modelAnim.anim.Advance();
	}
	
	return 1;
}

int CharacterBlock::Render()
{
	modelAnim.Render(nullptr);
	return 1;
}

CharacterBlock::~CharacterBlock() {}
