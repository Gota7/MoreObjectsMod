#include "ToxBox.h"
#include "ToxBoxFlower.h"

namespace
{
	enum States
	{
		ST_HIT_GROUND,
		ST_STOP,
		ST_MOVE,
		ST_JUMP,
		ST_ANTIGRAVITY
	};
	
	struct State
	{
		using FuncPtr = void(ToxBox::*)();
		FuncPtr main;
	};
	const State states[]
	{
		State{ &ToxBox::StHitGround },
		State{ &ToxBox::StStop },
		State{ &ToxBox::StMove },
		State{ &ToxBox::StJump },
		State{ &ToxBox::StAntigravity }
	};
	
	FixedSizeCLPS_Block<1> clpsBlock =
	{
		{'C', 'L', 'P', 'S'},
		8,
		1,
		{
			CLPS(0x04, 0, 0x3f, 0x1, 0x0, 0x00, 1, 0, 0, 0xff)
		}
	};
	
	constexpr Fix12i RADIUS = 0xfa000_f;
	constexpr Fix12i SQRT_2 = 0x16a1_f;
	constexpr Fix12i SQRT_SQRT_2 = 0x1307_f; //compromise, because CubeClsn does not exist.
	constexpr Vector3 DIRS[] =
	{
		Vector3{ 0x0000_f, 0x0000_f,  0x1000_f},
		Vector3{ 0x1000_f, 0x0000_f,  0x0000_f},
		Vector3{ 0x0000_f, 0x0000_f, -0x1000_f},
		Vector3{-0x1000_f, 0x0000_f,  0x0000_f}
	};
}

SharedFilePtr ToxBox::modelFiles[2];
SharedFilePtr ToxBox::clsnFiles[2];
SharedFilePtr PlayerToxBox::modelFile;
SharedFilePtr PlayerToxBox::texSeqFile;

SpawnInfo<ToxBox> ToxBox::spawnData =
{
	&ToxBox::Spawn,
	0x0030,
	0x0132,
	0x02000002,
	RADIUS,
	0x002ee000_f,
	0x01fa0000_f,
	0x01fa0000_f,
};

ToxBox* ToxBox::Spawn()
{
	return new ToxBox;
}

void ToxBox::UpdateClsnTransform()
{
	clsnNextMat = model.mat4x3;
	clsnNextMat.r0c3 = pos.x;
	clsnNextMat.r1c3 = pos.y;
	clsnNextMat.r2c3 = pos.z;
	clsn.Transform(clsnNextMat, ang.y);
}

void ToxBox::UpdateModelTransform()
{
	model.mat4x3 = baseMat * turnMat * orientMat;
	model.mat4x3.r0c3 = pos.x >> 3;
	model.mat4x3.r1c3 = pos.y >> 3;
	model.mat4x3.r2c3 = pos.z >> 3;
}

int ToxBox::InitCommonResources()
{
	baseMat.ThisFromRotationXYZExt(ang.x, ang.y, ang.z);
	normal = Vector3{baseMat.r0c1, baseMat.r1c1, baseMat.r2c1};
	radiusNormal = normal * RADIUS;
	pos += normal * RADIUS;
	
	unsigned orientMatID = param1 & 0x1f; //24 possible values
	Vector3 matBlob[] =
	{
		Vector3{0x1000_f, 0x0000_f, 0x0000_f},
	    Vector3{0x0000_f, 0x1000_f, 0x0000_f},
	    Vector3{0x0000_f, 0x0000_f, 0x1000_f},
	    Vector3{0x1000_f, 0x0000_f, 0x0000_f},
	    Vector3{0x0000_f, 0x1000_f, 0x0000_f}
	};
	Vector3* matPtr = &matBlob[orientMatID / 8];
	if(orientMatID & 4)
	{
		Vector3 temp = matPtr[1];
		matPtr[1] = matPtr[2];
		matPtr[2] = temp * -0x1000_f;
	}
	if(orientMatID & 2)
	{
		matPtr[1] *= -0x1000_f;
		matPtr[2] *= -0x1000_f;
	}
	if(orientMatID & 1)
	{
		matPtr[0] *= -0x1000_f;
		matPtr[1] *= -0x1000_f;
	}
	MultiCopy_Int((void*)matPtr, (void*)&orientMat, 36);
	turnMat.LoadIdentity();
	turnAng = 0;
	baseMatInv = baseMat.Inv();
	UpdateModelTransform();
	UpdateClsnTransform();
	backAndForth = !(param1 & 0x40);
	
	char* clsnF = MovingMeshCollider::LoadFile(clsnFiles[param1 >> 7 & 1]);
	clsn.SetFile(clsnF, clsnNextMat, 0x1000_f, ang.y, clpsBlock);
	clsn.beforeClsnCallback = (decltype(clsn.beforeClsnCallback))0x02039348;
	
	return 1;
}

