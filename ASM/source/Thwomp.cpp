#include "Thwomp.h"

namespace
{
	enum States
	{
		ST_GO_UP,
		ST_WAIT_UP,
		ST_HIT_GROUND,
		ST_WAIT_GROUND,
		ST_WAIT_GROUND_2
	};
	
	enum GigaStates
	{
		ST_G_WAIT,
		ST_G_TALK,
		ST_G_GO_UP,
		ST_G_WAIT_UP,
		ST_G_HIT_GROUND,
		ST_G_WAIT_GROUND,
		ST_G_WAIT_GROUND_2,
		ST_G_HIT_PLAYER,
		ST_G_TALK_ED,
		ST_G_DEAD
	};
	
	struct State
	{
		using FuncPtr = void(Thwomp::*)();
		FuncPtr main;
	};
	const State states[]
	{
		//State{ &Thwomp::StHitGround },
	};
	
	FixedSizeCLPS_Block<2> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		2,
		{
			CLPS(0x04, 0, 0x3f, 0x0, 0x0, 0x00, 1, 0, 0, 0xff),
			CLPS(0x04, 0, 0x3f, 0x1, 0x0, 0x00, 1, 0, 0, 0xff)
		}
	};
	
	constexpr Fix12i WIDTH  =  0xf0000_f; //0x140000_f
	constexpr Fix12i DEPTH  =  0xa0000_f; //0xdc000_f
	constexpr Fix12i HEIGHT = 0x19a000_f; //0x19a000_f
	
	const short THWOMP_ID       = 0x227;
	const short MAGMA_THWOMP_ID = 0x228;
	const short GIGA_THWOMP_ID  = 0x229;
	
	constexpr Fix12i BOSS_START_BATTLE_RADIUS = 0xbb8000_f;
	constexpr Fix12i BOSS_STOP_BATTLE_RADIUS = 0x1600000_f;
	
	constexpr uint16_t waitMsgIDs  [] = {0x0096, 0x0097, 0x0098, 0x0099};
	constexpr uint16_t defeatMsgIDs[] = {0x009a, 0x009b, 0x009c, 0x009d};
	constexpr Fix12i TALK_HEIGHT = 0x1d74a6_f;
	
	constexpr Fix12i COLISSION_RADIUS[] = {0x140_f, 0x140_f, 0x340_f};
	constexpr unsigned SHADOW_SCALE[] = {1, 1, 2};
}

SharedFilePtr Thwomp::modelFiles[3];
SharedFilePtr Thwomp::texSeqFile;
SharedFilePtr Thwomp::clsnFile;

SpawnInfo<Thwomp> Thwomp::spawnData =
{
	&Thwomp::Spawn,
	0x00a1,
	0x0047,
	0x02000002,
	0x00100000_f,
	0x00200000_f,
	0x01000000_f,
	0x01000000_f,
};

Thwomp* Thwomp::Spawn()
{
	return new Thwomp;
}

//02133254
/*void Thwomp::InitResourcesHelper()
{
	
}*/

void Thwomp::UpdateModelPosAndRotXYZ()
{
	model.mat4x3.ThisFromRotationXYZExt(ang.x, ang.y, ang.z);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}

