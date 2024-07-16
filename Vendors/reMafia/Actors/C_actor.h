#pragma once

#include <LS3D.h>
#include <I3D_frame.h>
#include <vector.h>
#include "common.h"

enum EActorType {
    ACT_Hidden        = 0x1,
    ACT_Player        = 0x2,
    ACT_Car           = 0x4,
    ACT_Script        = 0x5,
    ACT_Door          = 0x6,
    ACT_Railway       = 0x8,
    ACT_VillaObject   = 0x9,
    ACT_Bottle        = 0xA,
    ACT_Traffic       = 0xC,
	ACT_DropOut		  = 0x11,
    ACT_Pedestrians   = 0x12,
    ACT_Bridge        = 0x14,
    ACT_Dog           = 0x15,
    ACT_Airplane      = 0x16,
    ACT_Turnout       = 0x18,
    ACT_Pumper        = 0x19,
    ACT_Human         = 0x1B,
    ACT_RaceCamera    = 0x1C,
    ACT_Wagon         = 0x1E,
    ACT_Irenka        = 0x1F,
    ACT_PublicPhysics = 0x20,
    ACT_Shot          = 0x21,
    ACT_Clock         = 0x22,
    ACT_Physical      = 0x23,
    ACT_Truck         = 0x24,
    ACT_RailGenerator = 0x26,
};

const char* GetActorTypeName(EActorType eActorType);

enum E_hit_type {
	
};

class C_chunk;
class C_program;
struct tDynamicCollObject;

#pragma pack(push, 1)
class C_actor {
public:
	enum E_act_state {
		IDLE,
		UNK2,
		MOVING,
	};

    C_actor();
    /*   ----- [C_actor REP SECTION]  -----*/
    virtual int Garbage1() {
        return -1;
    }
    virtual int Garbage2(int garbage) {
        return 0;
    }
    virtual int Garbage3(int garbage) {
        return 0;
    }
    virtual int Garbage4(int garbage) {
        return 1;
    }
    virtual int Garbage5(int garbage, int garbage2) {
        return 0;
    }
    virtual int Garbage6(int garbage, int garbage2) {
        return 0;
    }
    virtual int Garbage7(char garbage) {
        return 0;
    }
    virtual int Garbage8(char garbage) {
        return 0;
    }
    virtual int Garbage9(char garbage) {
        return 0;
    }
    virtual int Garbage10() {
        return 1;
    }
    virtual int Garbage11(int garbage, int garbage2) {
        return 0;
    }
    virtual int Garbage12(int garbage) {
        return 0;
    }
    virtual void Garbage13() { }

    /*   ----- [C_actor SECTION]  -----*/
    virtual void AI(uint32_t iFrameTime);
    virtual void Update(uint32_t iFrameTime);
    virtual void NetDirect(uint32_t iFrameTime);
    virtual ~C_actor();
    virtual void Release();
    virtual bool Init(I3D_frame* pFrame);
    virtual bool SaveData(C_chunk* pChunk);
    virtual bool LoadData(C_chunk* pChunk);
    virtual void DuplicateFrom(C_actor* pOther);
    virtual int32_t SaveGameGetSize();
    virtual bool SaveGameSave(void** pBuf);
    virtual bool SaveGameLoad(void** pBuf);
    virtual void GameInit();
    virtual void GameDone();
    virtual void ChangeState();
    virtual void GetActorSpeed(S_vector& vSpeed);
    virtual void Death();
    virtual bool Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pOtherFrame);
    virtual bool Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame);
    virtual int32_t GetMaterialID(I3D_frame* pFrame);
    virtual bool IsPossibleColl(C_actor* pActor, S_vector vUnk1, S_vector vUnk2, float fUnk, float& fUnk2, C_actor** pActorOut);
	virtual bool GetBoundPoints2D(std::vc6_vector<S_vector2>* vBoundPoints, I3D_frame* pFrame);
    virtual C_program* GetProgram();
    virtual bool cbCollTester(tDynamicCollObject* pDynamicCollObj);
    virtual void SoundsChange(int32_t iUnk);
    virtual void ChangeSceneInit();
    virtual void ChangeSceneDone();
	
	void SetActState(E_act_state state);

    inline void SetPos(const S_vector& vPos) {
        m_vWorldPos = vPos;
    }

    inline void SetDir(const S_vector& vDir) {
        m_vWorldDir = vDir;
    }

    inline S_vector GetPos() {
        return m_vWorldPos;
    }

    inline S_vector GetDir() {
        return m_vWorldDir;
    }

    EActorType GetActorType() {
        return m_eActorType;
    }

    I3D_frame* GetFrame() {
        return m_pFrame;
    }

//protected:
    									// 0x0	- 0x4 - vtable
	uint32_t m_uFlags;					// 0x4  - 0x8
	uint32_t m_uUnk;					// 0x8  - 0xC

	uint16_t m_uRefCount;				// 0xC  - 0xE
	uint8_t m_uRefCount_pad[2];			// 0xE  - 0x10

	EActorType m_eActorType;			// 0x10 - 0x14
	uint32_t m_uUnk3;					// 0x14 - 0x18
	uint32_t m_uID;						// 0x18 - 0x1C
	E_act_state m_eState;				// 0x1C - 0x20 
	
	bool m_bUnkBool;					// 0x20 - 0x21
	uint8_t m_bUnkBool_pad[3];			// 0x21 - 0x24

	S_vector m_vWorldPos;				// 0x24 - 0x30 
	S_vector m_vWorldDir;				// 0x30 - 0x3C
	S_vector m_vPosition;				// 0x3C - 0x48
	S_quat m_qRotation;					// 0x48 - 0x58
	I3D_frame *m_pPreviousFrame;		// 0x58	- 0x5C
	bool m_bIsOn;						// 0x5C - 0x5D
	bool m_bIsActive;					// 0x5D - 0x5E
	
	bool m_bDoRemove;					// 0x5E - 0x5F
	bool m_bUnk7;						// 0x5F - 0x60

	uint8_t m_uPadxx[2];				// 0x60 - 0x62	
	bool m_bUnk5;						// 0x62 - 0x63
	bool m_bIsActorHidden;				// 0x63 - 0x64

	bool m_bIsInited;				    // 0x64 - 0x65
	uint8_t m_IsInited__pad[3];			// 0x65 - 0x68
	I3D_frame *m_pFrame;				// 0x68 - 0x6C
					
	bool m_uUnk6;						// 0x6C - 0x6D
	uint8_t m_uUnk6_pad[3];				// 0x6D - 0x70
	
	friend class DebugGui;
    friend class C_debug;
};

constexpr int offset = offsetof(C_actor, m_vWorldDir);

#pragma pack(pop)