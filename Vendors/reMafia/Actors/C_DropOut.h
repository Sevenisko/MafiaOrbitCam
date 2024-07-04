#pragma once
#include "C_PhThing.h"

#pragma pack(push, 1)

class C_DropOut : public C_PhThing {
public:
    C_DropOut();
	void AI(uint32_t uFrameTime);
    void Update(uint32_t uFrameTime);
	virtual ~C_DropOut();
    bool Init(I3D_frame* pFrame);
	void GameInit();
    void GameDone();
    void ChangeState();
	bool Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pFrame);
    bool Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame);

	C_DropOut* Constructor() {
		this->C_DropOut::C_DropOut();
		return this;
	}
private:
    uint32_t m_iUnk;
    bool m_IsInactive;
	uint8_t m_IsInactive__pad[3];
}; //916 bytes

#pragma pack(pop)

//constexpr auto testSize = sizeof(C_DropOut)