int Thwomp::InitResources()
{
	//modelFilePtr = &resources; //???
	
	if (actorID == THWOMP_ID)
		thwompType = ThwompType::NORMAL;
	else if (actorID == MAGMA_THWOMP_ID)
		thwompType = ThwompType::MAGMA;
	else if (actorID == GIGA_THWOMP_ID)
		Crash(); //unfinished
	
	char* modelF = Model::LoadFile(modelFiles[thwompType]);
	model.SetFile(modelF, 1, -1);
	
	UpdateModelPosAndRotY();
	UpdateClsnPosAndRot();
	
	char* clsnF = MovingMeshCollider::LoadFile(clsnFile);
	clsn.SetFile(clsnF, clsnNextMat, COLISSION_RADIUS[thwompType], ang.y, clpsBlock);
	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	clsn.Enable(this);
	
	if (thwompType != ThwompType::GIGA)
	{
		TextureSequence::LoadFile(texSeqFile);
		TextureSequence::Prepare(modelFiles[thwompType].filePtr, texSeqFile.filePtr);
		texSeq.SetFile(texSeqFile.filePtr, Animation::NO_LOOP, 0x1000_f, 0);
	}
	
	if(!shadow.InitCuboid())
		return 0;
	
	minPosY = -0x7d00000_f;
	
	RaycastGround raycaster;
	raycaster.SetObjAndPos(Vector3{pos.x, pos.y + 0x32000_f, pos.z}, nullptr);
	if(raycaster.DetectClsn())
		minPosY = raycaster.clsnPosY;
	
	minPosYSide = minPosY + 100._f;
	maxPosY = pos.y;
	maxPosYSide = maxPosY + 100._f;
	pos.y = minPosY;
	waitTime = 40;
	
	originalPos = pos;
	justHit = false;
	starID = param1;
	ang.x = 0x0000;
	
	vertAccel = -0x4000_f;
	termVel = -0x3c000_f;
	
	if (thwompType == ThwompType::GIGA)
		state = ST_GO_UP;
	else
		state = ST_G_WAIT;
	
	actionWaitTime = 0;
	
	return 1;
}

int Thwomp::CleanupResources()
{
	clsn.Disable();
	texSeqFile.Release();
	modelFiles[0].Release();
	modelFiles[1].Release();
	modelFiles[2].Release();
	clsnFile.Release();
	return 1;
}

//02133020
void Thwomp::GoUp()
{
	ang.x -= 0x01FF;
	
	if (ang.x > 0x3FFF || ang.x < 0x0000)
		ang.x = 0x0000;
	
	if (health == 1)
		ang.x = 0x3FFF;
	
	if (thwompType == ThwompType::GIGA && health == 1)
		pos.y += 0x3f000_f;
	else if (thwompType == ThwompType::GIGA && justHit)
		pos.y += 0x7000_f;
	else
		pos.y += 0xa000_f;
	
	if (thwompType == ThwompType::GIGA && ((curSlam == 3 && health == 3) || (curSlam == 5 && health == 2) || health == 1))
	{
		if(pos.y < maxPosYSide)
			return;
		
		pos.y = maxPosYSide;
	}
	else
	{
		if(pos.y < maxPosY)
			return;
		
		pos.y = maxPosY;
	}
	
	
	state = thwompType != ThwompType::GIGA ? ST_WAIT_UP : ST_G_WAIT_UP;
	waitTime = thwompType != ThwompType::GIGA ? 25 : (curSlam == 3 && health == 3) || (curSlam == 5 && (health == 2 || health == 1)) ? 40 : 20;
	
	if ((curSlam == 4 && health == 3) || (curSlam == 6 && (health == 2 || health == 1)))
		curSlam = 0;
}

//02132ff4
void Thwomp::WaitUp()
{
	if ((curSlam == 3 && health == 3) || (curSlam == 5 && (health == 2 || health == 1)) || (justHit && health == 1))
		ang.x += 0x13FF;
	
	if(DecIfAbove0_Byte(waitTime) == 0)
	{
		if (health == 1)
			ang.x = 0x3FFF;
		
		state = thwompType != ThwompType::GIGA ? ST_HIT_GROUND : ST_G_HIT_GROUND;
		if (thwompType == ThwompType::GIGA && ((curSlam == 3 && health == 3) || (curSlam == 5 && (health == 2 || health == 1))))
			ang.x = 0x3FFF;
	}
}

