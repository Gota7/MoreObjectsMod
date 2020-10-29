#ifndef GRAVITYMODIFIER_INCLUDED
#define GRAVITYMODIFIER_INCLUDED

#include "SM64DS_2.h"

Fix12i gravityMultiplier;

//Change gravity.
void ChangeGravity(uint16_t scale, bool isUp) {
	gravityMultiplier = 0x1_f * scale;
	if (isUp) { gravityMultiplier = -gravityMultiplier; }
}

//Setup gravity.
void GravitySetup(Actor* actor) {
	ChangeGravity(actor->param1, (actor->ang.x & 0xFF) == 1);
	uint16_t mulAmount = ((actor->ang.x & 0xFF00) >> 8);
	Fix12i multiplier = 0x1_f;
	uint16_t bigAmount = ((actor->ang.z & 0xFF00) >> 8);
	uint16_t smallAmount = actor->ang.z & 0xFF;
	for (uint16_t i = 0; i < mulAmount; i++) {
		multiplier *= 0x10000_f;
	}
	if (bigAmount == 0) {
		FALL_DAMAGE_BIG = 0xBB8000_f;
	} else if (bigAmount = 0xFF) {
		FALL_DAMAGE_BIG = 0x7FFFFFFF_f;
	} else {
		FALL_DAMAGE_BIG = multiplier * bigAmount;
	}
	if (smallAmount == 0) {
		FALL_DAMAGE_SMALL = 0x47E000_f;
	} else if (smallAmount == 0xFF) {
		FALL_DAMAGE_SMALL = 0x7FFFFFFF_f;
	} else {
		FALL_DAMAGE_SMALL = multiplier * smallAmount;
	}
}

//Make back to normal.
void GravityCleanup() {
	ChangeGravity(0x1000, false);
	FALL_DAMAGE_BIG = 0xBB8000_f;
	FALL_DAMAGE_SMALL = 0x47E000_f;
}

#endif
