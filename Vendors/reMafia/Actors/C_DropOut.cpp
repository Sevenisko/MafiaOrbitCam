#include "C_DropOut.h"

C_DropOut::C_DropOut(){
	C_PhThing::C_PhThing();
	m_eActorType = ACT_DropOut;
	m_iUnk = 0;
}

C_DropOut::~C_DropOut(){
}

void C_DropOut::AI(uint32_t uFrameTime) {
	((void(__thiscall*)(C_DropOut*, int32_t))(0x00443D80))(this, uFrameTime);
}

void C_DropOut::Update(uint32_t uFrameTime) {
	((void(__thiscall*)(C_DropOut*, int32_t))(0x00443DA0))(this, uFrameTime);
}

bool C_DropOut::Init(I3D_frame* pFrame) {
	return ((bool(__thiscall*)(C_DropOut*, I3D_frame*))(0x00443B30))(this, pFrame);
}

void C_DropOut::GameInit() {
	((void(__thiscall*)(C_DropOut*))(0x00443D10))(this);   
}

void C_DropOut::GameDone() {
	((void(__thiscall*)(C_DropOut*))(0x00443D30))(this);   
}

void C_DropOut::ChangeState() {
	((void(__thiscall*)(C_DropOut*))(0x00443DC0))(this);   
}

bool C_DropOut::Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pFrame) {
	if (m_IsInactive) {
        SetActState(MOVING);
	} else {
       C_PhThing::Collision(pActor, vUnk1, vUnk2, vUnk3, fUnk, pFrame);
	}
	return true;
}

bool C_DropOut::Hit(E_hit_type eHitType, S_vector const& vDir, S_vector const& vWorldPos, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) {
	if (m_IsInactive) {
		SetActState(MOVING);
    } else {
		C_PhThing::Hit(eHitType, vDir, vWorldPos, vUnk3, fDamage, pAttacker, uUnk, pFrame);
    }
	return true;
}