int ToxBox::InitResources()
{
	char* modelF = Model::LoadFile(modelFiles[param1 >> 7 & 1]);
	model.SetFile(modelF, 1, -1);
	
	pathPtr.FromID(param1 >> 8 & 0xff);
	numPathPts = pathPtr.NumPts();
	currPathPtID = (ang.z << 4 & 0xf0) | (ang.x & 0xf);
	pathPtr.GetPt(currPathPt, currPathPtID);
	
	ang.x &= ~0xf;
	ang.z &= ~0xf;
	
	return InitCommonResources();
}

int ToxBox::CleanupResources()
{
	clsn.Disable();
	modelFiles[param1 >> 7 & 1].Release();
	clsnFiles [param1 >> 7 & 1].Release();
	return 1;
}

void ToxBox::AdvancePath()
{
	if(backward)
	{
		if(currPathPtID == 0)
			backward = false;
		else
			--currPathPtID;
	}
	else
	{
		if(backAndForth && currPathPtID == numPathPts - 1)
			backward = true;
		else
			currPathPtID = (currPathPtID + 1) % numPathPts;
	}
}

void ToxBox::InitMoveState()
{
	state = ST_MOVE;
	if(numTurns == 0)
	{
		if(actorID != 0x22B)
		{
			prevPathPt = currPathPt;
			AdvancePath();
			pathPtr.GetPt(currPathPt, currPathPtID);
		}
		
		Vector3 diff = currPathPt - prevPathPt;
		diff.TransformThis(baseMatInv);
		if(diff.y.Abs() < 0x1000_f)
			diff.y = 0_f;
		else
			state = ST_ANTIGRAVITY;
		
		if(diff.x.Abs() > diff.z.Abs())
		{
			diff.z = 0_f;
			turnDir = diff.x >= 0_f ? 1 : 3;
		}
		else
		{
			diff.x = 0_f;
			turnDir = diff.z >= 0_f ? 0 : 2;
		}
		if(state != ST_ANTIGRAVITY)
		{
			numTurns = (int)(diff.HorzLen() / (2 * RADIUS) + 0x800_f);
			if(numTurns == 0)
			{
				state = ST_JUMP;
				return;
			}
		}
		
		targetPos = currPathPt;
		pivotHorzDist = (DIRS[turnDir] * RADIUS).Transform(baseMat);
		radiusNormal = Vector3{0_f, RADIUS, 0_f}.Transform(baseMat); //for precision
		if(state == ST_ANTIGRAVITY)
		{
			pivotCenter = (DIRS[turnDir] * RADIUS).Transform(baseMat) + prevPathPt;
			Fix12i forwardDist;
			switch(turnDir)
			{
				case 0: forwardDist =  diff.z; break;
				case 1: forwardDist =  diff.x; break;
				case 2: forwardDist = -diff.z; break;
				case 3: forwardDist = -diff.x; break;
			}
			destTurnAng = Atan2(forwardDist - RADIUS, diff.y);
			if(destTurnAng > -0x4000 && destTurnAng <= 0)
			{
				SetBaseMat();
				if(actorID == 0x22B)
				{
					radiusNormal = Vector3{0_f, RADIUS, 0_f}.Transform(baseMat);
					state = ST_JUMP;
				}
				else
					InitMoveState();
			}
			return;
		}
		else
			destTurnAng = 0x4000;
	}
	--numTurns;
	pivotCenter = (DIRS[turnDir] * RADIUS * Fix12i(-2 * numTurns - 1)).Transform(baseMat) + targetPos;
}

