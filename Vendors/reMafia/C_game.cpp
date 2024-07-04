#include "C_game.h"

void C_game::Done() {
    ((void(__thiscall*)(C_game*))(0x5A3C60))(this);
}

bool C_game::Init() {
    return ((bool(__thiscall*)(C_game*))(0x5A0810))(this);
}

void C_game::AddTemporaryActor(C_actor* pActor) {
	 ((void(__thiscall*)(C_game*, C_actor*))(0x5A77C0))(this, pActor);
}

void C_game::RemoveTemporaryActor(C_actor* pActor) { 
    ((void(__thiscall*)(C_game*, C_actor*))(0x5A79A0))(this, pActor);
}
