#ifndef SM64DS_INPUT_INCLUDED
#define SM64DS_INPUT_INCLUDED

#include "SM64DS_Common.h"


struct Input
{
	enum Buttons : uint16_t{
		A = 1 << 0,
		B = 1 << 1,
		SELECT = 1 << 2,
		START = 1 << 3,
		RIGHT = 1 << 4,
		LEFT = 1 << 5,
		UP = 1 << 6,
		DOWN = 1 << 7,
		CAM_RIGHT = 1 << 8,
		CAM_LEFT = 1 << 9,
		R = 1 << 10,
		Y = 1 << 11,
		L = 1 << 14,
		X = 1 << 15
	};

	Fix12s magRelated0;
	Fix12s magRelated1;
	uint16_t buttonsHeld;
	uint16_t buttonsPressed;
	Fix12s magnitude;
	Fix12s dirX;
	Fix12s dirZ;
	uint16_t angle; //0x0000 is toward the camera, 0x4000 is right
	uint8_t touchscreenX; //0 is at the left
	uint8_t touchscreenY; //0 is at the top
	uint8_t touchscreenDelay;
	uint8_t touchOffDelay;
	bool touchscreenBeingUsed;
	uint8_t unk15;
	uint16_t unk16;

};

extern "C"
{
	extern Input INPUT_ARR[4];
}

#endif // SM64DS_INPUT_INCLUDED
