#include "SM64DS_2.h"
#include "twoDee.h"

//Actor IDs.
constexpr short int TWO_DEE_LEVEL_ID = 0x0206;

//If MOM has been initialized.
static bool init = false;

//Initialization code.
void Initialize()
{

	//Load the MOM overlay.
	FS_LoadOverlay(false, 155);

}

//Manage 2D mode.
void Manage2D() 
{

	Actor* actor = nullptr;
	bool found = false;
	while (!found) {
		actor = actor->Next();
		if (!actor) break;

		//2D object.
		if (actor->actorID == TWO_DEE_LEVEL_ID) {
			twoDee::dist = actor->param1 * 0x10000_f;
			twoDee::fov = actor->ang.x;
			twoDee::isFront = actor->ang.z == 0;
			if (twoDee::fov == 0) {
				twoDee::fov = 0x30;
			}
			if (actor->param1 == 0) {
				twoDee::dist = 0x24000000_f;
			}
			found = true;
		}

	}
	if (found) {
		twoDee::Enable();
	} else {
		twoDee::Disable();
	}

}

//Hooks every frame.
void hook_0200da0c()
{
	
	//Initialize if needed.
	if (!init) {
		Initialize();
	}

	//2D code.
	twoDee::Update();

}

//Hooks when course is changed.
void hook_0202D494()
{
	
	//Manage 2D.
	Manage2D();

}