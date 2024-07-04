#include "g_collision.h"

void g_collision::LoadCollision(const char* szName, bool bUnk) {
    ((void(__thiscall*)(g_collision*, const char*, bool))(0x5C2B70))(this, szName, bUnk);
}

void g_collision::DynInsert(tDynamicCollObject* pCollObj) {
    ((void(__thiscall*)(g_collision*, tDynamicCollObject*))(0x1C36C0))(this, pCollObj);
}
uint32_t g_collision::DynErase(tDynamicCollObject* pCollObj) {
    return ((uint32_t(__thiscall*)(g_collision*, tDynamicCollObject*))(0x1C38B0))(this, pCollObj);
}

void g_collision::TestSphereB(const S_vector& vPos, const S_vector& vUnk, float fDistance, S_vector* pvOutPos, S_vector* pvOutNormal, uint32_t uUnk, uint32_t uFlags) {
    ((void(__thiscall*)(g_collision*, const S_vector&, const S_vector&, float, S_vector*, S_vector*, uint32_t, uint32_t))(0x005D5940))(this, vPos, vUnk, fDistance, pvOutPos, pvOutNormal, uUnk, uFlags);
}

void g_collision::TestLineV(const S_vector& vPos, const S_vector& vRelDestination, S_vector* pvOutPos, S_vector* pvOutNormal, uint32_t uUnk, uint32_t uFlags) {
    ((void(__thiscall*)(g_collision*, const S_vector&, const S_vector&, S_vector*, S_vector*, uint32_t, uint32_t))(0x005C6B10))(this, vPos, vRelDestination, pvOutPos, pvOutNormal, uUnk, uFlags);
}