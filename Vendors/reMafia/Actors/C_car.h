#pragma once

#include "C_actor.h"
#include "C_Vehicle.h"

class C_car : public C_actor {
public:
    void CarExplosion(uint32_t uUnk);
    void RepairPosition(bool bResetCar);
    static void OdjebajTuStrechu(I3D_frame* pFrame, bool bEnable);

    C_Vehicle m_sVehicle;
};