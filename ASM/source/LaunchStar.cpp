#include "LaunchStar.h"

namespace
{
	enum Animations
	{
		WAIT,
		LAUNCH,
		
		NUM_ANIMS
	};
	
}

SharedFilePtr LaunchStar::modelFile;
SharedFilePtr LaunchStar::animFiles[2];
LaunchStar* LaunchStar::ls_ptr;
BezierPathIter LaunchStar::bzIt;
Vector3_16 LaunchStar::lsDiffAng;
Vector3_16 LaunchStar::lsInitAng;

SpawnInfo<LaunchStar> LaunchStar::spawnData =
{
	&LaunchStar::Spawn,
	0x0168,
	0x00aa,
	0x00000003,
	0x00000000_f,
	0x00600000_f,
	0x01000000_f,
	0x01000000_f
};

bool Player::LS_Init()
{
	horzSpeed = vertAccel = 0_f;
	lsInitPos = pos;
	lsPtr = LaunchStar::ls_ptr;
	lsPos = LaunchStar::ls_ptr->pos; //launch star pos
	lsPos.y += 0xa0000_f; //correct y-pos of target launch star
	LaunchStar::lsInitAng = ang;
	LaunchStar::lsDiffAng.x = LaunchStar::ls_ptr->ang.x - ang.x;
	LaunchStar::lsDiffAng.y = LaunchStar::ls_ptr->ang.y - ang.y;
	LaunchStar::lsDiffAng.z = LaunchStar::ls_ptr->ang.z - ang.z;
	SetAnim(0x5f, Animation::NO_LOOP, 0x00001000_f, 0);
	Sound::PlayBank3(0x61, camSpacePos);
	return true;
}

/*void Vector3::Lerp(const Vector3& a, const Vector3& b, Fix12i t)
{
    *this = b;
    *this -= a;
    *this *= t;
    *this += a;
}*/

bool Player::LS_Behavior()
{
	Fix12s t = Fix12s(lsState0Timer) >> 2;
	speed.y = 0_f;
	Model& lsModel = lsPtr->rigMdl;
	Animation& lsAnim = lsPtr->rigMdl.anim;

	if(launchState == 1 || launchState == 2)
	{
		lsModel.data.UpdateBones(lsAnim.file, (int)lsAnim.currFrame);

		pos = lsModel.data.bones[1].pos.Transform(lsModel.mat4x3) << 3;
	}

	switch(launchState)
	{
		case 0:

			//pos = Vector3::Interp(lsInitPos, lsPos, t);
			pos = lsPos; //Replacement interpolation code.
			pos -= lsInitPos;
			pos *= t;
			pos += lsInitPos;

			ang.x = (Fix12s(LaunchStar::lsDiffAng.x, true) * t).val + LaunchStar::lsInitAng.x;
			ang.y = (Fix12s(LaunchStar::lsDiffAng.y, true) * t).val + LaunchStar::lsInitAng.y;
			ang.z = (Fix12s(LaunchStar::lsDiffAng.z, true) * t).val + LaunchStar::lsInitAng.z;

			++lsState0Timer; //increment state timer
			if(lsState0Timer > 4)
			{
				lsPtr->rigMdl.SetAnim(LaunchStar::animFiles[LAUNCH].filePtr, Animation::NO_LOOP, 0x00001000_f, 0x00000000);
				launchState = 1;
			}
			break;
		case 1:
			if(lsAnim.currFrame >= 0x14000_f)
			{
				lsAnim.speed = lsPtr->launchSpeed / 8;
				Sound::PlayBank3(0x14f, camSpacePos);
				Sound::PlayBank3(0x0c5, camSpacePos);
				Sound::PlayBank0(0x0b9, camSpacePos);
				launchState = 2;
			}
			break;
		case 2:

			if(lsAnim.currFrame >= 0x2f000_f)
			{
				lsAnim.currFrame = 0x2f000_f;
				lsAnim.speed = 0x00001000_f;
				pos = lsPos;

				LaunchStar::bzIt.pathPtr = lsPtr->pathPtr;
				LaunchStar::bzIt.currSplineX3 = 0;
				LaunchStar::bzIt.tinyStep = 0x0010_fs;
				LaunchStar::bzIt.step = lsPtr->launchSpeed;
				LaunchStar::bzIt.currTime = 0_f;
				LaunchStar::bzIt.pos = lsPos;

				Vector3_16f zeros {0_fs, 0_fs, 0_fs};
				lsPtr->particleID = Particle::System::New(lsPtr->particleID, 0x114, pos.x, pos.y, pos.z, &zeros, nullptr);
				launchState = 3;
			}
			break;
		case 3:

			bool finished = !LaunchStar::bzIt.Advance();
			pos = LaunchStar::bzIt.pos;

			ang.y = motionAng.y = prevPos.HorzAngle(pos);
			ang.x = 0x4000 - pos.VertAngle(prevPos);

			Vector3_16f zeros {0_fs, 0_fs, 0_fs};
			lsPtr->particleID = Particle::System::New(lsPtr->particleID, 0x114, pos.x, pos.y, pos.z, &zeros, nullptr);

			if(finished)
			{
				Vector3 p2;
				LaunchStar::bzIt.pathPtr.GetPt(p2, LaunchStar::bzIt.currSplineX3 + 2);

				short ang = pos.VertAngle(p2);
				horzSpeed = Cos(ang) * LaunchStar::bzIt.step;
				speed.y   = Sin(ang) * LaunchStar::bzIt.step;
				ChangeState(*(Player::State*)Player::ST_FALL);
			}
			break;
	}

	prevPos = pos;

	/*if((launchState == 1 && lsModel.anim.currFrame > 0) || launchState == 2)
	{
		lsModel.anim.currFrame -= lsModel.anim.speed;
	}*/

	return true;
}

