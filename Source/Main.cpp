#include <Actors/C_car.h>
#include <Actors/C_human.h>
#include <C_game.h>
#include <C_mission.h>
#include <G_Camera.h>
#include <I3D_driver.h>
#include <I3D_frame.h>
#include <IGraph.h>
#include <MinHook.h>
#include <ModConfig.h>
#include <cmath>
#include <g_collision.h>

#define M_PI 3.1415926535897931

#define TEXT_LINE(lineNum) lineNum * 16

static S_vector g_ZeroVector(0.0f, 0.0f, 0.0f);

static S_vector g_ColorBlack = g_ZeroVector;
static S_vector g_ColorWhite(1, 1, 1);
static S_vector g_ColorGray(0.65f, 0.65f, 0.65f);
static S_vector g_ColorRed(1.0f, 0.0f, 0.0f);
static S_vector g_ColorGreen(0.0f, 1.0f, 0.0f);
static S_vector g_ColorBlue(0.0f, 0.0f, 0.1f);

typedef void(__thiscall* G_Camera__Tick)(G_Camera*, uint32_t);
typedef void(__thiscall* G_Camera__SetCar)(G_Camera*, C_car*);
typedef bool(__thiscall* C_game__Init)(C_game*);
typedef void(__stdcall* IGraph__Present)(IGraph*);
typedef void(__thiscall* C_human__Go_Func1)(C_human*, bool);
typedef void(__thiscall* C_human__Go_Func2)(C_human*);
typedef void(__thiscall* C_human__PersonAnim)(C_human*, float, int, float);
typedef void(__thiscall* C_game__PauseAllSounds)(C_game*);
typedef void(__thiscall* C_game__NewExplosion)(C_game*, C_actor*, S_vector&, float, float, bool, bool, bool, int);
typedef void(__thiscall* C_player__AI_walk)(C_player*, int, int, float*);
typedef void(__thiscall* G_Inventory__Vystrel)(G_Inventory*);
typedef void(__cdecl* CloseSystem)();

G_Camera__Tick G_Camera__Tick_original           = nullptr;
G_Camera__SetCar G_Camera__SetCar_original       = nullptr;
C_game__Init C_game__Init_original               = nullptr;
IGraph__Present IGraph__Present_original            = nullptr;

C_human__Go_Func1 C_human__Go_FrontLeft_original  = nullptr;
C_human__Go_Func1 C_human__Go_FrontRight_original = nullptr;
C_human__Go_Func1 C_human__Go_Front_original      = nullptr;
C_human__Go_Func2 C_human__Go_BackLeft_original   = nullptr;
C_human__Go_Func2 C_human__Go_BackRight_original  = nullptr;
C_human__Go_Func2 C_human__Go_Back_original       = nullptr;
C_human__Go_Func1 C_human__Go_Left_original       = nullptr;
C_human__Go_Func1 C_human__Go_Right_original      = nullptr;
C_human__PersonAnim C_human__PersonAnim_original  = nullptr;

C_player__AI_walk C_player__AI_walk_original = nullptr;

C_game__PauseAllSounds C_game__PauseAllSounds_original = nullptr;
C_game__PauseAllSounds C_game__PlayAllSounds_original  = nullptr;
C_game__NewExplosion C_game__NewExplosion_original     = nullptr;

G_Inventory__Vystrel G_Inventory__Vystrel_original = nullptr;

CloseSystem CloseSystem_original = nullptr;

HMODULE ls3df = nullptr;

ModConfig* g_ModConfig = nullptr;

IDirect3DDevice8*& g_D3DDevice = *(IDirect3DDevice8**)(0x101C597C);

float g_CurTime = 0;

float g_RotX = 0, g_RotY = 0, g_Distance = 2.0f, g_CurDistance = g_Distance, g_InterpolationSpeed = 8.5f;

float g_CurHumanRot = 0, g_TargetHumanRot = 0;
float g_HumanYStandOffset = 1.85f, g_HumanYCrouchOffset = 1.25f, g_HumanYCurOffset = g_HumanYStandOffset;

float g_FrontFaceCurTime = 0;

float g_TargetShakeIntensity = 0, g_CurShakeIntensity = g_TargetShakeIntensity;
float g_ShakeIntensityX = 3.5f, g_ShakeIntensityY = 3.5f;
float g_ShakeAngleX = 0, g_ShakeAngleY = 0;
float g_AimOffsetRotX = 0, g_AimOffsetRotY = 0;

float g_ShootTargetOffsetRotX = 0, g_ShootTargetOffsetRotY = 0;
float g_ShootOffsetRotX = 0, g_ShootOffsetRotY = 0;

float g_ExplosionShakeIntensity = 0.0f;
float g_ExplodeAngleX = 0, g_ExplodeAngleY = 35.0f;
float g_ExplodeOffsetRotX = 0, g_ExplodeOffsetRotY = 0;

float g_WeaponKickIntensity     = 0.0f;
float g_WeaponKickDuration      = 0.0f;
float g_WeaponKickCurDuration   = 0.0f;

S_vector g_CamOffset;
S_vector g_TargetPos, g_CurPos;

bool g_FirstTime          = true;
bool g_IsInterpolating    = false;
bool g_IsInterpolatingRot = false;
bool g_IsPaused           = false;
bool g_IsAiming           = false;
bool g_IsRunning          = false;

bool g_LMBCurrentState  = false;
bool g_LMBPreviousState = false;
bool g_LMBWasPressed    = false;
bool g_LMBWasReleased   = false;
bool g_RMBCurrentState  = false;
bool g_RMBPreviousState = false;
bool g_RMBWasPressed    = false;
bool g_RMBWasReleased   = false;

bool g_CurAimState = false;
bool g_PrevAimState = false;
bool g_StartedAiming = false;
bool g_StoppedAiming = false;

static void InstallJmpHook(DWORD address, DWORD function) {
    DWORD lpflOldProtect;
    VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
    *(BYTE*)(address)      = 0xE9;
    *(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
    VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
}

#define Radians(degrees) degrees*(M_PI / 180)
#define Degrees(radians) radians * (180 / M_PI)

int GetRandomInt(int min, int max) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    return min + (rand() % (max - min + 1));
}