Matrix4x3 ToxBox::GetTurnMat(short ang)
{
	switch(turnDir)
	{
		case 0: return Matrix4x3::FromRotationX( ang);
		case 1: return Matrix4x3::FromRotationZ(-ang);
		case 2: return Matrix4x3::FromRotationX(-ang);
		case 3: return Matrix4x3::FromRotationZ( ang);
	}
}
void ToxBox::SetTurnMat(Matrix4x3& theTurnMat, short ang)
{
	switch(turnDir)
	{
		case 0: theTurnMat.ThisFromRotationX( ang); break;
		case 1: theTurnMat.ThisFromRotationZ(-ang); break;
		case 2: theTurnMat.ThisFromRotationX(-ang); break;
		case 3: theTurnMat.ThisFromRotationZ( ang); break;
	}
}

void ToxBox::SetBaseMat()
{
	baseMat = baseMat * GetTurnMat(destTurnAng - 0x4000);
	baseMatInv = GetTurnMat(-destTurnAng + 0x4000) * baseMatInv;
	orientMat = GetTurnMat(0x4000) * orientMat;
	normal = Vector3{baseMat.r0c1, baseMat.r1c1, baseMat.r2c1};
	radiusNormal = normal * RADIUS;
	pos = targetPos + radiusNormal;
}

void ToxBox::StHitGround()
{
	Earthquake(pos, 0x860000_f);
	Sound::PlayBank3(0x46, camSpacePos);
	state = ST_STOP;
	
	StStop();
}


/*void PrintShort(short val, int x, int y)
{
	int shift = 12;
	for(int i = 0; i < 4; ++i)
	{
		Message::ShowNumber(true, *Message::SpriteRef::NUMBER_PTRS[val >> shift & 0xf], x, y, -1, 1, 0);
		x += 9;
		shift -= 4;
	}
}*/
void ToxBox::StStop()
{
	if(actorID == 0x22B)
	{
		PlayerToxBox* pTox = (PlayerToxBox*)this;
		if(INPUT_ARR[0].buttonsPressed & Input::B)
		{
			prevPathPt = currPathPt;
			InitMoveState();
		}
		else if(INPUT_ARR[0].magnitude > 0x800_f)
		{
			Vector3 moveDir = pTox->downDir * (Fix12i)INPUT_ARR[0].dirZ + pTox->rightDir * (Fix12i)INPUT_ARR[0].dirX;
			//round it to a direction the Tox Box can move in
			short angle = (Atan2(moveDir.Dot(Vector3{baseMat.r0c0, baseMat.r1c0, baseMat.r2c0}), moveDir.Dot(Vector3{baseMat.r0c2, baseMat.r1c2, baseMat.r2c2})) + 0x2000) & 0xc000;
			moveDir = Vector3{baseMat.r0c2, baseMat.r1c2, baseMat.r2c2} * Cos(angle) + Vector3{baseMat.r0c0, baseMat.r1c0, baseMat.r2c0} * Sin(angle);
			
			radiusNormal = normal * RADIUS;
			Vector3 radiusMoveDir = moveDir * RADIUS;
			pivotCenter = radiusMoveDir + currPathPt;
			
			RaycastLine raycast;
			for(int i = 0; i < 3; ++i)
			{
				switch(i)
				{
					case 0:
						raycast.SetObjAndLine(pivotCenter + radiusNormal - moveDir, pivotCenter + radiusMoveDir - normal, this); break;
					case 1:
						raycast.SetObjAndLine(pivotCenter + radiusMoveDir, pivotCenter - radiusNormal - moveDir, this); break;
					case 2:
						raycast.SetObjAndLine(pivotCenter - radiusNormal, pivotCenter - radiusMoveDir, this); break;
				}
				
				if(raycast.DetectClsn())
				{
					Vector3 newFaceDir = raycast.clsnPos - pivotCenter;
					if (raycast.result.clps.Padding() & 2)
					{
						angle = (Atan2(newFaceDir.Dot(moveDir), newFaceDir.Dot(normal)) + 0x800) & 0xf000;
						prevPathPt = currPathPt;
						currPathPt = pivotCenter + radiusNormal * Cos(angle) + radiusMoveDir * Sin(angle);
						InitMoveState();
					}
					break;
				}
			}
		}
	}
	else
	{
		if(stateTimer == 20)
			InitMoveState();
	}
}
//8 frames long

