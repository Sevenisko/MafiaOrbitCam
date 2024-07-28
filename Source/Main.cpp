#include <MinHook.h>
#include <I3D_frame.h>
#include <Actors/C_human.h>
#include <Actors/C_car.h>
#include <G_Camera.h>
#include <g_collision.h>
#include <cmath>
#include <IGraph.h>
#include <C_game.h>
#include <C_mission.h>
#include <I3D_driver.h>
#include <ModConfig.h>

#define M_PI 3.1415926535897931

static S_vector vZero(0.0f, 0.0f, 0.0f);

static S_vector vColorBlack = vZero;
static S_vector vColorWhite(1, 1, 1);
static S_vector vColorGray(0.65f, 0.65f, 0.65f);
static S_vector vColorRed(1.0f, 0.0f, 0.0f);
static S_vector vColorGreen(0.0f, 1.0f, 0.0f);
static S_vector vColorBlue(0.0f, 0.0f, 0.1f);

typedef void(__thiscall* G_Camera__Tick)(G_Camera*, uint32_t);
typedef void(__thiscall* G_Camera__SetCar)(G_Camera*, C_car*);
typedef void(__thiscall* G_Camera__SetPlayer)(G_Camera*, C_human*);
typedef bool(__thiscall* C_game__Init)(C_game*);
typedef void(__stdcall* IGraph__Func)(IGraph*);
typedef void(__thiscall* C_human__Go_Func1)(C_human*, bool);
typedef void(__thiscall* C_human__Go_Func2)(C_human*);
typedef void(__thiscall* C_human__PersonAnim)(C_human*, float, int, float);
typedef void(__thiscall* C_game__PauseAllSounds)(C_game*, bool);
typedef void(__thiscall* C_game__NewExplosion)(C_game*, C_actor*, S_vector&, float, float, bool, bool, bool, int);

G_Camera__Tick G_Camera__Tick_original = nullptr;
G_Camera__SetCar G_Camera__SetCar_original = nullptr;
G_Camera__SetPlayer G_Camera__SetPlayer_original = nullptr;
C_game__Init C_game__Init_original               = nullptr;
IGraph__Func IGraph__Present_original       = nullptr;
IGraph__Func IGraph__EndScene_original           = nullptr;

C_human__Go_Func1 C_human__Go_FrontLeft_original = nullptr;
C_human__Go_Func1 C_human__Go_FrontRight_original = nullptr;
C_human__Go_Func1 C_human__Go_Front_original      = nullptr;
C_human__Go_Func2 C_human__Go_BackLeft_original   = nullptr;
C_human__Go_Func2 C_human__Go_BackRight_original   = nullptr;
C_human__Go_Func2 C_human__Go_Back_original   = nullptr;
C_human__Go_Func1 C_human__Go_Left_original        = nullptr;
C_human__Go_Func1 C_human__Go_Right_original        = nullptr;
C_human__PersonAnim C_human__PersonAnim_original   = nullptr;

C_game__PauseAllSounds C_game__PauseAllSounds_original = nullptr;
C_game__PauseAllSounds C_game__PlayAllSounds_original  = nullptr;
C_game__NewExplosion C_game__NewExplosion_original     = nullptr;

HMODULE ls3df = nullptr;

ModConfig* g_pModConfig = nullptr;

IDirect3DDevice8*& g_pD3DDevice = *(IDirect3DDevice8**)(0x101C597C);

float g_fRotX = 0, g_fRotY = 0, g_fDistance = 2.0f, g_fCurDistance = g_fDistance, g_fInterpolationSpeed = 8.5f;

float g_fCurHumanRot = 0, g_fTargetHumanRot = 0;

float g_fFrontFaceCurTime = 0;

float g_fHumanYStandOffset = 1.85f, g_fHumanYCrouchOffset = 1.25f, g_fHumanYCurOffset = g_fHumanYStandOffset;

float g_fOffsetRotX = 0, g_fOffsetRotY = 0;
float g_fTargetShakeIntensity = 0, g_fCurShakeIntensity = g_fTargetShakeIntensity;
float g_fShakeIntensityX = 3.5f, g_fShakeIntensityY = 3.5f;
float g_fShakeAngleX = 0, g_fShakeAngleY = 0;

S_vector g_vTargetPos, g_vCurPos;