float GetRandomFloat(float min, float max) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    float range = max - min;
    return ((float)rand() / RAND_MAX) * range + min;
}

double NormalizeAngle(double angle) {
    while (angle < -360) angle += 360.0;
    while (angle > 360) angle -= 360.0;
    return angle;
}

double LerpAngle(double start, double end, double t) {
    start        = NormalizeAngle(start);
    end          = NormalizeAngle(end);
    double delta = end - start;

    if (abs(delta) > 180.0) {
        if (delta > 0) {
            start += 360.0;
        } else {
            end += 360.0;
        }
    }

    double result = start + t * (end - start);
    return NormalizeAngle(result);
}

float Lerpf(float start, float end, float t) {
    return start + t * (end - start);
}

S_vector LerpVec(const S_vector& start, const S_vector& end, float t) {
    return S_vector(Lerpf(start.x, end.x, t), Lerpf(start.y, end.y, t), Lerpf(start.z, end.z, t));
}

S_vector EulerFromDir(const S_vector& dir) {
    S_vector euler;

    float hyp = sqrtf(dir.x * dir.x + dir.z * dir.z);
    euler.y  = Degrees(atan2f(-dir.y, hyp));

    euler.x = Degrees(atan2f(dir.x, dir.z));

    return euler;
}

S_vector DirFromEuler(const S_vector& euler) {
    S_vector dir;

    float yawRad   = Radians(euler.x);
    float pitchRad = Radians(euler.y);

    dir.x = cosf(pitchRad) * sinf(yawRad);
    dir.y = sinf(pitchRad);
    dir.z = cosf(pitchRad) * cosf(yawRad);

    return dir;
}

void CalcOrbit(const S_vector& centerPos, const S_vector& targetDir, S_vector& pos, S_vector& dir, float radius, float azimuth, float elevation) {
    dir = targetDir;

    if (g_RotX >= 360.0f) {
        g_RotX = 0;
    } else if (g_RotX <= -360.0f) {
        g_RotX = 0;
    }

    if (g_RotY > 75.0f) {
        g_RotY = 75.0f;
    } else if (g_RotY < -75.0f) {
        g_RotY = -75.0f;
    }

    float offX = std::sin(Radians(g_RotX)) * std::cos(Radians(g_RotY));
    float offY = std::sin(Radians(g_RotY));
    float offZ = std::cos(Radians(g_RotX)) * std::cos(Radians(g_RotY));

    float camX = centerPos.x + offX * radius;
    float camY = centerPos.y + offY * radius;
    float camZ = centerPos.z + offZ * radius;

    pos = { camX, camY, camZ };

    dir.SetNormalized(centerPos - pos);
}

void ApplyWeaponKick(int itemID) {
    switch (itemID) {
    case 6: // Colt Detective Special & Colt 1911
    case 9: g_WeaponKickIntensity = 1.85f; break;
    case 7: // Magnum
        g_WeaponKickIntensity = 6.15f;
        break;
    case 8: // M&P
        g_WeaponKickIntensity = 5.65f;
        break;
    case 10: // Tommygun
        g_WeaponKickIntensity = 1.15f;
        break;
    case 11: // Sawn-off Shotgun + Pumping Shotgun
    case 12: g_WeaponKickIntensity = 10.25f; break;
    case 13: // Springfield + Mosin
    case 14: g_WeaponKickIntensity = 8.5f; break;
    default: // No weapon
        g_WeaponKickIntensity = 0.0f;
        break;
    }

    g_WeaponKickDuration = 0.1f;

    g_ShootTargetOffsetRotX = GetRandomFloat(-g_WeaponKickIntensity * 2, g_WeaponKickIntensity * 6) / 6;
    g_ShootTargetOffsetRotY = GetRandomFloat(-g_WeaponKickIntensity * 2, g_WeaponKickIntensity * 6) / 4;
}

void __fastcall G_Camera__SetCar_Hook(G_Camera* _this, DWORD edx, C_car* pCar) {
    G_Camera__SetCar_original(_this, pCar);

    if (pCar) {
        S_vector vCarPos(pCar->m_sVehicle.m_vPosition), vCenter(vCarPos.x, vCarPos.y + 1.85f, vCarPos.z), vPos, vDir;
        g_Distance           = 5.0f;
        g_InterpolationSpeed = 8.5f;
        g_IsInterpolating    = true;
        g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
        g_TargetPos          = vCenter;
    } else if (_this->m_pFollowedHuman) {
        S_vector vHumanPos(_this->m_pFollowedHuman->GetPos()), vCenter(vHumanPos.x, vHumanPos.y + 1.5f, vHumanPos.z), vPos, vDir;
        g_Distance           = 2.0f;
        g_InterpolationSpeed = 12.5f;
        g_IsInterpolating    = true;
        g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
        g_TargetPos          = vCenter;
    }
}

