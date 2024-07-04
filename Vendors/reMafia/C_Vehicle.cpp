#include "C_Vehicle.h"

void C_Vehicle::EnableRightIndicator(bool bEnable) {
    if (!(m_uLightFlags & 8)) {
        if (bEnable) {
            m_uLightFlags |= 2;
            if (m_pCallbackWU)
                ((void (*)(const void*))(m_pCallbackWU))(m_pCar);

        } else {
            m_uLightFlags &= 0xFD;
        }
    }
}

void C_Vehicle::DisableTurnIndicatorFlag(bool bEnable) {
    if (bEnable) {
        m_uLightFlags |= 8;
    } else {
        m_uLightFlags &= 0xF7;
    }
}

void C_Vehicle::EnableLeftIndicator(bool bEnable) {
    if (!(m_uLightFlags & 8)) {
        if (bEnable) {
            m_uLightFlags |= 1;
            if (m_pCallbackWU)
                ((void (*)(const void*))(m_pCallbackWU))(m_pCar);
        } else {
            m_uLightFlags &= 0xFE;
        }
    }
}

void C_Vehicle::SetLightFlags(uint32_t uFlags) {
    m_uLightFlags ^= (uFlags ^ m_uLightFlags) & 0xFF00000;
    if (m_pCallbackWU)
        ((void (*)(const void*))(m_pCallbackWU))(m_pCar);
}

void C_Vehicle::EnableLight(bool bEnable) {
    if (bEnable) {
        m_uLightFlags |= 0x80;
    } else {
        m_uLightFlags &= 0x7F;
    }
    if (m_pCallbackWU)
        ((void (*)(const void*))(m_pCallbackWU))(m_pCar);
}

void C_Vehicle::EnableLightsValue(bool bEnable) {
    _pad28[360] = bEnable;
}
