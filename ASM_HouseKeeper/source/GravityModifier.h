#ifndef GRAVITYMODIFIER_INCLUDED
#define GRAVITYMODIFIER_INCLUDED

#include "SM64DS_2.h"

Fix12i gravityMultiplier;

//Change gravity.
void ChangeGravity(uint16_t scale, bool isUp) {
	gravityMultiplier = 0x1_f * scale;
	if (isUp) { gravityMultiplier = -gravityMultiplier; }
}

#endif
