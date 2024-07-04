#pragma once
#include "C_PhThing.h"

#pragma pack(push, 1)

class C_box_new : public C_PhThing {
public:
    C_box_new();
	virtual ~C_box_new() {}
    bool Init(I3D_frame* pFrame);
    bool SaveData(C_chunk* pChunk);
    bool LoadData(C_chunk* pChunk);
    void DuplicateFrom(C_actor* pOther);
    void GameInit();
    bool Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pFrame);
    bool Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame);
public:
	bool m_bIsBox;
	uint8_t m_bIsBox__pad[3];
	float m_fUnk0;
	float m_fUnk1;
	float m_fMass;
	float m_fFriction;
	float m_fUnk4;
	bool m_bWasHitted;
	bool m_bUnk2;
	uint8_t m_bUnk2__pad[2];
}; //936 bytes

#pragma pack(pop)
//constexpr auto testSize = offsetof(C_box_new, m_bWasHitted);