//02132f04
void Thwomp::HitGround()
{
	justHit = false;
	
	speed.y += vertAccel;
	if(speed.y < termVel)
		speed.y = termVel;
	
	pos.y += speed.y;
	
	if (thwompType == ThwompType::GIGA && ((curSlam == 3 && health == 3) || (curSlam == 5 && health == 2) || health == 1))
	{
		if(pos.y > minPosYSide)
			return;
		
		pos.y = minPosYSide;
	}
	else
	{
		if(pos.y > minPosY)
			return;
		
		pos.y = minPosY;
	}
	
	speed.y = 0_f;
	state = thwompType != ThwompType::GIGA ? ST_WAIT_GROUND : ST_G_WAIT_GROUND;
	waitTime = thwompType != ThwompType::GIGA ? 10 : (curSlam == 3 && health == 3) || (curSlam == 5 && (health == 2 || health == 1)) ? 20 : 10;
	
	BigLandingDust(true);
	Earthquake(pos, 0x7d0000_f);
	Sound::PlayBank3(0xc7, camSpacePos);
	
	
	curSlam++;
	
	
}

//02132e98
void Thwomp::WaitGround()
{
	if(DecIfAbove0_Byte(waitTime) != 0)
		return;
	
	state = thwompType != ThwompType::GIGA ? ST_WAIT_GROUND_2 : ST_G_WAIT_GROUND_2;
	waitTime = thwompType != ThwompType::GIGA ? 25 : (curSlam == 4 && health == 3) || (curSlam == 6 && (health == 2 || health == 1)) ? 50 : 10;
}

//02132e64
void Thwomp::WaitGround2()
{
	if(DecIfAbove0_Byte(waitTime) == 0)
	{
		state = thwompType != ThwompType::GIGA ? ST_GO_UP : ST_G_GO_UP;
		waitTime = thwompType != ThwompType::GIGA ? 40 : 30;
	}
}

//02133098
void Thwomp::DropShadow()
{
	Fix12i shadowHeight = pos.y - minPosY + 0x12000_f;
	shadowMat = model.mat4x3;
	shadowMat.r1c3 -= 0x10000_f;
	DropShadowScaleXYZ(shadow, shadowMat, WIDTH * SHADOW_SCALE[thwompType], shadowHeight, DEPTH * SHADOW_SCALE[thwompType], 0xf);
}

void Thwomp::AttemptTalkStartIfNotStarted()
{
	Player* player = ClosestPlayer();
	if(player->StartTalk(*this, true))
	{
		Message::PrepareTalk();
		listener = player;
	}
}

void Thwomp::Talk()
{
	if(!listener)
		return;
	
	int talkState = listener->GetTalkState();
	switch(talkState)
	{
		case Player::TK_NOT:
			Message::EndTalk();
			listener = nullptr;
			if(state == ST_G_TALK_ED)
			{
				state = ST_G_DEAD;
				KillAndTrackInDeathTable();
				Sound::StopLoadedMusic();
				if (starID < 8)
					Actor::Spawn(0x00b2, (0x0040 + starID), Vector3{pos.x, pos.y + 250._f, pos.z}, nullptr, 0, -1);
				else if (starID < 13)
					Actor::Spawn(0x011a, (0x0000 + starID - 8), Vector3{pos.x, pos.y + 250._f, pos.z}, nullptr, 0, -1);
			}
			else
			{
				state = ST_G_GO_UP;
				Sound::LoadAndSetMusic(Sound::MU_BOSS);
			}
			break;
			
		case Player::TK_START:
			listener->ShowMessage(*this, state == ST_G_TALK_ED ? defeatMsgIDs[listener->param1] : waitMsgIDs[listener->param1], Vector3{pos.x, pos.y + TALK_HEIGHT, pos.z}, 0, 0);
			break;
			
		default:
			return;
	}
}

