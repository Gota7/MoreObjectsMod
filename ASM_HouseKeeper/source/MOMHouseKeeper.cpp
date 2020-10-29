#include "MOMHouseKeeper.h"
#include "SM64DS_2.h"
#include "twoDee.h"
#include "GravityModifier.h"

//Actor IDs.
constexpr short int TWO_DEE_LEVEL_ID = 0x0206;
constexpr short int GRAVITY_MODIFIER_ID = 0x0214;

//If MOM has been initialized.
static bool init = false;

//Objects.
constexpr int NUM_OBJECTS = 2;
static HouseKeeperObject Objects[NUM_OBJECTS];

//Initialization code.
void Initialize()
{

	//Load the MOM overlay.
	FS_LoadOverlay(false, 155);

	//2d Level. Camera code is included with it.
	Objects[0].actorID = TWO_DEE_LEVEL_ID;
	Objects[0].setupFunc = &twoDee::Setup;
	Objects[0].cleanupFunc = &twoDee::Disable;

	//Gravity modifier.
	Objects[1].actorID = GRAVITY_MODIFIER_ID;
	Objects[1].setupFunc = &GravitySetup;
	Objects[1].cleanupFunc = &GravityCleanup;

}

//Manage modes.
void ManageModes() 
{

	Actor* actor = nullptr;
	for (int i = 0; i < NUM_OBJECTS; i++) {
		Objects[i].foundInLevel = false;
	}
	while (true) {
		actor = actor->Next();
		if (!actor) break;

		//Go through objects.
		for (int i = 0; i < NUM_OBJECTS; i++) {
			if (!Objects[i].foundInLevel && actor->actorID == Objects[i].actorID) {
				Objects[i].foundInLevel = true;
				Objects[i].setupFunc(actor);
			}
		}

	}
	for (int i = 0; i < NUM_OBJECTS; i++) {
		if (!Objects[i].foundInLevel) {
			Objects[i].cleanupFunc();
		}
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