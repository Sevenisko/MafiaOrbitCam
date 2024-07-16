#pragma once

class I3D_frame;
class C_human;
class C_car;

class G_Camera {
public:
    I3D_frame* m_pFrame;
    I3D_frame* m_pCameraFrame;
    C_human* m_pFollowedHuman;
    C_car* m_pFollowedCar;
    int m_iCameraState;
    char pad2[68];
    bool m_bEnableSwing;
    float m_fSwingIntensity;
    I3D_frame* m_pSwingTargetFrame;
    char pad3[36];
    I3D_frame* m_pUnkFrame;
    char pad4[2];
    bool m_bFreeLookEnabled;
    float m_fSniperFOV;
};