#include "SM64DS_2.h"
#include "MOM_Objects.h"

asm(R"(
nsub_020b0cc4_ov_02:
    bne  0x020b0cc8
    mov  r0, r6
    bl   FindCustomPipe
    movs r7, r0
    beq  0x020b0cd4
    b    0x020b0cc8
)");

extern "C" Actor* FindCustomPipe(const Actor& exit)
{
    Actor* customPipe = nullptr;
    while ((customPipe = Actor::FindWithActorID(CustomObject::COLORED_PIPE, customPipe)) && customPipe->pos.Dist(exit.pos) >= 300._f);
    return customPipe;
}








	/*Actor* customPipe = FindWithActorID(537, 0);

	while (customPipe != nullptr)
	{
		if (Vec3_Dist(customPipe->pos, exit.pos) < 300._f) return customPipe;

		customPipe = FindWithActorID(537, customPipe);
	}
	
	return customPipe;*/