bool g_bFirstTime          = true;
bool g_bIsInterpolating    = false;
bool g_bIsInterpolatingRot = false;
bool g_bIsPaused           = false;
bool g_bIsAiming           = false;

static void InstallCallHook(DWORD address, DWORD function) {
    DWORD lpflOldProtect;
    VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
    *(BYTE*)(address)      = 0xE8;
    *(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
    VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
}

static void InstallJmpHook(DWORD address, DWORD function) {
    DWORD lpflOldProtect;
    VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
    *(BYTE*)(address)      = 0xE9;
    *(DWORD*)(address + 1) = (unsigned long)function - (address + 5);
    VirtualProtect((void*)address, 5, lpflOldProtect, &lpflOldProtect);
}

void DebugPrintf(const char* str, ...) {
    va_list va;

    va_start(va, str);

    char buf[256];

    vsprintf(buf, str, va);

    OutputDebugStringA(buf);

    va_end(va);
}

#define Radians(degrees) degrees*(M_PI / 180)
#define Degrees(radians) radians * (180 / M_PI)

double NormalizeAngle(double angle) {
    while (angle < 0) angle += 360.0;
    while (angle >= 360) angle -= 360.0;
    return angle;
}

double LerpAngle(double start, double end, double t) {
    start        = NormalizeAngle(start);
    end          = NormalizeAngle(end);
    double delta = end - start;

    if (std::abs(delta) > 180.0) {
        if (delta > 0) {
            start += 360.0;
        } else {
            end += 360.0;
        }
    }

    double result = start + t * (end - start);
    return NormalizeAngle(result);
}

float Lerpf(float fBegin, float fEnd, float t) {
    return fBegin + t * (fEnd - fBegin);
}

S_vector LerpVec(const S_vector& vBegin, const S_vector& vEnd, float t) {
    return S_vector(Lerpf(vBegin.x, vEnd.x, t), Lerpf(vBegin.y, vEnd.y, t), Lerpf(vBegin.z, vEnd.z, t));
}

S_vector EulerFromDir(const S_vector& vDir) {
    S_vector vEuler;

    float hyp = sqrtf(vDir.x * vDir.x + vDir.z * vDir.z);
    vEuler.y  = Degrees(atan2f(-vDir.y, hyp));

    vEuler.x = Degrees(atan2f(vDir.x, vDir.z));

    return vEuler;
}

S_vector DirFromEuler(const S_vector& vEuler) {
    S_vector vDir;

    float yawRad = Radians(vEuler.x);
    float pitchRad = Radians(vEuler.y);

    vDir.x = cosf(pitchRad) * sinf(yawRad);
    vDir.y = sinf(pitchRad);
    vDir.z = cosf(pitchRad) * cosf(yawRad);

    return vDir;
}

void CalcOrbit(const S_vector& vCenterPos, const S_vector& vTargetDir, S_vector& vPos, S_vector& vDir, float fRadius, float fAzimuth, float fElevation) {
    vDir = vTargetDir;

    if (g_fRotX >= 360.0f) {
        g_fRotX = 0;
    } else if (g_fRotX <= -360.0f) {
        g_fRotX = 0;
    }

    if (g_fRotY > 75.0f) {
        g_fRotY = 75.0f;
    } else if (g_fRotY < -75.0f) {
        g_fRotY = -75.0f;
    }

    float fOffX = std::sin(Radians(g_fRotX)) * std::cos(Radians(g_fRotY));
    float fOffY = std::sin(Radians(g_fRotY));
    float fOffZ = std::cos(Radians(g_fRotX)) * std::cos(Radians(g_fRotY));

    float fCamX = vCenterPos.x + fOffX * fRadius;
    float fCamY = vCenterPos.y + fOffY * fRadius;
    float fCamZ = vCenterPos.z + fOffZ * fRadius;

    vPos = { fCamX, fCamY, fCamZ };

    vDir.SetNormalized(vCenterPos - vPos);
}

void __fastcall G_Camera__SetPlayer_Hook(G_Camera* _this, DWORD edx, C_human* pHuman) {
    G_Camera__SetPlayer_original(_this, pHuman);
}

void __fastcall G_Camera__SetCar_Hook(G_Camera* _this, DWORD edx, C_car* pCar) {
    G_Camera__SetCar_original(_this, pCar);

    if (pCar) {
        S_vector vCarPos(pCar->m_sVehicle.m_vPosition), vCenter(vCarPos.x, vCarPos.y + 1.85f, vCarPos.z), vPos, vDir;
        g_fDistance        = 5.0f;
        g_fInterpolationSpeed = 8.5f;
        g_bIsInterpolating = true;
        g_vTargetPos = vCenter;
    } else if (_this->m_pFollowedHuman) {
        S_vector vHumanPos(_this->m_pFollowedHuman->GetPos()), vCenter(vHumanPos.x, vHumanPos.y + 1.5f, vHumanPos.z), vPos, vDir;

        g_fDistance = 2.0f;

        g_fInterpolationSpeed = 8.5f;

        g_bIsInterpolating = true;

        g_vTargetPos = vCenter;
    }
}

// TODO: Smoothly interpolate between normal and aim modes (both player and car) - Блыад!
void __fastcall G_Camera__Tick_Hook(G_Camera* _this, DWORD edx, uint32_t uTickTime) {
    if (!g_bIsPaused) {
        g_fCurShakeIntensity = Lerpf(g_fCurShakeIntensity, g_fTargetShakeIntensity, 4.5f * float(uTickTime / 1000.0f));

        g_fShakeAngleX += g_fShakeIntensityX * float(uTickTime / 1000.0f);
        g_fShakeAngleY += g_fShakeIntensityY * float(uTickTime / 1000.0f);

        if (g_fShakeAngleX > 360)
            g_fShakeAngleX = 0;

        if (g_fShakeAngleY > 360)
            g_fShakeAngleY = 0;

        g_fOffsetRotX = cosf(g_fShakeAngleX);
        g_fOffsetRotY = sinf(g_fShakeAngleY);

        if (g_bFirstTime) {
            g_bIsInterpolating = false;
            g_bFirstTime       = false;

            if (_this->m_pFollowedCar) {
                S_vector vEuler = EulerFromDir(_this->m_pFollowedCar->GetDir());

                g_fRotX = vEuler.x + 180.0f;
                g_fRotY = vEuler.y + 12.5f;
            } else if (_this->m_pFollowedHuman) {
                S_vector vEuler = EulerFromDir(_this->m_pFollowedHuman->GetDir());

                g_fDistance = 2.0f;

                g_fRotX = vEuler.x + 180.0f;
                g_fRotY = vEuler.y;
            }
        }

        switch (_this->m_iCameraState) {
        case 7:
            g_fInterpolationSpeed = 4.5f;
            g_fDistance           = 4.5f;
            break;
        case 8:
            g_fInterpolationSpeed = 4.5f;
            g_fDistance           = 6.5f;
            break;
        case 9:
            g_fInterpolationSpeed = 4.5f;
            g_fDistance           = 8.5f;
            break;
        case 12:
            g_fInterpolationSpeed = 6.5f;
            g_fDistance           = 4.5f;
            break;
        }

        C_human* pHuman = _this->m_pFollowedHuman;
        C_car* pCar     = _this->m_pFollowedCar;

        IGraph* pIGraph = GetIGraph();

        auto iMouseX = pIGraph->Mouse_rx();
        auto iMouseY = pIGraph->Mouse_ry();

        if ((iMouseX != 0 || iMouseY != 0)) {
            g_bIsInterpolatingRot = false;
            g_fFrontFaceCurTime   = 0.0f;
        }

        g_fRotX += ((iMouseX * g_pModConfig->fMouseSensitivity) * (g_pModConfig->bInvertMouseX ? -1 : 1));
        g_fRotY += ((iMouseY * g_pModConfig->fMouseSensitivity) * (g_pModConfig->bInvertMouseY ? -1 : 1));

        /*g_fDistance -= pIGraph->Mouse_rz() * 0.005f;

        if (g_fDistance < 4.5f)
            g_fDistance = 4.5f;
        else if (g_fDistance > 12.5f)
            g_fDistance = 12.5f;
        */

        bool bIsCarCameraMode    = (_this->m_iCameraState == 7 || _this->m_iCameraState == 8 || _this->m_iCameraState == 9);
        bool bIsPlayerCameraMode = (_this->m_iCameraState == 1 || _this->m_iCameraState == 5);

        g_vCurPos      = LerpVec(g_vCurPos, g_vTargetPos, g_fInterpolationSpeed * float(uTickTime / 1000.0f));
        g_fCurDistance = Lerpf(g_fCurDistance, g_fDistance, g_fInterpolationSpeed * float(uTickTime / 1000.0f));

        if ((g_vTargetPos - g_vCurPos).Magnitude() <= 0.05f) {
            g_bIsInterpolating = false;
        }

        if (pCar && bIsCarCameraMode) {
            if (pCar->m_sVehicle.m_fSpeed >= 2.5f && !g_bIsInterpolatingRot) {
                g_fFrontFaceCurTime += float(uTickTime / 1000.0f);

                if (g_fFrontFaceCurTime > g_pModConfig->fCarFrontFacingTime) {
                    g_bIsInterpolatingRot = true;
                }
            } else if (g_bIsInterpolatingRot) {
                S_vector vEuler = EulerFromDir(pCar->GetDir());

                g_fRotX = LerpAngle(g_fRotX, vEuler.x + 180.0f, 4.5f * float(uTickTime / 1000.0f));
                g_fRotY = LerpAngle(g_fRotY, vEuler.y + 12.5f, 4.5f * float(uTickTime / 1000.0f));
            }

            S_vector vCarPos(pCar->m_sVehicle.m_vPosition), vCenter, vPos, vDir;

            if (g_bIsInterpolating) {
                vCenter = g_vCurPos;
            } else {
                vCenter   = { vCarPos.x, vCarPos.y + 1.5f, vCarPos.z };
                g_vCurPos = vCenter;
            }

            if (_this->m_pCameraFrame) {
                if (pHuman->m_bIsAiming) {
                    auto vHumanPos = pHuman->GetFrame()->GetWorldPos();

                    vCenter = { vHumanPos.x, vHumanPos.y + 2.45f, vHumanPos.z };

                    CalcOrbit(vCenter, pCar->GetDir(), vPos, vDir, 1.25f, g_fRotX, g_fRotY);

                    S_vector vRaycastPos, vRaycastNormal;

                    g_pCollisionManager->TestLineV(vPos + (vDir * g_fCurDistance), (vDir * -(g_fCurDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 1, 0x80000u);

                    S_vector finalPos { 0, 0, 0 };

                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                    }

                    _this->m_pCameraFrame->SetWorldDir(vDir, 0);
                } else {
                    CalcOrbit(vCenter, pCar->GetDir(), vPos, vDir, g_fCurDistance, g_fRotX, g_fRotY);

                    S_vector vRaycastPos, vRaycastNormal;

                    g_pCollisionManager->TestLineV(vPos + (vDir * g_fCurDistance), (vDir * -(g_fCurDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

                    S_vector finalPos { 0, 0, 0 };

                    if (vRaycastPos.Magnitude() > 0) {
                        finalPos += vRaycastPos + (vRaycastNormal * 0.15f);
                    }

                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                    }
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, g_pModConfig->bEnableCarRotRoll ? pCar->m_sVehicle.m_vAngularVelocity.y * g_pModConfig->fCarRotRollIntensity : 0);
                _this->m_pCameraFrame->Update();
            }
        } else if (pHuman && _this->m_iCameraState == 17) {
            pHuman->m_vWorldDir = DirFromEuler({ g_fRotX, 0, 0 });

            G_Camera__Tick_original(_this, uTickTime);

            if (g_fRotY >= 45.0f)
                g_fRotY = 45.0f;

            if (g_fRotY <= -45.0f)
                g_fRotY = -45.0f;

            S_vector vDir = DirFromEuler({ g_fRotX, -g_fRotY, 0 });

            _this->m_pCameraFrame->SetWorldDir(vDir, 0);

            //G_Camera__Tick_original(_this, uTickTime);
        } else if (pHuman && bIsPlayerCameraMode) {
            uint32_t uMouseButtons = pIGraph->GetMouseButtons();

            if (uMouseButtons & 2) {
                g_bIsAiming = true;
            } else {
                g_bIsAiming = false;
            }

            if (g_bIsAiming) {
                g_fTargetHumanRot = g_fRotX + 180.0f;

                g_fCurHumanRot = LerpAngle(g_fCurHumanRot, g_fTargetHumanRot, 9.5f * float(uTickTime / 1000.0f));

                pHuman->m_vWorldDir = DirFromEuler({ g_fCurHumanRot, 0, 0 });

                S_vector vHumanPos(pHuman->GetPos()), vPos, vDir, vRightDir;

                vDir = DirFromEuler({ g_fTargetHumanRot, -g_fRotY, 0 });

                vRightDir = vDir.Cross({ 0, 1, 0 });

                g_fHumanYCurOffset = Lerpf(g_fHumanYCurOffset, pHuman->m_bIsCrouching ? g_fHumanYCrouchOffset : g_fHumanYStandOffset, 4.5f * float(uTickTime / 1000.0f));

                vPos = { vHumanPos.x - (vRightDir.x * 0.3f) - (vDir.x * 0.75f), vHumanPos.y + (g_fHumanYCurOffset) - (vDir.y * 0.75f), vHumanPos.z - (vRightDir.z * 0.3f) - (vDir.z * 0.75f) };

                if (g_fRotY >= 45.0f)
                    g_fRotY = 45.0f;

                if (g_fRotY <= -45.0f)
                    g_fRotY = -45.0f;

                S_vector vRaycastPos, vRaycastNormal;

                g_pCollisionManager->TestLineV(vPos + (vDir * g_fDistance), (vDir * -(g_fDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

                S_vector finalPos { 0, 0, 0 };

                if (vRaycastPos.Magnitude() > 0) {
                    finalPos += vRaycastPos + (vDir * 0.02f);
                }

                if (finalPos.Magnitude() == 0) {
                    _this->m_pCameraFrame->SetWorldPos(vPos);
                } else {
                    _this->m_pCameraFrame->SetWorldPos(finalPos);
                }

                _this->m_pCameraFrame->SetWorldDir(vDir, 0);
                _this->m_pCameraFrame->Update();
            } else {
                S_vector vHumanPos(pHuman->GetPos()), vCenter, vPos, vDir;

                g_fHumanYCurOffset = Lerpf(g_fHumanYCurOffset, pHuman->m_bIsCrouching ? g_fHumanYCrouchOffset : g_fHumanYStandOffset, 4.5f * float(uTickTime / 1000.0f));

                if (g_bIsInterpolating) {
                    vCenter = g_vCurPos;
                } else {
                    vCenter   = { vHumanPos.x, vHumanPos.y + g_fHumanYCurOffset, vHumanPos.z };
                    g_vCurPos = vCenter;
                }

                g_fCurHumanRot = LerpAngle(g_fCurHumanRot, g_fTargetHumanRot, 4.5f * float(uTickTime / 1000.0f));

                pHuman->m_vWorldDir = DirFromEuler({ g_fCurHumanRot, 0, 0 });

                if (_this->m_pCameraFrame) {
                    CalcOrbit(vCenter, { 1, 0, 0 }, vPos, vDir, g_fDistance, g_fRotX, g_fRotY);

                    S_vector vRaycastPos, vRaycastNormal;

                    g_pCollisionManager->TestLineV(vPos + (vDir * g_fDistance), (vDir * -(g_fDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

                    S_vector finalPos { 0, 0, 0 };

                    if (vRaycastPos.Magnitude() > 0) {
                        finalPos += vRaycastPos + (vRaycastNormal * 0.02f);
                    }

                    if (finalPos.Magnitude() == 0) {
                        _this->m_pCameraFrame->SetWorldPos(vPos);
                    } else {
                        _this->m_pCameraFrame->SetWorldPos(finalPos);
                    }

                    _this->m_pCameraFrame->SetWorldDir(vDir, 0);
                    _this->m_pCameraFrame->Update();
            }
            }
        } else {
            G_Camera__Tick_original(_this, uTickTime);
        }
    }
}

bool __fastcall C_game__Init_Hook(C_game* _this, DWORD edx) {
    bool ret = C_game__Init_original(_this);

    g_bFirstTime = true;
    g_bIsPaused  = false;

    return ret;
}

void __fastcall C_game__PauseAllSounds_Hook(C_game* _this, DWORD edx, bool a2) {
    g_bIsPaused = true;
    C_game__PauseAllSounds_original(_this, a2);
}

void __fastcall C_game__PlayAllSounds_Hook(C_game* _this, DWORD edx, bool a2) {
    g_bIsPaused = false;
    C_game__PlayAllSounds_original(_this, a2);
}

void __fastcall C_game__NewExplosion_Hook(C_game* _this, DWORD edx, C_actor* pActor, S_vector& vOrigin, float fRange, float fDamage, bool a6, bool bCreateParticle, bool a8, int iSoundId) {
    

    C_game__NewExplosion_original(_this, pActor, vOrigin, fRange, fDamage, a6, bCreateParticle, a8, iSoundId);
}

void __fastcall C_human__PersonAnim_Hook(C_human* _this, DWORD edx, float a2, int a3, float a4) {
    C_human* pHuman = g_pMission->m_pGame->m_pHuman;
    
    if (_this == pHuman) {
        if (g_bIsAiming) {
            pHuman->m_bIsAiming = true;

            /*switch (pHuman->m_sInventory.sSelectedItem.iItemID) {
            case 6:
            case 7:
            case 8:
            case 9: {
                switch (pHuman->m_iAnimState) {
                case 1: pHuman->m_iAnimState = 513; break;
                case 6: pHuman->m_iAnimState = 518; break;
                case 17: pHuman->m_iAnimState = 529; break;
                case 18: pHuman->m_iAnimState = 530; break;
                case 7: pHuman->m_iAnimState = 519; break;
                case 19: pHuman->m_iAnimState = 531; break;
                case 20: pHuman->m_iAnimState = 532; break;
                case 11: pHuman->m_iAnimState = 523; break;
                case 12: pHuman->m_iAnimState = 524; break;
                default: break;
                }
            } break;
            case 10:
            case 11:
            case 12: {
                switch (pHuman->m_iAnimState) {
                case 4097: pHuman->m_iAnimState = 2049; break;
                case 4102: pHuman->m_iAnimState = 2054; break;
                case 4113: pHuman->m_iAnimState = 2065; break;
                case 4114: pHuman->m_iAnimState = 2066; break;
                case 4103: pHuman->m_iAnimState = 2055; break;
                case 4115: pHuman->m_iAnimState = 2067; break;
                case 4116: pHuman->m_iAnimState = 2068; break;
                case 4107: pHuman->m_iAnimState = 2059; break;
                case 4108: pHuman->m_iAnimState = 2060; break;
                default: break;
                }
            } break;
            case 13:
            case 14: {
                switch (pHuman->m_iAnimState) {
                case 4097: pHuman->m_iAnimState = 1025; break;
                case 4102: pHuman->m_iAnimState = 1030; break;
                case 4113: pHuman->m_iAnimState = 1041; break;
                case 4114: pHuman->m_iAnimState = 1042; break;
                case 4103: pHuman->m_iAnimState = 1031; break;
                case 4115: pHuman->m_iAnimState = 1043; break;
                case 4116: pHuman->m_iAnimState = 1044; break;
                case 4107: pHuman->m_iAnimState = 1035; break;
                case 4108: pHuman->m_iAnimState = 1036; break;
                }
            } break;
            default: break;
            }*/
        }
    }

    C_human__PersonAnim_original(_this, a2, a3, a4);
}

void __fastcall C_human__Go_FrontLeft_Hook(C_human* _this, DWORD edx, bool a2) {
    if (g_bIsAiming) {
        C_human__Go_FrontLeft_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) - 45.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_FrontLeft_original(_this, a2);
    }
}

void __fastcall C_human__Go_FrontRight_Hook(C_human* _this, DWORD edx, bool a2) {
    if (g_bIsAiming) {
        C_human__Go_FrontRight_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) + 45.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_FrontRight_original(_this, a2);
    }
}

void __fastcall C_human__Go_Front_Hook(C_human* _this, DWORD edx, bool a2) {
    if (g_bIsAiming) {
        C_human__Go_Front_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f);

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_Front_original(_this, a2);
    }
}

void __fastcall C_human__Go_BackLeft_Hook(C_human* _this, DWORD edx) {
    if (g_bIsAiming) {
        C_human__Go_BackLeft_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f + 45.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_BackLeft_original(_this);
    }
}

void __fastcall C_human__Go_BackRight_Hook(C_human* _this, DWORD edx) {
    if (g_bIsAiming) {
        C_human__Go_BackRight_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f - 45.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_BackRight_original(_this);
    }
}

void __fastcall C_human__Go_Back_Hook(C_human* _this, DWORD edx) {
    if (g_bIsAiming) {
        C_human__Go_Back_original(_this);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_Back_original(_this);
    }
}

void __fastcall C_human__Go_Left_Hook(C_human* _this, DWORD edx, bool a2) {
    if (g_bIsAiming) {
        C_human__Go_Left_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) - 90.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_Left_original(_this, a2);
    }
}

void __fastcall C_human__Go_Right_Hook(C_human* _this, DWORD edx, bool a2) {
    if (g_bIsAiming) {
        C_human__Go_Right_original(_this, false);
    } else if (_this == g_pMission->m_pGame->m_pHuman && g_pMission->m_pGame->m_eCamera.m_iCameraState != 17) {
        g_fTargetHumanRot = (g_fRotX - 180.0f) + 90.0f;

        C_human__Go_Front_original(_this, true);
    } else {
        C_human__Go_Right_original(_this, a2);
    }
}

void __stdcall IGraph__EndScene_Hook(IGraph* pIGraph) {
    IGraph__EndScene_original(pIGraph);
}

void __stdcall IGraph__Present_Hook(IGraph* pIGraph) {
    if (g_pMission->m_pGame && g_pD3DDevice) {
        g_pD3DDevice->BeginScene();

        auto pCamera = &g_pMission->m_pGame->m_eCamera;

        char buf[128] = { 0 };
        sprintf(buf, "Cam state: %d", pCamera->m_iCameraState);
        I3DGetDriver()->DrawText2D(0, 0, buf, 0, 1.0f);

        if (pCamera->m_pCameraFrame) {
            S_vector vPos = pCamera->m_pCameraFrame->GetWorldPos();
            S_vector vDir = pCamera->m_pCameraFrame->GetWorldDir();
            sprintf(buf, "Cam pos: [%f, %f, %f]", vPos.x, vPos.y, vPos.z);
            I3DGetDriver()->DrawText2D(0, 16, buf, 0, 1.0f);

            sprintf(buf, "Cam dir: [%f, %f, %f", vDir.x, vDir.y, vDir.z);
            I3DGetDriver()->DrawText2D(0, 32, buf, 0, 1.0f);

            sprintf(buf, "Rotation: [%f, %f]", g_fRotX, g_fRotY);
            I3DGetDriver()->DrawText2D(0, 48, buf, 0, 1.0f);

            sprintf(buf, "Human rot: %f", g_fTargetHumanRot);
            I3DGetDriver()->DrawText2D(0, 64, buf, 0, 1.0f);

            sprintf(buf, "Cam dist: %f (dest %f)", g_fCurDistance, g_fDistance);
            I3DGetDriver()->DrawText2D(0, 80, buf, 0, 1.0f);

            sprintf(buf, "Interpol. speed: %f", g_fInterpolationSpeed);
            I3DGetDriver()->DrawText2D(0, 96, buf, 0, 1.0f);

            auto pHuman = g_pMission->m_pGame->m_pHuman;

            if (pHuman) {
                sprintf(buf, "Player base: 0x%p", pHuman);
                I3DGetDriver()->DrawText2D(0, 112, buf, 0, 1.0f);
            }
        }

        g_pD3DDevice->EndScene();
    }

    IGraph__Present_original(pIGraph);
}

void SetPlayerDir(C_human* pPlayer) {
    //pPlayer->m_vWorldDir = DirFromEuler({ g_fRotX + 180.0f, 0, 0 });
}

void SetPlayerShootTarget(C_human* pPlayer) {
    S_vector vPos = g_pMission->m_pGame->m_eCamera.m_pCameraFrame->GetWorldPos(), vDir = g_pMission->m_pGame->m_eCamera.m_pCameraFrame->GetWorldDir();

    S_vector vRelDest = vDir * 100.0f;

    S_vector vRaycastPos, vRaycastNormal;

    g_pCollisionManager->TestLineV(vPos, vRelDest, &vRaycastPos, &vRaycastNormal, 1, 0x80000u);
    //g_pCollisionManager->TestLineV(vPos + vDir * (g_fCurDistance * 1.45f), vRelDest, &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

    if (vRaycastPos.Magnitude() != 0) {
        pPlayer->m_vShootTarget = vRaycastPos;
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
        MessageBox(NULL, "Failed to get the handle of LS3DF.dll", "Error", MB_OK | MB_ICONERROR);
        ExitProcess(1);
    }

    DebugPrintf("LS3DF.dll base: %x\n", ls3df);

    if (MH_Initialize() != MH_OK) {
        MessageBox(NULL, "Failed to initialize MinHook", "Error", MB_OK | MB_ICONERROR);
        ExitProcess(1);
    }

    static ModConfig sModCfg(Utils::GetModPath() + "\\MafiaOrbitCam.ini");

    if (!sModCfg.IsLoaded()) {
        sModCfg.SaveConfig(Utils::GetModPath() + "\\MafiaOrbitCam.ini");
    }

    g_pModConfig = &sModCfg;

    IGraph* pIGraph = GetIGraph();

    uint32_t iVTableAddr = *(uint32_t*)pIGraph;

    void* pAddrPresent = *(void**)(iVTableAddr + 60);
    void* pAddrEndScene = *(void**)(iVTableAddr + 56);

    if (g_pModConfig->bEnableCamOrbit) {
        MH_CreateHook((LPVOID)0x5ED4C0, (LPVOID)&G_Camera__Tick_Hook, (LPVOID*)&G_Camera__Tick_original);
        MH_CreateHook((LPVOID)0x5ED190, (LPVOID)&G_Camera__SetCar_Hook, (LPVOID*)&G_Camera__SetCar_original);

        MH_CreateHook((LPVOID)0x5A0810, (LPVOID)&C_game__Init_Hook, (LPVOID*)&C_game__Init_original);
        MH_CreateHook((LPVOID)0x5B8430, (LPVOID)&C_game__PauseAllSounds_Hook, (LPVOID*)&C_game__PauseAllSounds_original);
        MH_CreateHook((LPVOID)0x5B85F0, (LPVOID)&C_game__PlayAllSounds_Hook, (LPVOID*)&C_game__PlayAllSounds_original);

        MH_CreateHook((LPVOID)0x573E50, (LPVOID)&C_human__PersonAnim_Hook, (LPVOID*)&C_human__PersonAnim_original);
        MH_CreateHook((LPVOID)0x57D670, (LPVOID)&C_human__Go_FrontLeft_Hook, (LPVOID*)&C_human__Go_FrontLeft_original);
        MH_CreateHook((LPVOID)0x57D860, (LPVOID)&C_human__Go_FrontRight_Hook, (LPVOID*)&C_human__Go_FrontRight_original);
        MH_CreateHook((LPVOID)0x57D360, (LPVOID)&C_human__Go_Front_Hook, (LPVOID*)&C_human__Go_Front_original);
        MH_CreateHook((LPVOID)0x57DA50, (LPVOID)&C_human__Go_BackLeft_Hook, (LPVOID*)&C_human__Go_BackLeft_original);
        MH_CreateHook((LPVOID)0x57DC10, (LPVOID)&C_human__Go_BackRight_Hook, (LPVOID*)&C_human__Go_BackRight_original);
        MH_CreateHook((LPVOID)0x57D430, (LPVOID)&C_human__Go_Back_Hook, (LPVOID*)&C_human__Go_Back_original);
        MH_CreateHook((LPVOID)0x57D4D0, (LPVOID)&C_human__Go_Left_Hook, (LPVOID*)&C_human__Go_Left_original);
        MH_CreateHook((LPVOID)0x57D5A0, (LPVOID)&C_human__Go_Right_Hook, (LPVOID*)&C_human__Go_Right_original);
        
        if (g_pModConfig->bEnableDebugStats) {
            MH_CreateHook(pAddrEndScene, (LPVOID)&IGraph__EndScene_Hook, (LPVOID*)&IGraph__EndScene_original);
            MH_CreateHook(pAddrPresent, (LPVOID)&IGraph__Present_Hook, (LPVOID*)&IGraph__Present_original);
        }

        InstallJmpHook(0x58FFB2, (DWORD)&C_player__AI_walk_setDir);
        InstallJmpHook(0x592926, (DWORD)&C_player__AI_drive_setShootDest);

        MH_EnableHook(MH_ALL_HOOKS);
    }
}