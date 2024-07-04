#pragma once
#include "C_actor.h"

class C_traffic_element;

//#pragma pack(push, 1)
class C_traffic_generator : public C_actor {
public:
	C_traffic_generator();
	void AI(uint32_t uFrameTime);
	virtual ~C_traffic_generator();
	bool Init(I3D_frame* pFrame);
	bool SaveData(C_chunk* pChunk);
    bool LoadData(C_chunk* pChunk);
    void DuplicateFrom(C_actor* pOther);
    int32_t SaveGameGetSize();
    bool SaveGameSave(void** pBuf);
    bool SaveGameLoad(void** pBuf);
    void GameInit();
    void GameDone();
private:
    bool m_bUnk1;
    float m_fSpawnRadius;
    int m_iUnk1;
    C_traffic_element* m_pTrafficElements;
    int m_iNumSpawnedPeds;
    int m_iNumElements;
    float m_fDespawnRadius;
    float m_fInnerSpawnRadius;
    float m_fOuterSpawnRadius;
    float m_fInnerSpawnRadius2;
    char** m_pModelNames;
    int m_iNumPeds;
    uint32_t* m_pDensityRatios;
    int m_iRNGShit;
    int m_iMaxSpawnedPeds;
    int m_iMaxPedsCount;
    int m_iRandomPedID;
    int m_iUnk3;

    friend class C_debug;
};
//#pragma pack(pop)