bool Player::LS_Cleanup()
{
	switch((unsigned)nextState)
	{
		case Player::ST_SWIM:
		case Player::ST_HURT:
		case Player::ST_HURT_WATER:
			return false;
		default:
			return true;
	}
}

void LaunchStar::UpdateModelTransform()
{
	Model& model = rigMdl;
	model.mat4x3.ThisFromRotationZXYExt(ang.x, ang.y, ang.z);
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}

LaunchStar* LaunchStar::Spawn()
{
	return new LaunchStar;
}

int LaunchStar::InitResources()
{
	Model::LoadFile(modelFile);
	rigMdl.SetFile(modelFile.filePtr, 1, -1);
	
	BoneAnimation::LoadFile(animFiles[WAIT]);
	BoneAnimation::LoadFile(animFiles[LAUNCH]);
	rigMdl.SetAnim(animFiles[WAIT].filePtr, Animation::LOOP, 0x1000_f, 0);
	
	cylClsn.Init(this, 0x000a0000_f, 0x00140000_f, 0x00100002, 0x00008000);
	
	launchSpeed = Fix12i(ang.x);
	eventID = param1 >> 8 & 0xff;
	particleID = 0;
	camSet = true;
	camFrames = 0;
	flags--;
	
	Vector3 p0, p1;
	pathPtr.FromID(param1 & 0xff);
	pathPtr.GetPt(p0, 0);
	pathPtr.GetPt(p1, 1);
	
	ang.x = 0x4000 - p1.VertAngle(p0);
	ang.y = p0.HorzAngle(p1);
	ang.z = 0;
	
	UpdateModelTransform();
	
	pos.y -= 0x000a0000_f; //silly cylinder colliders that can't be offset
	
	//Event::ClearBit(eventID);
	
	return 1;
}

int LaunchStar::CleanupResources()
{
	animFiles[WAIT].Release();
	animFiles[LAUNCH].Release();
	modelFile.Release();
	return 1;
}

int LaunchStar::Behavior()
{
	if(eventID < 0x20)
	{
		if(Event::GetBit(eventID))
		{
			Event::ClearBit(eventID);
			eventID = 0xff;
			camSet = false;
			camOldPos = CAMERA->pos;
			camOldTarget = CAMERA->lookAt;
			
			camNewPos = pos;
			camNewPos.y += 0x250000_f;
			camNewPos.x += Sin(ang.x) * 0x350000_f;
			camNewPos.z += Cos(ang.y) * 0x350000_f;
			
			playerOldPos = PLAYER_ARR[0]->pos;
			playerOldAng = PLAYER_ARR[0]->ang;
			playerOldState = PLAYER_ARR[0]->currState;
			playerOldPrevState = PLAYER_ARR[0]->prevState;
			playerOldNextState = PLAYER_ARR[0]->nextState;
			
			//Teleport the player up 10,000 editor measurements to ensure the player doesn't collide with objects or takes damage
			playerNewPos = playerOldPos;
			playerNewPos.y += 0x10000000_f;
		}
		else
			return 1;
	}
	
	if (eventID == 0xff && !camSet)
	{
		camFrames++;
		
		if (camFrames <= 90)
		{
			
			//Keep the camera looking at the Launch Star
			CAMERA->lookAt = pos;
			CAMERA->pos = camNewPos;
			
			//Keep the player in the same position and rotation so they can't move
			PLAYER_ARR[0]->pos = playerNewPos;
			PLAYER_ARR[0]->ang = playerOldAng;
			
			//Play the 'secret found' sound effect
			if (camFrames == 15)
				Sound::LoadAndSetMusic(0x029);
				//WOOD: Sound::PlayBank3(0x041, camSpacePos);
		}
		else
		{
			//Reset the player's original states and position
			PLAYER_ARR[0]->currState = playerOldState;
			PLAYER_ARR[0]->prevState = playerOldPrevState;
			PLAYER_ARR[0]->nextState = playerOldNextState;
			PLAYER_ARR[0]->pos = playerOldPos;
			
			//Reset the camera to it's original position
			CAMERA->lookAt = camOldTarget;
			CAMERA->pos = camOldPos;
			camSet = true;
		}
	}
		
	Player* isThereAPlayer = ClosestPlayer();
	
	if(isThereAPlayer)
	{
		Player& player = *isThereAPlayer;
		
		if(player.uniqueID == cylClsn.otherObjID && (INPUT_1_FRAME & Input::A) &&
		   ((unsigned)player.currState != Player::ST_LAUNCH_STAR || player.lsPtr != this))
		{
			ls_ptr = this;
			((Player::State*)Player::ST_LAUNCH_STAR)->init    = &Player::LS_Init;
			((Player::State*)Player::ST_LAUNCH_STAR)->main    = &Player::LS_Behavior;
			((Player::State*)Player::ST_LAUNCH_STAR)->cleanup = &Player::LS_Cleanup;
			player.ChangeState(*(Player::State*)Player::ST_LAUNCH_STAR);
		}
	}
	
	if(rigMdl.anim.file == animFiles[LAUNCH].filePtr && rigMdl.anim.currFrame + 1_f >= rigMdl.anim.GetNumFrames())
		rigMdl.SetAnim(animFiles[WAIT].filePtr, Animation::LOOP, 0x1000_f, 0);
	
	rigMdl.anim.Advance();
	cylClsn.Clear();
	cylClsn.Update();
	return 1;
}

int LaunchStar::Render()
{
	if(eventID >= 0x20)
		rigMdl.Render(nullptr);
	
	return 1;
}

LaunchStar::~LaunchStar() {}