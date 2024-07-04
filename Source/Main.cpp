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

#define M_PI 3.1415926535897931

typedef void(__thiscall* G_Camera__Tick)(G_Camera*, uint32_t);
typedef void(__thiscall* G_Camera__SetCar)(G_Camera*, C_car*);
typedef void(__thiscall* G_Camera__SetPlayer)(G_Camera*, C_human*);
typedef bool(__thiscall* C_game__Init)(C_game*);
typedef void(__stdcall* IGraph__Present)(IGraph*);
typedef void(__thiscall* C_human__Go_Func1)(C_human*, bool);
typedef void(__thiscall* C_human__Go_Func2)(C_human*);

G_Camera__Tick G_Camera__Tick_original = nullptr;
G_Camera__SetCar G_Camera__SetCar_original = nullptr;
G_Camera__SetPlayer G_Camera__SetPlayer_original = nullptr;
C_game__Init C_game__Init_original               = nullptr;
IGraph__Present IGraph__Present_original       = nullptr;

C_human__Go_Func1 C_human__Go_FrontLeft_original = nullptr;
C_human__Go_Func1 C_human__Go_FrontRight_original = nullptr;
C_human__Go_Func1 C_human__Go_Front_original      = nullptr;
C_human__Go_Func2 C_human__Go_BackLeft_original   = nullptr;
C_human__Go_Func2 C_human__Go_BackRight_original   = nullptr;
C_human__Go_Func2 C_human__Go_Back_original   = nullptr;
C_human__Go_Func1 C_human__Go_Left_original        = nullptr;
C_human__Go_Func1 C_human__Go_Right_original        = nullptr;

HMODULE ls3df = nullptr;

IDirect3DDevice8*& g_pD3DDevice = *(IDirect3DDevice8**)(0x101C597C);

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

float g_fRotX = 0, g_fRotY = 0, g_fDistance = 2.0f, g_fCurDistance = g_fDistance, g_fInterpolationSpeed = 8.5f;

float g_fCurHumanRot = 0, g_fTargetHumanRot = 0;

float g_fFrontFaceCurTime = 0, g_fFrontFaceTime = 3.5f;

float g_fSensitivity = 0.1f;

S_vector g_vTargetPos, g_vCurPos;

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

    // Step 1: Position and orbital positioníng

    float fOffX = std::sin(Radians(g_fRotX)) * std::cos(Radians(g_fRotY));
    float fOffY = std::sin(Radians(g_fRotY));
    float fOffZ = std::cos(Radians(g_fRotX)) * std::cos(Radians(g_fRotY));

    float fCamX = vCenterPos.x + fOffX * fRadius;
    float fCamY = vCenterPos.y + fOffY * fRadius;
    float fCamZ = vCenterPos.z + fOffZ * fRadius;

    vPos = { fCamX, fCamY, fCamZ };

    // Step 2: Rotation calculation
    vDir.SetNormalized(vCenterPos - vPos);
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

            sprintf(buf, "Cam dist: %f (dest %f)", g_fCurDistance, g_fDistance);
            I3DGetDriver()->DrawText2D(0, 64, buf, 0, 1.0f);

            sprintf(buf, "Interpol. speed: %f", g_fInterpolationSpeed);
            I3DGetDriver()->DrawText2D(0, 80, buf, 0, 1.0f);
        }

        g_pD3DDevice->EndScene();
    }

    IGraph__Present_original(pIGraph);
}

void __fastcall G_Camera__SetPlayer_Hook(G_Camera* _this, DWORD edx, C_human* pHuman) {
    G_Camera__SetPlayer_original(_this, pHuman);
}

bool g_bFirstTime       = true;
bool g_bIsInterpolating = false;
bool g_bIsInterpolatingRot = false;

