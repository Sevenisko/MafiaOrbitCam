#pragma once
#include "C_actor.h"
#include "g_collision.h"
#include "C_using_object.h"

enum E_door_flags {
	DOOR_FORCED_OPEN_DIRECTION = 0x10,
	DOOR_LOCKED = 0x20,
};

class C_door : public C_actor {
public:
    enum E_state {
        DS_OPENED, 
        DS_CLOSED,
        DS_OPENING,
        DS_CLOSING,
    };

    C_door();

    void AI(uint32_t iFrameTime) override;
    void NetDirect(uint32_t iFrameTime) override;
    virtual ~C_door();
    bool SaveData(C_chunk* pChunk) override;
    bool LoadData(C_chunk* pChunk) override;
    void DuplicateFrom(C_actor* pOther) override;
    int32_t SaveGameGetSize() override;
    bool SaveGameSave(void** pBuf) override;
    bool SaveGameLoad(void** pBuf) override;
    void GameInit() override;
    void GameDone() override;
    void ChangeState() override;
    bool Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) override;
    bool cbCollTester(tDynamicCollObject* pDynamicCollObj) override;

    void SetDoorState(C_door::E_state doorState, C_actor* actor, bool a4, bool a5);
private:
	E_state m_eDoorState;
	float m_fAngle;
	tDynamicCollObject m_sCollObject;
	S_matrix m_mDoorMatrix;
	S_quat m_qUnk;
	uint32_t m_uFlags;
	uint8_t pad5[4];
	C_door *m_pNextDoor;
	uint8_t m_uUnk;
	bool m_bOpenInBothDirs;
	uint8_t m_uForcedOpenDirection;
	uint8_t pad6_2;
	float m_fMaxAngle;
	bool m_bOpened;
	bool m_bLocked;
	float m_fOpenSpeed;
	float m_fCloseSpeed;
	char m_szOpenSoundName[16];
	char m_szCloseSoundName[16];
	char m_szLockedSoundName[16];
	uint8_t pad10[4];
	uint8_t m_uUnk2;
	uint8_t pad11[15];
	S_vector m_vUnk1;
	C_using_object::S_using_object m_sUsingObject;
	C_using_object::S_using_object** m_ppUsingObject;
	friend class C_debug;
};

static char* szDoorStateNames[] = { "Opened", "Closed", "Opening", "Closing" };