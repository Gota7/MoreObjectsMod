#include "SM64DS_2.h"
#include "twoDee.h"
#include "GravityModifier.h"

//Actor IDs.
constexpr short int TWO_DEE_LEVEL_ID = 0x0206;
constexpr short int GRAVITY_MODIFIER = 0x0214;

//If MOM has been initialized.
static bool init = false;

//Initialization code.
void Initialize()
{

	//Load the MOM overlay.
	FS_LoadOverlay(false, 155);

}

//Manage modes.
void ManageModes() 
{

	Actor* actor = nullptr;
	bool found2D = false;
	bool foundGM = false;
	while (true) {
		actor = actor->Next();
		if (!actor) break;

		//Gravity modifier object.
		if (!foundGM && actor->actorID == GRAVITY_MODIFIER) {
			ChangeGravity(actor->param1, actor->ang.x == 1);
			foundGM = true;
			continue;
		}

		//2D object.
		if (!found2D && actor->actorID == TWO_DEE_LEVEL_ID) {
			twoDee::dist = actor->param1 * 0x10000_f;
			twoDee::fov = actor->ang.x;
			twoDee::isFront = actor->ang.z == 0;
			if (twoDee::fov == 0) {
				twoDee::fov = 0x30;
			}
			if (actor->param1 == 0) {
				twoDee::dist = 0x24000000_f;
			}
			found2D = true;
			continue;
		}

	}
	if (found2D) {
		twoDee::Enable();
	} else {
		twoDee::Disable();
	}
	if (!foundGM) {
		ChangeGravity(0x1000, false);
	}

}

//Hooks every frame.
void hook_0200da0c()
{
	
	//Initialize if needed.
	if (!init) {
		Initialize();
		init = true;
	}

	//2D code.
	twoDee::Update();

}

//Hooks when course is changed.
void hook_0202D494()
{
	
	//Manage modes.
	ManageModes();

}