void __fastcall G_Camera__Tick_Hook(G_Camera* _this, DWORD edx, uint32_t uTickTime) {
    if (!_this->m_pCameraFrame)
        return;

    if (!g_IsPaused) {
        float deltaTime = float(uTickTime / 1000.0f);

        g_CurShakeIntensity = Lerpf(g_CurShakeIntensity, g_TargetShakeIntensity, 4.5f * deltaTime);

        g_ShakeAngleX += (g_ShakeIntensityX)*deltaTime;
        g_ShakeAngleY += (g_ShakeIntensityY)*deltaTime;

        if (g_ShakeAngleX > 360)
            g_ShakeAngleX = 0;

        if (g_ShakeAngleY > 360)
            g_ShakeAngleY = 0;

        g_AimOffsetRotX = cosf(g_ShakeAngleX) * 0.5f;
        g_AimOffsetRotY = sinf(g_ShakeAngleY) * 0.5f;

        if (g_WeaponKickDuration > 0) {
            g_WeaponKickCurDuration += deltaTime;

            if (g_WeaponKickCurDuration >= g_WeaponKickDuration) {
                g_WeaponKickDuration = 0;
                g_WeaponKickCurDuration = 0;
                g_ShootTargetOffsetRotX = 0;
                g_ShootTargetOffsetRotY = 0;
            }
        }

        C_human* pHuman = _this->m_pFollowedHuman;
        C_car* pCar     = _this->m_pFollowedCar;

        if (pHuman && pHuman->m_bIsCrouching) {
            g_ShootTargetOffsetRotX *= 0.75f;
            g_ShootTargetOffsetRotY *= 0.65f;
        }

        g_ShootOffsetRotX = Lerpf(g_ShootOffsetRotX, g_ShootTargetOffsetRotX, 24.5f * deltaTime);
        g_ShootOffsetRotY = Lerpf(g_ShootOffsetRotY, g_ShootTargetOffsetRotY, 24.5f * deltaTime);

        if (g_ExplosionShakeIntensity > 0) {
            g_ExplosionShakeIntensity -= (0.45f * deltaTime);

            if (g_ExplosionShakeIntensity < 0)
                g_ExplosionShakeIntensity = 0;
        }

        g_ExplodeAngleX += (g_ExplosionShakeIntensity * 48.0f)*deltaTime;
        g_ExplodeAngleY += (g_ExplosionShakeIntensity * 48.0f)*deltaTime;

        if (g_ExplodeAngleX > 360)
            g_ExplodeAngleX = 0;

        if (g_ExplodeAngleY > 360)
            g_ExplodeAngleY = 0;

        g_ExplodeOffsetRotX = (cosf(g_ExplodeAngleX) * 9.5f) * g_ExplosionShakeIntensity;
        g_ExplodeOffsetRotY = -(sinf(g_ExplodeAngleY) * 9.5f) * g_ExplosionShakeIntensity;

        switch (_this->m_iCameraState) {
        case 7: g_Distance = 4.5f; break;
        case 8: g_Distance = 6.5f; break;
        case 9: g_Distance = 8.5f; break;
        case 12: g_Distance = 4.5f; break;
        }

        if (g_FirstTime) {
            g_IsInterpolating = false;
            g_FirstTime       = false;

            g_IsAiming           = false;
            g_IsInterpolatingRot = false;

            if (pCar) {
                S_vector vEuler = EulerFromDir(pCar->GetDir());

                S_vector vCarPos   = pCar->m_vWorldPos;
                g_TargetPos       = { vCarPos.x, vCarPos.y + 1.85f, vCarPos.z };
                g_CurPos          = g_TargetPos;
                g_IsInterpolating = false;

                g_RotX = vEuler.x + 180.0f;
                g_RotY = vEuler.y + 12.5f;
            } else if (pHuman) {
                S_vector vEuler = EulerFromDir(pHuman->GetDir());

                S_vector vHumanPos = pHuman->m_vWorldPos;

                g_TargetPos       = { vHumanPos.x, vHumanPos.y + 1.5f, vHumanPos.z };
                g_CurPos          = g_TargetPos;
                g_IsInterpolating = false;

                g_Distance = 2.0f;

                g_RotX = vEuler.x + 180.0f;
                g_RotY = vEuler.y;
            }

            g_CurDistance = g_Distance;
        }

        IGraph* pIGraph = GetIGraph();

        auto iMouseX = pIGraph->Mouse_rx();
        auto iMouseY = pIGraph->Mouse_ry();

        uint32_t uButtons = pIGraph->GetMouseButtons();

        g_LMBPreviousState = g_LMBCurrentState;
        g_LMBCurrentState  = (uButtons & 1) != 0;

        g_LMBWasPressed  = !g_LMBPreviousState && g_LMBCurrentState;
        g_LMBWasReleased = g_LMBPreviousState && !g_LMBCurrentState;

        g_RMBPreviousState = g_RMBCurrentState;
        g_RMBCurrentState  = (uButtons & 2) != 0;

        g_RMBWasPressed  = !g_RMBPreviousState && g_RMBCurrentState;
        g_RMBWasReleased = g_RMBPreviousState && !g_RMBCurrentState;

        if ((iMouseX != 0 || iMouseY != 0)) {
            g_IsInterpolatingRot = false;
            g_FrontFaceCurTime   = 0.0f;
        }
        
        g_RotX += ((iMouseX * (_this->m_iCameraState != 5 ? g_ModConfig->fMouseSensitivity : g_ModConfig->fSniperMouseSensitivity)) * (g_ModConfig->bInvertMouseX ? -1 : 1));
        g_RotY += ((iMouseY * (_this->m_iCameraState != 5 ? g_ModConfig->fMouseSensitivity : g_ModConfig->fSniperMouseSensitivity)) * (g_ModConfig->bInvertMouseY ? -1 : 1));

        bool bIsCarCameraMode    = (_this->m_iCameraState == 7 || _this->m_iCameraState == 8 || _this->m_iCameraState == 9);
        bool bIsPlayerCameraMode = (_this->m_iCameraState == 1 || _this->m_iCameraState == 5);

        if (pHuman) {
            g_PrevAimState = g_CurAimState;
            g_CurAimState  = pHuman->m_bIsAiming;

            g_StartedAiming = !g_PrevAimState && g_CurAimState;
            g_StoppedAiming = g_PrevAimState && !g_CurAimState;
        }

        g_CurPos      = LerpVec(g_CurPos, g_TargetPos, (g_InterpolationSpeed) * deltaTime);
        g_CurDistance = Lerpf(g_CurDistance, g_Distance, (g_InterpolationSpeed) * deltaTime);

        if ((g_TargetPos - g_CurPos).Magnitude() <= 0.15f) {
            g_IsInterpolating = false;
            g_CurPos          = g_TargetPos;
        }

        g_CurTime += deltaTime;

        if (pCar && bIsCarCameraMode) {
            if (!g_ModConfig->bEnableInCarOrbitCam) {
                G_Camera__Tick_original(_this, uTickTime);
                return;
            }

            if (pCar->m_sVehicle.m_fSpeed >= 2.5f && !g_IsInterpolatingRot) {
                g_FrontFaceCurTime += deltaTime;

                if (g_FrontFaceCurTime > g_ModConfig->fCarFrontFacingTime) {
                    g_IsInterpolatingRot = true;
                }
            } else if (g_IsInterpolatingRot) {
                S_vector vEuler = EulerFromDir(pCar->GetDir());

                g_RotX = LerpAngle(g_RotX, vEuler.x + 180.0f, 4.5f * deltaTime);
                g_RotY = LerpAngle(g_RotY, vEuler.y + 12.5f, 4.5f * deltaTime);
            }

            S_vector vCarPos(pCar->m_sVehicle.m_vPosition), vCenter, vPos, vDir;

            if (pHuman && pHuman->m_bIsAiming) {
                auto vHumanPos = pHuman->GetFrame()->GetWorldPos();

                vCenter = { vHumanPos.x, vHumanPos.y + 2.45f, vHumanPos.z };

                CalcOrbit(vCenter, pCar->GetDir(), vPos, vDir, 1.25f, g_RotX, g_RotY);

                S_vector vRaycastPos, vRaycastNormal;

                S_vector vEuler = EulerFromDir(vDir);
                vDir            = DirFromEuler({ vEuler.x + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -vEuler.y + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                g_pCollisionManager->TestLineV(vPos + (vDir * g_CurDistance), (vDir * -(g_CurDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 1, 0x80000u);

                S_vector finalPos { 0, 0, 0 };

                /*if (!g_IsInterpolating && g_StartedAiming && g_ModConfig->bEnableCameraInterpolation) {
                    g_StartedAiming   = false;
                    g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
                    g_InterpolationSpeed = 26.5f;
                    g_IsInterpolating    = true;
                }*/

                if (!g_IsInterpolating) {
                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                    }
                } else {
                    if (finalPos.Magnitude() == 0) {
                        g_TargetPos = vPos;
                    } else {
                        g_TargetPos = finalPos;
                    }

                    _this->m_pCameraFrame->SetWorldPos(g_CurPos);
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, 0);
            } else {
                vCenter = { vCarPos.x, vCarPos.y + 1.5f, vCarPos.z };

                CalcOrbit(vCenter, pCar->GetDir(), vPos, vDir, g_CurDistance, g_RotX, g_RotY);

                S_vector vRaycastPos, vRaycastNormal;

                S_vector vEuler = EulerFromDir(vDir);
                vDir            = DirFromEuler({ vEuler.x + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -vEuler.y + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                g_pCollisionManager->TestLineV(vPos + (vDir * g_CurDistance), (vDir * -(g_CurDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

                S_vector finalPos { 0, 0, 0 };

                if (vRaycastPos.Magnitude() != 0) {
                    finalPos += vRaycastPos + (vDir * 0.15f);
                }

                /*if (!g_IsInterpolating && g_StoppedAiming && g_ModConfig->bEnableCameraInterpolation) {
                    g_StoppedAiming   = false;
                    g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
                    g_InterpolationSpeed = 26.5f;
                    g_IsInterpolating    = true;
                }*/

                if (!g_IsInterpolating) {
                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                    }
                } else {
                    if (finalPos.Magnitude() == 0) {
                        g_TargetPos = vPos;
                    } else {
                        g_TargetPos = finalPos;
                    }

                    _this->m_pCameraFrame->SetWorldPos(g_CurPos);
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, g_ModConfig->bEnableCarRotRoll ? pCar->m_sVehicle.m_vAngularVelocity.y * g_ModConfig->fCarRotRollIntensity : 0);
            }

            _this->m_pCameraFrame->Update();
        } else if (pHuman && _this->m_iCameraState == 17) {
            pHuman->m_vWorldDir = DirFromEuler({ g_RotX, 0, 0 });

            G_Camera__Tick_original(_this, uTickTime);

            if (g_RotY >= 45.0f)
                g_RotY = 45.0f;

            if (g_RotY <= -45.0f)
                g_RotY = -45.0f;

            S_vector vDir = DirFromEuler({ g_RotX + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -g_RotY + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

            _this->m_pCameraFrame->SetWorldDir(vDir, 0);
            _this->m_pCameraFrame->Update();
        } else if (pHuman && _this->m_iCameraState == 23) {
            g_TargetHumanRot = NormalizeAngle(g_RotX);

            S_vector vHumanRot = EulerFromDir(pHuman->GetFrame()->GetLocalDir());

            pHuman->m_vWorldDir = DirFromEuler({ g_TargetHumanRot, 0, 0 });

            pHuman->GetFrame()->SetDir(pHuman->m_vWorldDir, 0);

            S_vector vHumanPos(pHuman->GetFrame()->GetWorldPos()), vPos, vDir, vRightDir;

            vHumanRot = EulerFromDir(pHuman->GetFrame()->GetWorldDir());

            vDir = DirFromEuler({ vHumanRot.x + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -g_RotY + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

            vRightDir = vDir.Cross({ 0, 1, 0 });

            vPos = { vHumanPos.x - (vRightDir.x * 0.3f) - (vDir.x * 0.75f), vHumanPos.y + (g_HumanYCrouchOffset) - (vDir.y * 0.75f), vHumanPos.z - (vRightDir.z * 0.3f) - (vDir.z * 0.75f) };

            if (g_RotY >= 45.0f)
                g_RotY = 45.0f;

            if (g_RotY <= -45.0f)
                g_RotY = -45.0f;

            _this->m_pCameraFrame->SetWorldPos(vPos);
            _this->m_pCameraFrame->SetWorldDir(vDir, 0);
            _this->m_pCameraFrame->Update();
        } else if (pHuman && bIsPlayerCameraMode) {
            if (!g_ModConfig->bEnableOnFootOrbitCam) {
                G_Camera__Tick_original(_this, uTickTime);
                return;
            }

            if (g_RMBWasPressed) {
                g_RMBWasPressed = false;
                g_IsAiming      = true;

                S_vector vHumanPos(pHuman->GetFrame()->GetWorldPos()), vPos, vDir, vRightDir;

                g_TargetHumanRot = g_RotX + 180.0f;

                vDir      = DirFromEuler({ g_TargetHumanRot + (g_AimOffsetRotX * 1.25f) + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -g_RotY + (g_AimOffsetRotY * 1.25f) + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });
                vRightDir = vDir.Cross({ 0, 1, 0 });

                vPos = { vHumanPos.x - (vRightDir.x * 0.3f) - (_this->m_iCameraState != 5 ? (vDir.x * 0.75f) : 0), vHumanPos.y + (g_HumanYCurOffset) - (vDir.y * 0.75f), vHumanPos.z - (vRightDir.z * 0.3f) - (vDir.z * 0.75f) };

                S_vector vRaycastPos, vRaycastNormal;

                g_pCollisionManager->TestLineV(vPos + (vDir * 0.75f), (vDir * -0.85f), &vRaycastPos, &vRaycastNormal, 0x80002u, 0x800000u);

                S_vector finalPos { 0, 0, 0 };

                if (vRaycastPos.Magnitude() != 0) {
                    finalPos += vRaycastPos + (vDir * 0.15f);
                    g_TargetPos = finalPos;
                } else {
                    g_TargetPos = vPos;
                }

                if (!g_IsInterpolating && (g_TargetPos - g_CurPos).Magnitude() > 0.15f && g_ModConfig->bEnableCameraInterpolation) {
                    g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
                    g_InterpolationSpeed = 26.5f;
                    g_IsInterpolating = true;
                }
            } else if (g_RMBWasReleased) {
                g_RMBWasReleased = false;
                g_IsAiming       = false;

                S_vector vHumanPos(pHuman->GetFrame()->GetWorldPos()), vCenter(vHumanPos.x, vHumanPos.y + g_HumanYCurOffset, vHumanPos.z), vPos, vDir;

                CalcOrbit(vCenter, { 1, 0, 0 }, vPos, vDir, g_Distance, g_RotX, g_RotY);

                S_vector vEuler = EulerFromDir(vDir);
                vDir            = DirFromEuler({ vEuler.x + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -vEuler.y + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                S_vector vRaycastPos, vRaycastNormal;

                g_pCollisionManager->TestLineV(vPos + (vDir * g_Distance), (vDir * -(g_Distance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0x80002u, 0x800000u);

                S_vector vRightDir = vDir.Cross({ 0, 1, 0 });
                S_vector vUpDir    = vRightDir.Cross(vDir);

                vPos += vRightDir * g_ModConfig->vCameraOffset.x;
                vPos += vUpDir * g_ModConfig->vCameraOffset.y;
                vPos += vDir * g_ModConfig->vCameraOffset.z;

                S_vector finalPos { 0, 0, 0 };

                if (vRaycastPos.Magnitude() != 0) {
                    finalPos += vRaycastPos + (vDir * 0.15f);
                    g_TargetPos = finalPos;
                } else {
                    g_TargetPos = vPos;
                }

                if (!g_IsInterpolating && (g_TargetPos - g_CurPos).Magnitude() > 0.15f && g_ModConfig->bEnableCameraInterpolation) {
                    g_CurPos             = _this->m_pCameraFrame->GetWorldPos();
                    g_InterpolationSpeed = 26.5f;
                    g_IsInterpolating = true;
                }
            }

            if (g_IsAiming) {
                g_TargetHumanRot = NormalizeAngle(g_RotX + 180.0f);

                g_CurHumanRot = LerpAngle(g_CurHumanRot, g_TargetHumanRot, 9.5f * deltaTime);

                pHuman->m_vWorldDir = DirFromEuler({ g_CurHumanRot, 0, 0 });

                S_vector vHumanPos(pHuman->GetFrame()->GetWorldPos()), vCenter, vPos, vDir, vRightDir, vUpDir;

                if (pHuman->m_sInventory.sSelectedItem.iItemID == 14) {
                    if (_this->m_iCameraState != 5) {
                        _this->SetSniperMode(true);
                        _this->SetSniperFov(0.12f);
                    }
                    g_ShakeIntensityX = 0.25f;
                    g_ShakeIntensityY = 0.45f;

                    vDir = DirFromEuler({ g_TargetHumanRot + (g_AimOffsetRotX * 0.45f) + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -g_RotY + (g_AimOffsetRotY * 0.25f) + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                    vPos = { vHumanPos.x + (vDir.x * 1.75f), vHumanPos.y + (g_HumanYCurOffset) + (vDir.y * 1.75f), vHumanPos.z + (vDir.z * 1.75f) };

                    _this->m_pCameraFrame->SetWorldPos(vPos);
                } else {
                    g_ShakeIntensityX = 1.25f;
                    g_ShakeIntensityY = 1.45f;

                    vDir = DirFromEuler({ g_TargetHumanRot + (g_AimOffsetRotX * 1.25f) + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -g_RotY + (g_AimOffsetRotY * 1.25f) + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                    vRightDir = vDir.Cross({ 0, 1, 0 });

                    vUpDir = vRightDir.Cross(vDir);

                    g_HumanYCurOffset = Lerpf(g_HumanYCurOffset, pHuman->m_bIsCrouching ? g_HumanYCrouchOffset : g_HumanYStandOffset, 4.5f * deltaTime);

                    vPos = { vHumanPos.x - (vRightDir.x * 0.3f) - (_this->m_iCameraState != 5 ? (vDir.x * 0.75f) : 0), vHumanPos.y + (g_HumanYCurOffset) - (vDir.y * 0.75f), vHumanPos.z - (vRightDir.z * 0.3f) - (vDir.z * 0.75f) };

                    if (g_RotY >= 45.0f)
                        g_RotY = 45.0f;

                    if (g_RotY <= -45.0f)
                        g_RotY = -45.0f;

                    S_vector vRaycastPos, vRaycastNormal;

                    g_pCollisionManager->TestLineV(vPos + (vDir * 0.75f), (vDir * -0.85f), &vRaycastPos, &vRaycastNormal, 0x80002u, 0x800000u);

                    S_vector finalPos { 0, 0, 0 };

                    if (vRaycastPos.Magnitude() != 0) {
                        finalPos += vRaycastPos + (vDir * 0.15f);
                    }

                    if (!g_IsInterpolating) {
                        if (finalPos.Magnitude() == 0) {
                            _this->m_pCameraFrame->SetWorldPos(vPos);
                            g_CurPos = vPos;
                        } else {
                            _this->m_pCameraFrame->SetWorldPos(finalPos);
                            g_CurPos = finalPos;
                        }
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(g_CurPos);
                        if (finalPos.Magnitude() == 0) {
                            g_TargetPos = vPos;
                        } else {
                            g_TargetPos = finalPos;
                        }
                    }
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, 0);

            } else {
                S_vector vHumanPos(pHuman->GetFrame()->GetWorldPos()), vCenter, vPos, vDir;

                if (_this->m_iCameraState == 5) {
                    _this->SetSniperMode(false);
                }

                g_HumanYCurOffset = Lerpf(g_HumanYCurOffset, pHuman->m_bIsCrouching ? g_HumanYCrouchOffset : g_HumanYStandOffset, 4.5f * deltaTime);

                g_CurHumanRot = LerpAngle(g_CurHumanRot, g_TargetHumanRot, 4.5f * deltaTime);

                pHuman->m_vWorldDir = DirFromEuler({ g_CurHumanRot, 0, 0 });

                vCenter = { vHumanPos.x, vHumanPos.y + g_HumanYCurOffset, vHumanPos.z };

                CalcOrbit(vCenter, { 1, 0, 0 }, vPos, vDir, g_Distance, g_RotX, g_RotY);

                S_vector vEuler = EulerFromDir(vDir);
                vDir            = DirFromEuler({ vEuler.x + g_ShootOffsetRotX + g_ExplodeOffsetRotX, -vEuler.y + g_ShootOffsetRotY + g_ExplodeOffsetRotY, 0 });

                S_vector vRightDir = vDir.Cross({ 0, 1, 0 });
                S_vector vUpDir    = vRightDir.Cross(vDir);

                vPos += vRightDir * g_ModConfig->vCameraOffset.x;
                vPos += vUpDir * g_ModConfig->vCameraOffset.y;
                vPos += vDir * g_ModConfig->vCameraOffset.z;

                S_vector vRaycastPos, vRaycastNormal;

                g_pCollisionManager->TestLineV(vPos + (vDir * g_Distance), (vDir * -(g_Distance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0x80002u, 0x800000u);

                S_vector finalPos { 0, 0, 0 };

                if (vRaycastPos.Magnitude() != 0) {
                    finalPos += vRaycastPos + (vDir * 0.15f);
                }

                if (!g_IsInterpolating) {
                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                        g_CurPos = vPos;
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                        g_CurPos = finalPos;
                    }
                } else {
                    _this->m_pCameraFrame->SetWorldPos(g_CurPos);
                    if (finalPos.Magnitude() == 0) {
                        g_TargetPos = vPos;
                    } else {
                        g_TargetPos = finalPos;
                    }
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, 0);
            }

            _this->m_pCameraFrame->Update();
        } else {
            G_Camera__Tick_original(_this, uTickTime);
        }
    }
}

void __fastcall C_player__AI_walk_Hook(C_player* _this, DWORD edx, int a2, int a3, float* a4) {
    g_IsRunning = *(bool*)(((int)_this) + sizeof(C_human) + 0x3C);

    C_player__AI_walk_original(_this, a2, a3, a4);
}

void __fastcall G_Inventory__Vystrel_Hook(G_Inventory* _this, DWORD edx) {
    G_Inventory__Vystrel_original(_this);

    ApplyWeaponKick(_this->sSelectedItem.iItemID);
}

bool __fastcall C_game__Init_Hook(C_game* _this, DWORD edx) {
    bool ret = C_game__Init_original(_this);

    g_FirstTime = true;
    g_IsPaused  = false;

    g_CurTime = 0;

    return ret;
}

void __fastcall C_game__PauseAllSounds_Hook(C_game* _this, DWORD edx) {
    g_IsPaused = true;
    C_game__PauseAllSounds_original(_this);
}

void __fastcall C_game__PlayAllSounds_Hook(C_game* _this, DWORD edx) {
    g_IsPaused = false;
    C_game__PlayAllSounds_original(_this);
}

void __fastcall C_game__NewExplosion_Hook(C_game* _this, DWORD edx, C_actor* pActor, S_vector& vOrigin, float fRange, float fDamage, bool a6, bool bCreateParticle, bool a8, int iSoundId) {
    C_game__NewExplosion_original(_this, pActor, vOrigin, fRange, fDamage, a6, bCreateParticle, a8, iSoundId);

    if (_this->m_pHuman) {
        S_vector vHumanPos = _this->m_pHuman->GetPos();
        float distance     = (vOrigin - vHumanPos).Magnitude();
        float modifiedRange = (fRange * 3.5f);
        if (distance < modifiedRange) {
            float intensity            = 1.0f - (distance / modifiedRange);
            g_ExplosionShakeIntensity = intensity;
        }
    }
}

void __fastcall C_human__PersonAnim_Hook(C_human* _this, DWORD edx, float a2, int a3, float a4) {
    C_human* pHuman = g_pMission->m_pGame->m_pHuman;

    if (_this == pHuman && g_ModConfig->bEnableOnFootOrbitCam) {
        if (g_IsAiming) {
            pHuman->m_bIsAiming = true;
        }
    }

    C_human__PersonAnim_original(_this, a2, a3, a4);
}

void __fastcall C_human__Go_FrontLeft_Hook(C_human* _this, DWORD edx, bool a2) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_FrontLeft_original(_this, a2);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_FrontLeft_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) - 45.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_FrontLeft_original(_this, a2);
    }
}

void __fastcall C_human__Go_FrontRight_Hook(C_human* _this, DWORD edx, bool a2) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_FrontRight_original(_this, a2);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_FrontRight_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) + 45.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_FrontRight_original(_this, a2);
    }
}

void __fastcall C_human__Go_Front_Hook(C_human* _this, DWORD edx, bool a2) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_Front_original(_this, a2);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_Front_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f);

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_Front_original(_this, a2);
    }
}

void __fastcall C_human__Go_BackLeft_Hook(C_human* _this, DWORD edx) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_BackLeft_original(_this);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_BackLeft_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) + 180.0f + 45.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_BackLeft_original(_this);
    }
}

void __fastcall C_human__Go_BackRight_Hook(C_human* _this, DWORD edx) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_BackRight_original(_this);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_BackRight_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) + 180.0f - 45.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_BackRight_original(_this);
    }
}

void __fastcall C_human__Go_Back_Hook(C_human* _this, DWORD edx) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_Back_original(_this);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_Back_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) + 180.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_Back_original(_this);
    }
}

void __fastcall C_human__Go_Left_Hook(C_human* _this, DWORD edx, bool a2) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_Left_original(_this, a2);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_Left_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) - 90.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_Left_original(_this, a2);
    }
}