/* Turn directions
           2
           ^
           |
           |
   3 <-----+-----> 1
           |
		   |
		   v
		   0
*/
void ToxBox::Move()
{
	turnAng = (unsigned)(uint16_t)destTurnAng * (stateTimer + 1) / 8;
	SetTurnMat(turnMat, turnAng);
	pos = pivotCenter + radiusNormal * (Sin(turnAng + 0x2000) * SQRT_2) - pivotHorzDist * (Cos(turnAng + 0x2000) * SQRT_2);
	
	if(stateTimer == 7)
	{
		turnAng = 0;
		if(state == ST_ANTIGRAVITY)
		{
			SetBaseMat();
		}
		else
		{
			orientMat = turnMat * orientMat;
			if(numTurns == 0)
				pos = targetPos + radiusNormal;
		}
		state = ST_HIT_GROUND;
		turnMat.LoadIdentity();
	}
}

void ToxBox::StMove()
{
	Move();
}

void ToxBox::StJump()
{
	pos = currPathPt + radiusNormal * (Sin((stateTimer + 1) * 0x800 + 0x2000) * SQRT_2);
	if(stateTimer == 7)
	{
		state = ST_HIT_GROUND;
	}
}

void ToxBox::StAntigravity()
{
	Move();
}

int ToxBox::Behavior()
{
	unsigned prevMoveState = state;
	(this->*states[state].main)();
	++stateTimer;
	if(prevMoveState != state)
		stateTimer = 0;
	
	UpdateModelTransform();
	if(actorID == 0x22B)
	{
		if(!clsn.IsEnabled())
			clsn.Enable(((PlayerToxBox*)this)->player);
		UpdateClsnTransform();
	}
	else if(IsClsnInRange(0_f, 0_f))
		UpdateClsnTransform();
	
	return 1;
}

int ToxBox::Render()
{
	model.Render(nullptr);
	return 1;
}

ToxBox::~ToxBox() {}

////////////////////
// PLAYER TOX BOX //
////////////////////

Player::State PlayerToxBox::ST_TOX_BOX 
{
	&Player::ToxBox_Init,
	&Player::ToxBox_Main,
	&Player::ToxBox_Cleanup
};

Player* PlayerToxBox::playerToPowerUp = nullptr;
PlayerToxBox* PlayerToxBox::playerToxBox;

SpawnInfo<PlayerToxBox> PlayerToxBox::spawnData =
{
	&PlayerToxBox::Spawn,
	0x0030,
	0x0132,
	0x02000002,
	RADIUS,
	0x002ee000_f,
	0x01fa0000_f,
	0x01fa0000_f,
};

PlayerToxBox* PlayerToxBox::Spawn()
{
	return new PlayerToxBox;
}

bool Player::ToxBox_Init()
{
	opacity = 0;
	isTangible = false;
	//TODO: disable the WithMeshClsn
	
	return true;
}

bool Player::ToxBox_Main()
{
	PlayerToxBox& pTox = *PlayerToxBox::playerToxBox;
	pTox.playerClsn.Clear();
	if(pTox.state == ST_HIT_GROUND && pTox.orientMat.r1c1 < 0x800_f)
	{
		pTox.playerClsn.pos = Vector3{pTox.pos.x, pTox.pos.y - RADIUS, pTox.pos.z}; //set absolute position
		pTox.playerClsn.Update();
	}
	return true;
}

bool Player::ToxBox_Cleanup()
{
	if(nextState == (Player::State*)Player::ST_JUMP) //Avoid bouncing off Goombas
		return false;
	
	PlayerToxBox::playerToxBox->shouldDestroy = true; //to avoid dangling cylinder collider pointer
	PlayerToxBox::playerToxBox = nullptr;
	Sound::PlayBank2_2D(22);
	opacity = 0x1f;
	isTangible = true;
	return true;
}

