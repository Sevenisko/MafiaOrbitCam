#pragma once

#include "Actors/C_traffic_generator.h"
#include "g_collision.h"
#include "C_using_object.h"

class C_traffic_element {
public:
    uint8_t pad0[4];
    C_traffic_generator* m_pGenerator;
    void* m_pAnimMachine;
    bool m_bIsActive;
    S_vector m_vPosition;
    S_vector m_vDirection;
    uint8_t pad1[24];
    I3D_frame* m_pFrame;
    I3D_frame* m_pHeadFrame;
    uint8_t pad2[4];
    I3D_frame* m_pUmbrellaFrame;
    uint8_t pad2_1[4];
    float m_fUnk1;
    uint8_t pad2_2[8];
    uint32_t m_uTargetNodeID;
    S_vector m_vUnk1;
    uint8_t pad3[12];
    uint32_t m_uOriginNodeID;
    S_vector m_vUnkArr[4];
    uint8_t pad4_1[8];
    I3D_frame* m_pUnkFrame;
    float m_fUnk2;
    uint8_t pad4_2[4];
    tDynamicCollObject m_sCollObject;
    uint8_t pad5[68];
    bool m_bHasCollisions;
    uint8_t pad5_1[4];
    bool m_bIsWoman;
    uint8_t pad5_2[19];
    int m_iAnimIndex;
    int m_iCurAnimIndex;
    S_vector m_vUnk2;
    uint8_t pad5_3[52];
    uint32_t m_uPedAnimType;
    uint8_t pad5_4[4];
    C_using_object::S_using_object m_sUsingObject;
    uint8_t pad6[4];
    uint16_t m_uTargetNodeType;
    uint8_t pad7[10];
};