void __fastcall C_human__Go_Right_Hook(C_human* _this, DWORD edx, bool a2) {
    if (!g_ModConfig->bEnableOnFootOrbitCam) {
        C_human__Go_Right_original(_this, a2);
        return;
    }

    if (_this == g_pMission->m_pGame->m_pHuman && g_IsAiming) {
        C_human__Go_Right_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && (g_pMission->m_pGame->m_eCamera.m_iCameraState != 17 && g_pMission->m_pGame->m_eCamera.m_iCameraState != 23)) {
        g_TargetHumanRot = (g_RotX - 180.0f) + 90.0f;

        C_human__Go_Front_original(_this, g_IsRunning);
    } else {
        C_human__Go_Right_original(_this, a2);
    }
}

void __stdcall IGraph__Present_Hook(IGraph* pIGraph) {
    if (g_pMission->m_pGame && g_D3DDevice) {
        g_D3DDevice->BeginScene();

        auto pCamera = &g_pMission->m_pGame->m_eCamera;

        char buf[256] = { 0 };
        sprintf(buf, "Cam state: %d", pCamera->m_iCameraState);
        I3DGetDriver()->DrawText2D(0, 0, buf, 0, 1.0f);

        if (pCamera->m_pCameraFrame) {
            S_vector vPos = pCamera->m_pCameraFrame->GetWorldPos();
            S_vector vDir = pCamera->m_pCameraFrame->GetWorldDir();
            sprintf(buf, "Cam pos: [%f, %f, %f]", vPos.x, vPos.y, vPos.z);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(1), buf, 0, 1.0f);

            sprintf(buf, "Cam dir: [%f, %f, %f", vDir.x, vDir.y, vDir.z);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(2), buf, 0, 1.0f);

            sprintf(buf, "Rotation: [%f, %f]", g_RotX, g_RotY);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(3), buf, 0, 1.0f);

            sprintf(buf, "Human rot: %f", g_TargetHumanRot);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(4), buf, 0, 1.0f);

            sprintf(buf, "Cam dist: %f (dest %f)", g_CurDistance, g_Distance);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(5), buf, 0, 1.0f);

            sprintf(buf, "Interpol. speed: %f", g_InterpolationSpeed);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(6), buf, 0, 1.0f);

            sprintf(buf, "g_TargetPos: [%f, %f, %f]", g_TargetPos.x, g_TargetPos.y, g_TargetPos.z);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(7), buf, 0, 1.0f);

            sprintf(buf, "g_CurPos: [%f, %f, %f]", g_CurPos.x, g_CurPos.y, g_CurPos.z);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(8), buf, 0, 1.0f);

            sprintf(buf, "Distance: %f", (g_TargetPos - g_CurPos).Magnitude());
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(9), buf, 0, 1.0f);

            sprintf(buf, "Is interpolating: %s", g_IsInterpolating ? "Yes" : "No");
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(10), buf, 0, 1.0f);

            sprintf(buf, "g_fBaseOffsetRot: [%f, %f]", g_AimOffsetRotX, g_AimOffsetRotY);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(11), buf, 0, 1.0f);

            sprintf(buf, "g_fShootOffsetRot: [%f, %f] - g_fShootDuration: %f, g_fShootIntensity: %f", g_ShootOffsetRotX, g_ShootOffsetRotY, g_WeaponKickDuration, g_WeaponKickIntensity);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(12), buf, 0, 1.0f);

            sprintf(buf, "g_fExplodeOffsetRot: [%f, %f] - g_fExplodeIntensity: %f", g_ExplodeOffsetRotX, g_ExplodeOffsetRotY, g_ExplosionShakeIntensity);
            I3DGetDriver()->DrawText2D(0, TEXT_LINE(13), buf, 0, 1.0f);
        }

        g_D3DDevice->EndScene();
    }

    IGraph__Present_original(pIGraph);
}

