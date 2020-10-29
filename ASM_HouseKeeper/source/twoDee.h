#ifndef TWODEE_INCLUDED
#define TWODEE_INCLUDED

#include "SM64DS_2.h"

extern bool twoDeeOn;
#define twoDeeOff (!twoDeeOn)

namespace twoDee
{

	void Setup(Actor* actor);

	void Update();

	void Enable();
	void Disable();

	void UpdateCamSpacePos();

	extern uint16_t savedFOV;
	extern Vector3 savedPos;
	extern Vector3 savedLookAt;
	extern Vector3 playerPos2d;
	extern Fix12i dist;
	extern uint16_t fov;
	extern bool isFront;
}

#endif
