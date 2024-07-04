#pragma once
#include "C_actor.h"
#include "../g_collision.h"

class I3D_sound;
struct S_THING_BOX { };
struct S_THING_WHEEL { };

#pragma pack(push, 1)

class C_PhThing : public C_actor {
public:
    // ---- C_PhThing [VTABLE section] ---- 
	C_PhThing();
    void AI(uint32_t uFrameTime);
    void Update(uint32_t uFrameTime);
    bool Init(I3D_frame* pFrame);
    int32_t SaveGameGetSize();
    bool SaveGameSave(void** pBuf);
    bool SaveGameLoad(void** pBuf);
    void GameInit();
    void GameDone();
    void ChangeState();
    bool Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pOtherFrame);
    bool Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame);
    // ---- C_PhThing [private & public __thiscall] ----
	void InitBox(S_THING_BOX* pBox);
	void InitWheel(S_THING_WHEEL* pWheel);
	void ExternalImpuls(const S_vector& vForce, const S_vector& vOffset);
protected:
	int m_eType;
	uint8_t ph_pad1;
	bool m_bUnkBool1;
	bool m_bUnkBool2;
	bool m_bUnkBool3;
	S_vector m_vUnkVec1;
	S_vector m_vUnkVec2;
	S_vector m_vUnkVec3WorldPos;
	int ph_pad1_1;
	S_vector m_vWorldRight;
	S_vector m_vWorldUp;
	tDynamicCollObject m_sDynamicCollObject;
	S_vector m_vUnkVec4;
	S_matrix m_mWorldMatrix;
	S_matrix m_mInverseWorldMatrix;
	S_matrix m_mInverseWorldMatrix2ZeroOrigin;
	S_matrix m_mInverseWorldMatrix3ZeroOrigin;
	float m_fUnkFloat;
	float m_fWeight;
	S_vector m_vVelocity;
	S_vector m_vUnk1;
	S_vector m_vMeybeAngularVelocity;
	float m_fUnkFloat2;
	S_vector m_vUnkVec5;
	S_vector m_vUnkVec6;
	float m_fGravity;
	float m_fUnkFloat4;
	float m_fUnkFloat5;
	float m_fUnkFloat3;
	uint8_t pad[212];
	I3D_sound *m_pSound;
	S_vector m_vMeybeExternalForce;
	S_vector mv_UnkVec6;
	uint8_t ph_pad5[36];
	bool m_bUnkBool;
	uint8_t m_bUnkBool__pad[3];
	bool m_bUnkBool5;
	uint8_t m_bUnkBool5__pad[3];
	S_vector m_vUnkVec7;
}; // 904bytes

#pragma pack(pop)
//constexpr auto test = sizeof(C_PhThing);