bool __fastcall C_game__Init_Hook(C_game* _this, DWORD edx) {
    bool ret = C_game__Init_original(_this);

    g_bFirstTime       = true;

    return ret;
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

void __fastcall G_Camera__Tick_Hook(G_Camera* _this, DWORD edx, uint32_t uTickTime) {
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
    case 12: g_fInterpolationSpeed = 6.5f;
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

    g_fRotX += iMouseX * g_fSensitivity;
    g_fRotY += iMouseY * g_fSensitivity;

    /*g_fDistance -= pIGraph->Mouse_rz() * 0.005f;

    if (g_fDistance < 4.5f)
        g_fDistance = 4.5f;
    else if (g_fDistance > 12.5f)
        g_fDistance = 12.5f;
    */

    bool bIsCarCameraMode = (_this->m_iCameraState == 7 || _this->m_iCameraState == 8 || _this->m_iCameraState == 9);
    bool bIsPlayerCameraMode = (_this->m_iCameraState == 1 || _this->m_iCameraState == 5);

    g_vCurPos      = LerpVec(g_vCurPos, g_vTargetPos, g_fInterpolationSpeed * float(uTickTime / 1000.0f));
    g_fCurDistance = Lerpf(g_fCurDistance, g_fDistance, g_fInterpolationSpeed * float(uTickTime / 1000.0f));

    if ((g_vTargetPos - g_vCurPos).Magnitude() <= 0.05f) {
        g_bIsInterpolating = false;
    }

    if (pCar && bIsCarCameraMode){
        if (pCar->m_sVehicle.m_fSpeed >= 2.5f && !g_bIsInterpolatingRot) {
            g_fFrontFaceCurTime += float(uTickTime / 1000.0f);

            if (g_fFrontFaceCurTime > g_fFrontFaceTime) {
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
            vCenter = { vCarPos.x, vCarPos.y + 1.5f, vCarPos.z };
            g_vCurPos = vCenter;
        }

        if (_this->m_pCameraFrame) {
            CalcOrbit(vCenter, pCar->GetDir(), vPos, vDir, g_fCurDistance, g_fRotX, g_fRotY);

            S_vector vRaycastPos, vRaycastNormal;

            g_pCollisionManager->TestLineV(vPos + (vDir * g_fCurDistance), (vDir * -(g_fCurDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

            S_vector finalPos { 0, 0, 0 };

            if (vRaycastPos.Magnitude() > 0) {
                finalPos += vRaycastPos + (vDir * 0.15f);
            }

            if (finalPos.Magnitude() == 0) 
            {
                _this->m_pCameraFrame->SetWorldPos(vPos);
            } else {
                _this->m_pCameraFrame->SetWorldPos(finalPos);
            }
             
            _this->m_pCameraFrame->SetWorldDir(vDir, pCar->m_sVehicle.m_vAngularVelocity.y * 0.075f);
            _this->m_pCameraFrame->Update();
        }
    } else if (pHuman && bIsPlayerCameraMode) {
        S_vector vHumanPos(pHuman->GetPos()), vCenter, vPos, vDir;

        bool bHasNeck = false;

        if (pHuman->GetFrame() != nullptr) {
            I3D_frame* pNeck = pHuman->GetFrame()->FindChildFrame("neck");

            if (pNeck) {
                bHasNeck  = true;
                vHumanPos = pNeck->GetWorldPos();
            }
        }

        if (g_bIsInterpolating) {
            vCenter = g_vCurPos;
        } else {
            vCenter = { vHumanPos.x, vHumanPos.y + (bHasNeck ? 0.25f : 1.85f), vHumanPos.z };
            g_vCurPos = vCenter;
        }

        g_fCurHumanRot = LerpAngle(g_fCurHumanRot, g_fTargetHumanRot, 4.5f * float(uTickTime / 1000.0f));

        pHuman->m_vWorldDir = DirFromEuler({ g_fCurHumanRot, 0, 0 });

        if (_this->m_pCameraFrame) {
            CalcOrbit(vCenter, pHuman->GetDir(), vPos, vDir, g_fDistance, g_fRotX, g_fRotY);

            S_vector vRaycastPos, vRaycastNormal;

            g_pCollisionManager->TestLineV(vPos + (vDir * g_fDistance), (vDir * -(g_fDistance + 0.25f)), &vRaycastPos, &vRaycastNormal, 0xFFFFFFFF, 0);

            S_vector finalPos { 0, 0, 0 };

            if (vRaycastPos.Magnitude() > 0) {
                finalPos += vRaycastPos + (vDir * 0.15f);
            }

            if (finalPos.Magnitude() == 0) {
                _this->m_pCameraFrame->SetWorldPos(vPos);
            } else {
                _this->m_pCameraFrame->SetWorldPos(finalPos);
            }

            _this->m_pCameraFrame->SetWorldDir(vDir, 0);
            _this->m_pCameraFrame->Update();
        }
    } else {
        G_Camera__Tick_original(_this, uTickTime);
    }
}

void __fastcall C_human__Go_FrontLeft_Hook(C_human* _this, DWORD edx, bool a2) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) - 45.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_FrontRight_Hook(C_human* _this, DWORD edx, bool a2) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) + 45.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_Front_Hook(C_human* _this, DWORD edx, bool a2) {
    g_fTargetHumanRot = (g_fRotX - 180.0f);

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_BackLeft_Hook(C_human* _this, DWORD edx) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f + 45.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_BackRight_Hook(C_human* _this, DWORD edx) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f - 45.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_Back_Hook(C_human* _this, DWORD edx) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) + 180.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_Left_Hook(C_human* _this, DWORD edx, bool a2) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) - 90.0f;

    C_human__Go_Front_original(_this, true);
}

void __fastcall C_human__Go_Right_Hook(C_human* _this, DWORD edx, bool a2) {
    g_fTargetHumanRot = (g_fRotX - 180.0f) + 90.0f;

    C_human__Go_Front_original(_this, true);
}

void SetPlayerDir(C_human* pPlayer) {
    //pPlayer->m_vWorldDir = DirFromEuler({ g_fRotX + 180.0f, 0, 0 });
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

    IGraph* pIGraph = GetIGraph();

    uint32_t iVTableAddr = *(uint32_t*)pIGraph;

    void* pAddr = *(void**)(iVTableAddr + 60);

    MH_CreateHook((LPVOID)0x5ED4C0, (LPVOID)&G_Camera__Tick_Hook, (LPVOID*)&G_Camera__Tick_original);
    MH_CreateHook((LPVOID)0x5ED190, (LPVOID)&G_Camera__SetCar_Hook, (LPVOID*)&G_Camera__SetCar_original);
    MH_CreateHook((LPVOID)0x5A0810, (LPVOID)&C_game__Init_Hook, (LPVOID*)&C_game__Init_original);

    MH_CreateHook((LPVOID)0x57D670, (LPVOID)&C_human__Go_FrontLeft_Hook, (LPVOID*)&C_human__Go_FrontLeft_original);
    MH_CreateHook((LPVOID)0x57D860, (LPVOID)&C_human__Go_FrontRight_Hook, (LPVOID*)&C_human__Go_FrontRight_original);
    MH_CreateHook((LPVOID)0x57D360, (LPVOID)&C_human__Go_Front_Hook, (LPVOID*)&C_human__Go_Front_original);
    MH_CreateHook((LPVOID)0x57DA50, (LPVOID)&C_human__Go_BackLeft_Hook, (LPVOID*)&C_human__Go_BackLeft_original);
    MH_CreateHook((LPVOID)0x57DC10, (LPVOID)&C_human__Go_BackRight_Hook, (LPVOID*)&C_human__Go_BackRight_original);
    MH_CreateHook((LPVOID)0x57D430, (LPVOID)&C_human__Go_Back_Hook, (LPVOID*)&C_human__Go_Back_original);
    MH_CreateHook((LPVOID)0x57D4D0, (LPVOID)&C_human__Go_Left_Hook, (LPVOID*)&C_human__Go_Left_original);
    MH_CreateHook((LPVOID)0x57D5A0, (LPVOID)&C_human__Go_Right_Hook, (LPVOID*)&C_human__Go_Right_original);

    #ifdef _DEBUG
    MH_CreateHook(pAddr, (LPVOID)&IGraph__Present_Hook, (LPVOID*)&IGraph__Present_original);
    #endif

    InstallJmpHook(0x58FFB2, (DWORD)&C_player__AI_walk_setDir);

    MH_EnableHook(MH_ALL_HOOKS);
}