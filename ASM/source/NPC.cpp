#include "NPC.h"
#include "include/ostream.h"

namespace
{
	enum Animations
	{
		WAIT,
		TALK,
		
		NUM_ANIMS
	};
	
	enum NPCs
	{
		YOSHI_NPC,
		COLORED_TOAD_NPC,
		PEACH_NPC,
		
		NUM_NPCS
	};
	
	enum NPC_IDs
	{
		YOSHI_NPC_ID = 0x218,
		COLORED_TOAD_NPC_ID = 0x220,
		PEACH_NPC_ID = 0x221
	};
	
	enum ToadColor
	{
		RED,
		YELLOW,
		GREEN,
		BLUE,
		PURPLE,
		
		NUM_TOAD_COLORS
	};
	
	using StateFuncPtr = void(NPC::*)();
	
	const StateFuncPtr stateFuncs[]
	{
		&NPC::State0_Wait,
		&NPC::State1_Talk
	};
	
	constexpr Fix12i RADIUS = 0x78000_f;
	constexpr Fix12i HEIGHT = 0x96000_f;
	constexpr Fix12i TALK_RADIUS = 0xf0000_f;
	constexpr Fix12i TALK_HEIGHT = 0x70000_f;
	constexpr short TURN_SPEED = 0x0800;
};

SharedFilePtr NPC::modelFiles[3];
SharedFilePtr NPC::animFiles[6];
SharedFilePtr NPC::texSeqFile;

SpawnInfo<NPC> NPC::spawnData =
{
	&NPC::Spawn,
	0x003b,
	0x00aa,
	0x00000003,
	0x00032000_f,
	0x00046000_f,
	0x01000000_f,
	0x01000000_f
};

void NPC::UpdateModelTransform()
{
	rigMdl.mat4x3.ThisFromRotationY(ang.y);
	rigMdl.mat4x3.r0c3 = pos.x >> 3;
	rigMdl.mat4x3.r1c3 = pos.y >> 3;
	rigMdl.mat4x3.r2c3 = pos.z >> 3;
	
	DropShadowRadHeight(shadow, rigMdl.mat4x3, 0x62980_f, 0x37000_f, 0xf); //radius and height are (C) Yoshi the Player.
}

NPC* NPC::Spawn()
{
	return new NPC;
}

int NPC::InitResources()
{
	if (actorID == YOSHI_NPC_ID)
		npcID = YOSHI_NPC;
	else if (actorID == COLORED_TOAD_NPC_ID)
		npcID = COLORED_TOAD_NPC;
	else if (actorID == PEACH_NPC_ID)
		npcID = PEACH_NPC;
	else
	{
		cout << "Actor is not a NPC." << '\n';
		Crash();
	}
	
	//The player should load his stuff first, so the SharedFilePtr's should be there before now.
	Model::LoadFile(modelFiles[npcID]);
	for(int i = 0; i < NUM_ANIMS; ++i)
		BoneAnimation::LoadFile(animFiles[i + 2 * npcID]);
	
	rigMdl.SetFile(modelFiles[npcID].filePtr, 1, -1);
	rigMdl.SetAnim(animFiles[WAIT + 2 * npcID].filePtr, Animation::LOOP, 0x1000_f, 0);
	
	cylClsn.Init(this, RADIUS, HEIGHT, 0x04200004, 0x00000000);
	shadow.InitCylinder();
	
	RaycastGround raycaster;
	raycaster.SetObjAndPos(Vector3{pos.x, pos.y + 0x14000_f, pos.z}, this);
	if(raycaster.DetectClsn())
		pos.y = raycaster.clsnPosY;
	
	UpdateModelTransform();
	
	state = 0;
	listener = nullptr;
	starSpawned = false;
	
	messages[0] = param1 & 0xfff;
	messages[1] = ang.x  & 0xfff;
	starID = (param1 >> 12 & 0xf);
	eventID = (ang.x >> 12 & 0xf);
	
	//Only change lighting color on Yoshi NPCs
	if (npcID == YOSHI_NPC)
	{
		unsigned r = ang.z >>  0 & 0x1f,
				 g = ang.z >>  5 & 0x1f,
				 b = ang.z >> 10 & 0x1f;
		rigMdl.data.materials[1].difAmb =
			rigMdl.data.materials[2].difAmb = (uint16_t)ang.z | 0x8000 | r >> 1 << 16 
																	   | g >> 1 << 21
																	   | b >> 1 << 26;
	} //Only set texture sequence on Colored Toads
	else if (npcID == COLORED_TOAD_NPC)
	{
		TextureSequence::LoadFile(texSeqFile);
		TextureSequence::Prepare(modelFiles[npcID].filePtr, texSeqFile.filePtr);
		texSeq.SetFile(texSeqFile.filePtr, Animation::NO_LOOP, 0x10000_f, 
		(ang.z > 5 ? 0 : ang.z));
	}
	
														 
	shouldTalk = (npcID == YOSHI_NPC ? ang.z & 0x8000 : true);
	ang.x = ang.z = 0;
	
	return 1;
}

int NPC::CleanupResources()
{
	for (int i = 0; i < NUM_NPCS; ++i)
		modelFiles[i].Release();
	
	for (int i = 0; i < NUM_ANIMS * NUM_NPCS; ++i)
		animFiles[i].Release();
	
	texSeqFile.Release();
	
	return 1;
}

void NPC::State0_Wait()
{
	rigMdl.SetAnim(animFiles[WAIT + 2 * npcID].filePtr, Animation::LOOP, 0x1000_f, 0);
	if(!(cylClsn.hitFlags & CylinderClsn::HIT_BY_PLAYER))
		return;
	
	Actor* actor = Actor::FindWithID(cylClsn.otherObjID);
	if(!actor || actor->actorID != 0x00bf)
		return;
	
	Player& player = *(Player*)actor;
	if(player.StartTalk(*this, false))
	{
		Message::PrepareTalk();
		state = 1;
		listener = &player;
	}
}
void NPC::State1_Talk()
{
	rigMdl.SetAnim(animFiles[TALK + 2 * npcID].filePtr, Animation::LOOP, 0x1000_f, 0);
	if(!AdvanceToDest_Short(ang.y, pos.HorzAngle(listener->pos), TURN_SPEED))
		return;
	
	int talkState = listener->GetTalkState();
	switch(talkState)
	{
		case Player::TK_NOT:
			Message::EndTalk();
			state = 0;
			if ((Event::GetBit(eventID) || eventID == 0xf) && starID < 8 && !starSpawned)
			{
				//Vector3 vec = pos;
				//vec.y += 150._f;
				Actor::Spawn(0x00b2, 0x0040 + starID, pos, nullptr, 0, -1);
				starSpawned = true;
			}
			break;
			
		case Player::TK_START:
			listener->ShowMessage(*this, shouldTalk ? (Event::GetBit(eventID) && eventID != 0xf ? messages[1] : messages[0]) : 0x000C,
				Vector3{pos.x, pos.y + TALK_HEIGHT, pos.z}, 0, 0);
			Sound::PlayCharVoice(Player::CH_YOSHI, 0x4, camSpacePos);
			break;
			
		default:
			return;
	}
}

int NPC::Behavior()
{
	rigMdl.anim.Advance();
	
	(this->*stateFuncs[state])();
	
	cylClsn.Clear();
	cylClsn.Update();
	
	UpdateModelTransform();
	
	return 1;
}

int NPC::Render()
{
	if (npcID == COLORED_TOAD_NPC)
		texSeq.Update(rigMdl.data);
	
	rigMdl.Render(nullptr);
	return 1;
}

NPC::~NPC() {}