int Thwomp::Behavior()
{
	if (thwompType != ThwompType::GIGA)
	{
		switch(state)
		{
			case ST_GO_UP:
				if((int)texSeq.currFrame != 0)
				{
					texSeq.currFrame -= 0x1000_f;
					if((int)texSeq.currFrame == 0)
						actionWaitTime = 10; //decimal
				}
				else if(actionWaitTime == 0)
					GoUp();
				else
					--actionWaitTime;
				break;
				
			case ST_WAIT_UP:
				WaitUp();
				break;
				
			case ST_HIT_GROUND:
				texSeq.Advance();
				if(texSeq.Finished())
				{
					if(actionWaitTime == 0)
						HitGround();
					else
						--actionWaitTime;
				}
				else
					actionWaitTime = 5;
				break;
				
			case ST_WAIT_GROUND:
				WaitGround();
				break;
				
			case ST_WAIT_GROUND_2:
				WaitGround2();
				break;
				
		}
	}
	else //Giga Thwomp
	{
		/*
		ST_G_WAIT,
		ST_G_TALK,
		ST_G_GO_UP,
		ST_G_WAIT_UP,
		ST_G_HIT_GROUND,
		ST_G_WAIT_GROUND,
		ST_G_WAIT_GROUND_2,
		ST_G_TALK_ED
		*/
		Player* player = ClosestPlayer();
		if(battleStarted && (!player || originalPos.Dist(player->pos) >= BOSS_STOP_BATTLE_RADIUS) && health != 0)
		{
			state = ST_G_WAIT;
			battleStarted = false;
			health = 3;
			pos = originalPos;
			ang.x = 0x0000;
			Sound::StopLoadedMusic();
		}
		
		switch(state)
		{
			case ST_G_WAIT:
				if (originalPos.Dist(player->pos) <= BOSS_START_BATTLE_RADIUS)
					state = ST_G_TALK;
				
				pos = originalPos;
				break;
				
			case ST_G_TALK:
				Talk();
				if (state == ST_G_TALK)
					AttemptTalkStartIfNotStarted();
				battleStarted = true;
				health = 1;
				curSlam = 0;
				break;
				
			case ST_G_GO_UP:
				GoUp();
				break;
				
			case ST_G_WAIT_UP:
				WaitUp();
				break;
				
			case ST_G_HIT_GROUND:
				HitGround();
				break;
				
			case ST_G_WAIT_GROUND:
				WaitGround();
				break;
				
			case ST_G_WAIT_GROUND_2:
				WaitGround2();
				break;
				
			case ST_G_HIT_PLAYER:
				if (health != 0)
					state = ST_G_GO_UP;
				else
					state = ST_G_TALK_ED;
				break;
				
			case ST_G_TALK_ED:
				Talk();
				if (state == ST_G_TALK_ED)
					AttemptTalkStartIfNotStarted();
				break;
				
			case ST_G_DEAD:
				break;
				
		}
	}
	
	UpdateModelPosAndRotXYZ();
	
	if(IsClsnInRange(0_f, 0_f))
		UpdateClsnPosAndRot();
	
	if (actorID == MAGMA_THWOMP_ID)
	{
		if (DistToCPlayer() < 750._f && ClosestPlayer()->currState != (Player::State*)Player::ST_BURN_FIRE && PLAYER_HEALTH != 0)
		{
			ClosestPlayer()->Burn();
		}
	}
	
	DropShadow();
	
	return 1;
}

int Thwomp::Render()
{
	if (thwompType != ThwompType::GIGA)
		texSeq.Update(model.data);
	model.Render(nullptr);
	return 1;
}

Thwomp::~Thwomp() {}

void Thwomp::OnHitByMegaChar(Player& megaChar)
{
	megaChar.IncMegaKillCount();
	Particle::System::NewSimple(0x48, pos.x, pos.y + HEIGHT / 2, pos.z);
	PoofDustAt(Vector3{pos.x, pos.y + HEIGHT / 2, pos.z});
	Destroy();
	Sound::PlayBank3(0x1e, camSpacePos);
}

void Thwomp::OnGroundPounded(Actor& groundPounder)
{
	if (thwompType != ThwompType::GIGA)
		return;
	
	if (((curSlam == 4 && health == 3) || (curSlam == 6 && health == 2) || (health == 1 && !justHit)) && ang.x == 0x3FFF)
	{
		health--;
		curSlam = 0;
		justHit = true;
		state = ST_G_HIT_PLAYER;
		//Play particles
		Sound::PlayBank3(0x1e, camSpacePos);
	}
}