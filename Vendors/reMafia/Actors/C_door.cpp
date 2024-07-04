#include "C_door.h"
#include "C_door.h"
#include <g_collision.h>

C_door::C_door() {
	C_actor::C_actor();
    memset(&m_sCollObject, 0, sizeof(tDynamicCollObject));
	memset(&m_sUsingObject, 0, sizeof(C_using_object::S_using_object));
    m_fCloseSpeed          = 0.004f;
    m_fOpenSpeed           = 0.004f;
    m_eActorType           = ACT_Door;
    m_bOpenInBothDirs      = true;
    m_uForcedOpenDirection = 0;
    m_fMaxAngle            = PI / 2.0f;
    m_bLocked              = false;
    m_bOpened              = false;
    m_uUnk                 = 0;
    m_uFlags               = 0;
    strcpy(m_szOpenSoundName, "d_woodop.wav");
    strcpy(m_szCloseSoundName, "d_woodcl.wav");
    strcpy(m_szLockedSoundName, "d_woodlo.wav");
    m_uUnk2 = 0;
}

void C_door::AI(uint32_t uFrameTime) {
    ((void(__thiscall*)(C_door*, int32_t))(0x4399C0))(this, uFrameTime);
}

void C_door::NetDirect(uint32_t uFrameTime) {
	((void(__thiscall*)(C_door*, int32_t))(0x439E90))(this, uFrameTime);
}

C_door::~C_door() { 
	
}

bool C_door::SaveData(C_chunk* pChunk) {
    return ((bool(__thiscall*)(C_door*, C_chunk*))(0x43AE50))(this, pChunk);
}

bool C_door::LoadData(C_chunk* pChunk) {
    return ((bool(__thiscall*)(C_door*, C_chunk*))(0x43B030))(this, pChunk);
}

void C_door::DuplicateFrom(C_actor* pOther) {
    ((void(__thiscall*)(C_door*, C_actor*))(0x4393BF))(this, pOther);
}

int32_t C_door::SaveGameGetSize() {
    return ((int32_t(__thiscall*)(C_door*))(0x43B4F0))(this);
}

bool C_door::SaveGameSave(void** pBuf) {
    return ((bool(__thiscall*)(C_door*, void**))(0x43B500))(this, pBuf);
}

bool C_door::SaveGameLoad(void** pBuf) {
    return ((bool(__thiscall*)(C_door*, void**))(0x43B560))(this, pBuf);
}

void C_door::GameInit() {
    ((void(__thiscall*)(C_door*))(0x439EB0))(this);
}

void C_door::GameDone() {
    ((void(__thiscall*)(C_door*))(0x43AD60))(this);
}

void C_door::ChangeState() {
    ((void(__thiscall*)(C_door*))(0x43BCC0))(this);
}

bool C_door::Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) {
	return ((bool(__thiscall*)(C_door*, E_hit_type, S_vector const&, S_vector const&, S_vector const&, float, C_actor*, uint32_t, I3D_frame*))(0x439480))(this, eHitType, vUnk1, vUnk2, vUnk3, fDamage, pAttacker, uUnk, pFrame);
}

bool C_door::cbCollTester(tDynamicCollObject* pDynamicCollObj) {
	return ((bool(__thiscall*)(C_door*, tDynamicCollObject*))(0x4075A0))(this, pDynamicCollObj);
}

void C_door::SetDoorState(C_door::E_state doorState, C_actor* actor, bool a4, bool a5) {
    ((void(__thiscall*)(C_door*, C_door::E_state, C_actor*, bool, bool))(0x439610))(this, doorState, actor, a4, a5);
}