void CloseSystem_Hook() {
    g_ModConfig->SaveConfig(Utils::GetModPath() + "\\MafiaOrbitCam.ini");

    CloseSystem_original();
}

void SetPlayerRunning() {
    g_IsRunning = !g_IsRunning;
}

void SetPlayerDir(C_human* pPlayer) {
    // pPlayer->m_vWorldDir = DirFromEuler({ g_RotX + 180.0f, 0, 0 });
}

void SetPlayerShootTarget(C_human* pPlayer) {
    S_vector vPos = g_pMission->m_pGame->m_eCamera.m_pCameraFrame->GetWorldPos(), vDir = g_pMission->m_pGame->m_eCamera.m_pCameraFrame->GetWorldDir();

    S_vector vRelDest = vDir * 100.0f;

    S_vector vRaycastPos, vRaycastNormal;

    g_pCollisionManager->TestLineV(vPos, vRelDest, &vRaycastPos, &vRaycastNormal, 1, 0x80000u);
    // g_pCollisionManager->TestLineV(pos + dir * (g_CurDistance * 1.45f), vRelDest, &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

    if (vRaycastPos.Magnitude() != 0) {
        pPlayer->m_vShootTarget = vRaycastPos;
    }
}

__declspec(naked) void C_player__AI_walk_setIsRunning() {
    __asm {
        call SetPlayerRunning
        mov eax, 0x58FA2C
        jmp eax
    }
}

