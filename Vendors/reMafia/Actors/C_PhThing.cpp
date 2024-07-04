#include "C_PhThing.h"

 // ---- C_PhThing [VTABLE section] ---- 

C_PhThing::C_PhThing() {	
  C_actor::C_actor();
  memset(&this->m_sDynamicCollObject, 0, sizeof(this->m_sDynamicCollObject));
  this->m_bUnkBool5 = 0;
  this->m_vUnkVec7.x = 0.0;
  this->m_vUnkVec7.y = 0.0;
  this->m_vUnkVec7.z = 0.0;
  this->m_eType = 0;
  this->m_fUnkFloat5 = 0.0;
  this->m_fUnkFloat3 = 0.0;
  this->m_fGravity = 10.0;
  this->m_fUnkFloat = 0.028000001f;
  this->m_vUnk1.x = 0.0;
  this->m_vUnk1.y = 0.0;
  this->m_vUnk1.z = 0.0;
  this->m_vMeybeAngularVelocity.x = 0.0;
  this->m_vMeybeAngularVelocity.y = 0.0;
  this->m_vMeybeAngularVelocity.z = 0.0;
  this->m_vUnkVec4.x = 0.0;
  this->m_bUnkBool2 = 0;
  this->m_vUnkVec4.y = 1.0;
  this->m_vUnkVec4.z = 0.0;
  //this->mv_UnkVec6.x = NAN;
  //this->mv_UnkVec6.y = NAN;
  this->m_vUnkVec1.x = 0.0;
  this->m_vUnkVec1.y = 0.0;
  this->m_vUnkVec1.z = 0.0;
  this->m_bUnkBool = 1;
}

void C_PhThing::AI(uint32_t uFrameTime) {
	((void(__thiscall*)(C_PhThing*, int32_t))(0x0045A690))(this, uFrameTime);
}

void C_PhThing::Update(uint32_t uFrameTime) {
    ((void(__thiscall*)(C_PhThing*, int32_t))(0x0045A7A0))(this, uFrameTime);
}

bool C_PhThing::Init(I3D_frame* pFrame) {
    return ((bool(__thiscall*)(C_PhThing*, I3D_frame*))(0x00567540))(this, pFrame);
}

int32_t C_PhThing::SaveGameGetSize() {
    return C_actor::SaveGameGetSize() + 211;
}

bool C_PhThing::SaveGameSave(void** pBuf) {
    return ((bool(__thiscall*)(C_PhThing*, void**))(0x004603C0))(this, pBuf);
}

bool C_PhThing::SaveGameLoad(void** pBuf) {
    return ((bool(__thiscall*)(C_PhThing*, void**))(0x004605A0))(this, pBuf);
}

void C_PhThing::GameInit() {
    ((void(__thiscall*)(C_PhThing*))(0x0045A120))(this);
}

void C_PhThing::GameDone() {
    ((void(__thiscall*)(C_PhThing*))(0x0045A630))(this);
}

void C_PhThing::ChangeState() {
    ((void(__thiscall*)(C_PhThing*))(0x0045ADC0))(this);
}

bool C_PhThing::Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pOtherFrame) {
    return ((bool(__thiscall*)(C_PhThing*, C_actor*, S_vector const&, S_vector const&, S_vector const&, float, I3D_frame*))(0x0045F520))(this, pActor, vUnk1, vUnk2, vUnk3, fUnk, pOtherFrame);
}

bool C_PhThing::Hit(E_hit_type eHitType, S_vector const& vDir, S_vector const& vWorldPos, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) {
	return ((bool(__thiscall*)(C_PhThing*, E_hit_type, S_vector const&, S_vector const&, S_vector const&, float, C_actor*, uint32_t, I3D_frame*))(0x0045F760))(this, eHitType, vDir, vWorldPos, vUnk3, fDamage, pAttacker, uUnk, pFrame);
}

// ---- C_PhThing [private & public __thiscall] ----

void C_PhThing::InitBox(S_THING_BOX* pBox) {
	((void(__thiscall*)(C_PhThing*, S_THING_BOX*))(0x0045EE90))(this, pBox);
}

void C_PhThing::InitWheel(S_THING_WHEEL* pWheel) {
	((void(__thiscall*)(C_PhThing*, S_THING_WHEEL*))(0x0045ED30))(this, pWheel);
}

void C_PhThing::ExternalImpuls(const S_vector& vForce, const S_vector& vOffset) {
	((void(__thiscall*)(C_PhThing*, const S_vector&, const S_vector&))(0x00460000))(this, vForce, vOffset);
}