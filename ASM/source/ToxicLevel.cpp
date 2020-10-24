#include "ToxicLevel.h"

SpawnInfo<ToxicLevel> ToxicLevel::spawnData =
{
	&ToxicLevel::Spawn,
	0x0004,
	0x0007,
	0x00000000,
	0x00000000_f,
	0x7fffffff_f,
	0x7fffffff_f,
	0x7fffffff_f
};

ToxicLevel* ToxicLevel::Spawn()
{
	return new ToxicLevel;
}

int ToxicLevel::InitResources()
{
	frameForDamage = param1;
	return 1;
}

int ToxicLevel::CleanupResources()
{
	return 1;
}

int ToxicLevel::Behavior()
{						
	if (PAUSE == 0) {

			if (toxicCounter >= frameForDamage) {

				if (PLAYER_HEALTH > 1) { PLAYER_HEALTH--; }

				else if (PLAYER_HEALTH == 1) {

					PLAYER_ARR[0]->Hurt(PLAYER_ARR[0]->pos, 1, 0xc000_f, 1, 0, 1);

				}

				toxicCounter = 0;

			} else if (PLAYER_ARR[0]->GetTalkState() != Player::TK_TALKING) {

				toxicCounter++;

			}

		} 
	return 1;
}

int ToxicLevel::Render()
{
	return 1;
}

ToxicLevel::~ToxicLevel() {}