__declspec(naked) void C_player__AI_walk_setDir() {
    __asm {
        // sub esp, 4
        push esi
        call SetPlayerDir
        add esp, 4
        mov eax, 0x58FFC0
        jmp eax
    }
}

__declspec(naked) void C_player__AI_drive_setShootDest() {
    __asm {
        push esi
        call SetPlayerShootTarget
        add esp, 4
        mov eax, 0x592B40
        jmp eax
    }
}

extern "C" __declspec(dllexport) void InitializeASI() {
#ifdef _DEBUG
    while (!IsDebuggerPresent()) {
        MessageBoxA(NULL, "Waiting for the debugger, press OK once you've attached one.", "Bruv", MB_OK | MB_ICONINFORMATION);
    }
#endif

    ls3df = GetModuleHandleA("LS3DF.dll");

    if (!ls3df) {
        return;
    }

    if (MH_Initialize() != MH_OK) {
        MessageBox(NULL, "Failed to initialize MinHook", "Error", MB_OK | MB_ICONERROR);
        ExitProcess(1);
    }

    static ModConfig sModCfg(Utils::GetModPath() + "\\MafiaOrbitCam.ini");

    if (!sModCfg.IsLoaded()) {
        sModCfg.SaveConfig(Utils::GetModPath() + "\\MafiaOrbitCam.ini");
    }

    g_ModConfig = &sModCfg;

    IGraph* pIGraph = GetIGraph();

    uint32_t iVTableAddr = *(uint32_t*)pIGraph;

    void* pAddrPresent  = *(void**)(iVTableAddr + 60);
    void* pAddrEndScene = *(void**)(iVTableAddr + 56);

    if (g_ModConfig->bEnableCamOrbit) {
        MH_CreateHook((LPVOID)0x5ED4C0, (LPVOID)&G_Camera__Tick_Hook, (LPVOID*)&G_Camera__Tick_original);
        MH_CreateHook((LPVOID)0x5ED190, (LPVOID)&G_Camera__SetCar_Hook, (LPVOID*)&G_Camera__SetCar_original);

        MH_CreateHook((LPVOID)0x5A0810, (LPVOID)&C_game__Init_Hook, (LPVOID*)&C_game__Init_original);
        MH_CreateHook((LPVOID)0x5B8430, (LPVOID)&C_game__PauseAllSounds_Hook, (LPVOID*)&C_game__PauseAllSounds_original);
        MH_CreateHook((LPVOID)0x5B85F0, (LPVOID)&C_game__PlayAllSounds_Hook, (LPVOID*)&C_game__PlayAllSounds_original);
        MH_CreateHook((LPVOID)0x5AAE10, (LPVOID)&C_game__NewExplosion_Hook, (LPVOID*)&C_game__NewExplosion_original);

        if (g_ModConfig->bEnableOnFootOrbitCam) {
            MH_CreateHook((LPVOID)0x573E50, (LPVOID)&C_human__PersonAnim_Hook, (LPVOID*)&C_human__PersonAnim_original);
            MH_CreateHook((LPVOID)0x57D670, (LPVOID)&C_human__Go_FrontLeft_Hook, (LPVOID*)&C_human__Go_FrontLeft_original);
            MH_CreateHook((LPVOID)0x57D860, (LPVOID)&C_human__Go_FrontRight_Hook, (LPVOID*)&C_human__Go_FrontRight_original);
            MH_CreateHook((LPVOID)0x57D360, (LPVOID)&C_human__Go_Front_Hook, (LPVOID*)&C_human__Go_Front_original);
            MH_CreateHook((LPVOID)0x57DA50, (LPVOID)&C_human__Go_BackLeft_Hook, (LPVOID*)&C_human__Go_BackLeft_original);
            MH_CreateHook((LPVOID)0x57DC10, (LPVOID)&C_human__Go_BackRight_Hook, (LPVOID*)&C_human__Go_BackRight_original);
            MH_CreateHook((LPVOID)0x57D430, (LPVOID)&C_human__Go_Back_Hook, (LPVOID*)&C_human__Go_Back_original);
            MH_CreateHook((LPVOID)0x57D4D0, (LPVOID)&C_human__Go_Left_Hook, (LPVOID*)&C_human__Go_Left_original);
            MH_CreateHook((LPVOID)0x57D5A0, (LPVOID)&C_human__Go_Right_Hook, (LPVOID*)&C_human__Go_Right_original);
        }

        MH_CreateHook((LPVOID)0x5C0080, (LPVOID)&CloseSystem_Hook, (LPVOID*)&CloseSystem_original);

        if (g_ModConfig->bEnableOnFootOrbitCam) {
            MH_CreateHook((LPVOID)0x609E50, (LPVOID)&G_Inventory__Vystrel_Hook, (LPVOID*)&G_Inventory__Vystrel_original);

            MH_CreateHook((LPVOID)0x58F4B0, (LPVOID)&C_player__AI_walk_Hook, (LPVOID*)&C_player__AI_walk_original);
        }

        if (g_ModConfig->bEnableDebugStats) {
            MH_CreateHook(pAddrPresent, (LPVOID)&IGraph__Present_Hook, (LPVOID*)&IGraph__Present_original);
        }

        if (g_ModConfig->bEnableOnFootOrbitCam) {
            InstallJmpHook(0x58FA23, (DWORD)&C_player__AI_walk_setIsRunning);
            InstallJmpHook(0x58FFB2, (DWORD)&C_player__AI_walk_setDir);
            InstallJmpHook(0x592926, (DWORD)&C_player__AI_drive_setShootDest);

            InstallJmpHook(0x58FF13, 0x58FF52);
        }

        MH_EnableHook(MH_ALL_HOOKS);
    }
}
