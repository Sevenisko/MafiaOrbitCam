#pragma once
#include "C_PhThing.h"

class C_public_physics : public C_PhThing {
public:
    C_public_physics():
        C_PhThing() {
        m_eActorType = ACT_PublicPhysics;
    }

    void GameInit();
};