void PlayerToxBox::CalcMoveDirs()
{
	short angToCam = GetAngleToCam(0);
	//rod stands for right, outward, down. Those are the columns of this matrix
	Matrix4x3 rodMat = Matrix4x3::FromRotationXYZExt(0x4000, angToCam, 0);
	/*Vector3 right {Cos(angToCam), 0_f, -Sin(angToCam)};
	Vector3 outward {Sin(angToCam), 0_f, Cos(angToCam)};
	Vector3 down {0_f, -0x1000_f, 0_f};*/
	
	//Rotate the vectors so that outward goes to normal around an axis perpendicular to both
	Vector3 axis = Vector3{rodMat.r0c1, rodMat.r1c1, rodMat.r2c1}.Cross(normal);
	Fix12i cosAngle = Vector3{rodMat.r0c1, rodMat.r1c1, rodMat.r2c1}.Dot(normal);
	Fix12i sinAngle = axis.Len();
	rightDir = Vector3{rodMat.r0c0, rodMat.r1c0, rodMat.r2c0} * cosAngle + axis.Cross(Vector3{rodMat.r0c0, rodMat.r1c0, rodMat.r2c0}) * sinAngle + axis * axis.Dot(Vector3{rodMat.r0c0, rodMat.r1c0, rodMat.r2c0}) * (0x1000_f - cosAngle);
	downDir = rightDir.Cross(normal);
}

int PlayerToxBox::InitResources()
{
	char* modelF = Model::LoadFile(PlayerToxBox::modelFile);
	model.SetFile(modelF, 1, -1);
	
	TextureSequence::LoadFile(PlayerToxBox::texSeqFile);
	TextureSequence::Prepare(PlayerToxBox::modelFile.filePtr, PlayerToxBox::texSeqFile.filePtr);
	texSeq.SetFile(PlayerToxBox::texSeqFile.filePtr, Animation::NO_LOOP, 0x1000_f, param1 >> 8 & 3);
	
	currPathPt = pos;
	
	if(!InitCommonResources())
		return 0;
	
	player = PlayerToxBox::playerToPowerUp;
	player->ChangeState(PlayerToxBox::ST_TOX_BOX);
	playerToxBox = this;
	playerClsn.Init(player, Vector3{pos.x, pos.y - RADIUS, pos.z}, SQRT_SQRT_2 * RADIUS, 2 * RADIUS,
		CylinderClsn::HIT_BY_CHAR_BODY | //TY_CLIMBABLE
		CylinderClsn::HIT_BY_MEGA_CHAR |
		CylinderClsn::HIT_BY_PLAYER,
		//
		CylinderClsn::HIT_BY_EXPLOSION | //TY_EXPLOSION
		CylinderClsn::HIT_BY_ENEMY | //TY_ENEMY (21)
		CylinderClsn::HIT_BY_COLLECTABLE); //TY_COLLECTABLE
	
	PoofDustAt(pos);
	
	uniqueID = player->uniqueID; //let's RIG this box
	
	return 1;
}

int PlayerToxBox::CleanupResources()
{
	clsn.Disable();
	PlayerToxBox::modelFile.Release();
	PlayerToxBox::texSeqFile.Release();
	return 1;
}

int PlayerToxBox::Behavior()
{
	if(shouldDestroy)
	{
		Destroy();
		return 1;
	}
	
	CalcMoveDirs();
	/*PrintShort(downDir.x.val, 0, 32);
	PrintShort(downDir.y.val, 0, 48);
	PrintShort(downDir.z.val, 0, 64);
	
	PrintShort(rightDir.x.val, 128, 32);
	PrintShort(rightDir.y.val, 128, 48);
	PrintShort(rightDir.z.val, 128, 64);*/
	ToxBox::Behavior();
	player->pos = pos;
	player->prevPos = pos;
	player->horzSpeed = 0_f;
	player->speed.y = 0_f;
	
	if(INPUT_ARR[0].buttonsPressed & Input::R)
		player->ChangeState(*(Player::State*)Player::ST_FALL);
	return 1;
}

int PlayerToxBox::Render()
{
	texSeq.Update(model.data);
	model.Render(nullptr);
	return 1;
}

void PlayerToxBox::Virtual30()
{
	Sound::PlayBank3(0x20, camSpacePos);
	DisappearPoofDustAt(pos);
}

PlayerToxBox::~PlayerToxBox() {}