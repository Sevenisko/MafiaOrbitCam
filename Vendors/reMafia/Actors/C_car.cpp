#include "C_car.h"

void C_car::CarExplosion(uint32_t uUnk) {
    ((void(__thiscall*)(C_car*, uint32_t))(0x421D60))(this, uUnk);
}

void C_car::RepairPosition(bool bResetCar) {
    ((void(__thiscall*)(C_car*, bool))(0x41E690))(this, bResetCar);
}

void C_car::OdjebajTuStrechu(I3D_frame* pFrame, bool bEnable) {
    ((void(__fastcall*)(I3D_frame*, bool))(0x42BA70))(pFrame, bEnable);
}
