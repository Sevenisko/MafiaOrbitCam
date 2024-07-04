#include <LS3D.h>
#include <Utils.h>

#include "C_mission.h"
#include "C_game.h"
#include "C_program.h"
#include "Actors/C_human.h"
#include "Actors/C_player.h"

#include "Actors/C_DropOut.h"
#include "Actors/C_actor.h"
#include "Actors/C_box_new.h"
#include "Actors/C_clock.h"
#include "Actors/C_door.h"
#include "Actors/C_public_physics.h"
#include "Actors/C_traffic_generator.h"

void C_mission::Close() {
    ((void(__thiscall*)(C_mission*))(0x5405E0))(this);
}

bool C_mission::Open(const char* szName, bool bUnk1, uint32_t uUnk, bool bUnk2) {
    return ((bool(__thiscall*)(C_mission*, const char*, bool, uint32_t, bool))(0x5409D0))(this, szName, bUnk1, uUnk, bUnk2);
}

void C_mission::AddActor(C_actor* pActor, bool bUnk) {
    ((void(__thiscall*)(C_mission*, C_actor*, bool))(0x53FEA0))(this, pActor, bUnk);
}

C_actor* C_mission::CreateActor(EActorType eType) {
    return ((C_actor * (__thiscall*)(C_mission*, EActorType))(0x53F7D0))(this, eType);
}