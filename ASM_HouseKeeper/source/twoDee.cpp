#include "twoDee.h"

//Actor IDs.
constexpr short int TWO_DEE_CAMERA_LIMITER = 0x0207;

bool twoDeeOn = false;

namespace twoDee
{
	uint16_t savedFOV;
	Vector3 savedPos;
	Vector3 savedLookAt;
	Vector3 playerPos2d;
	bool needsDisable;
	Fix12i dist;
	uint16_t fov;
	bool isFront;
}

void twoDee::Setup(Actor* actor) {
	dist = actor->param1 * 0x10000_f;
	fov = actor->ang.x;
	isFront = actor->ang.z == 0;
	if (fov == 0) {
		fov = 0x30;
	}
	if (actor->param1 == 0) {
		dist = 0x24000000_f;
	}
	Enable();
}

void twoDee::Update()
{
	if (twoDeeOff) {return;}
	if (CAMERA == nullptr) {
		return;
	}
	if (needsDisable) {
		CAMERA->fov = savedFOV;
		CAMERA->pos = PLAYER_ARR[0]->pos;
		CAMERA->pos.z -= 1000_f;
		CAMERA->lookAt = savedLookAt;
		twoDeeOn = false;
		needsDisable = false;
		return;
	}

	static short angle = 0x4000;

	if ((unsigned)(PLAYER_ARR[0]->currState) == Player::ST_WALK || (unsigned)(PLAYER_ARR[0]->currState) == Player::ST_WAIT)
	{
		if (INPUT_PERSISTENT & Input::LEFT)
		{
			angle = isFront ? 0xc000 : 0x4000;
		}
		else if (INPUT_PERSISTENT & Input::RIGHT)
		{
			angle = isFront ? 0x4000 : 0xc000;
		}
		PLAYER_ARR[0]->ang.y = angle;
	}

	Fix12i camX = PLAYER_ARR[0]->pos.x;
	Fix12i camY = PLAYER_ARR[0]->pos.y;

	Actor* actor = nullptr;
	while (true) {
		actor = actor->Next();
		if (!actor) break;
		actor->pos.z = playerPos2d.z;

		//Camera limiter object.
		if (actor->actorID == TWO_DEE_CAMERA_LIMITER) {
			switch (actor->param1) {

				//Upper.
				case 0:
					if (camY > actor->pos.y) {
						camY = actor->pos.y;
					}
					break;

				//Downer.
				case 1:
					if (camY < actor->pos.y) {
						camY = actor->pos.y;
					}
					break;

				//Lefto.
				case 2:
					if (camX < actor->pos.x) {
						camX = actor->pos.x;
					}
					break;

				//Rightkowski.
				case 3:
					if (camX > actor->pos.x) {
						camX = actor->pos.x;
					}
					break;
			}

		}

	}

	CAMERA->lookAt.x = CAMERA->pos.x = camX;
	CAMERA->lookAt.y = CAMERA->pos.y = camY;
	CAMERA->lookAt.z = CAMERA->pos.z = PLAYER_ARR[0]->pos.z;
	CAMERA->pos.z += dist * (isFront ? 1 : -1);
	CAMERA->fov = fov;		
}

void twoDee::Enable()
{
	if (twoDeeOn) return;

	savedFOV = CAMERA->fov;
	savedPos = CAMERA->pos;
	savedLookAt = CAMERA->lookAt;
	playerPos2d = PLAYER_ARR[0]->pos;
	twoDeeOn = true;
}

void twoDee::Disable()
{
	if (twoDeeOff) return;
	needsDisable = true;
}

Vector3* camSpacePos;
Vector3 camSpacePos2;

void twoDee::UpdateCamSpacePos()
{
	camSpacePos2 = Vector3{camSpacePos->x, camSpacePos->y, twoDeeOn ? 0x0_f : camSpacePos->z};
}

// Ignore the Z coordinate of camSpacePos in 4 sound player functions:

// PlayCharVoice
void hook_02012160()
{
	asm
	(
		"ldr	r0, =camSpacePos		\n\t"
		"str	r2, [r0]				\n\t"
	);
	twoDee::UpdateCamSpacePos();
}

// PlayLong
void hook_0201232c()
{
	asm
	(
		"ldr	r0, =camSpacePos		\n\t"
		"str	r3, [r0]				\n\t"
	);
	twoDee::UpdateCamSpacePos();
}

// Play
void hook_02012594()
{
	asm
	(
		"ldr	r0, =camSpacePos		\n\t"
		"str	r2, [r0]				\n\t"
	);
	twoDee::UpdateCamSpacePos();
}

// PlayBank0
void hook_02012650()
{
	asm
	(
		"ldr	r0, =camSpacePos		\n\t"
		"str	r1, [r0]				\n\t"
	);
	twoDee::UpdateCamSpacePos();
}

// There are more hooks in twoDeeASM.s
