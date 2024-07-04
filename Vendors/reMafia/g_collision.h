#pragma once
#include <LS3D.h>

class I3D_frame;
struct I3D_bbox;
class C_actor;

struct tDynamicCollObject {
    int m_iUnk;
    I3D_frame* m_pAttachedFrame;
    int m_iUnk2;
    I3D_bbox* m_pBBOXUnk1;
    S_vector m_vPos;
    float m_fScale;
    uint8_t pad3[4];
    I3D_bbox m_sBoundBox;
    S_matrix* m_pmWorldMatrix;
    S_matrix* m_pmLocalMatrix;
    C_actor* m_pAttachedActor;
};

class g_collision {
public:
    void LoadCollision(const char* szName, bool bUnk);
    void DynInsert(tDynamicCollObject* pCollObj);
    uint32_t DynErase(tDynamicCollObject* pCollObj);
    void TestSphereB(const S_vector& vCenter, const S_vector& vWorldPos, float fDistance, S_vector* pvOutPos, S_vector* pvOutUnk2, uint32_t uUnk, uint32_t uFlags);
    void TestLineV(const S_vector& vPos1, const S_vector& vPos2, S_vector* pvOutUnk1, S_vector* pvOutUnk2, uint32_t uUnk, uint32_t uFlags);
};

static g_collision* g_pCollisionManager = (g_collision*)0x647F48;
