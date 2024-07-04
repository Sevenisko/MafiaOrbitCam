#include "C_actor.h"
#include "C_mission.h"

C_actor::C_actor() {
    uint32_t vtable = *(uint32_t*)this;
    ((void(__thiscall*)(C_actor*))(0x405D20))(this);
    *(uint32_t*)this = vtable;
}

void C_actor::AI(uint32_t uFrameTime) { }

void C_actor::Update(uint32_t uFrameTime) { }

void C_actor::NetDirect(uint32_t uFrameTime) { }

C_actor::~C_actor() { 
	((void(__thiscall*)(C_actor*))(0x405D90))(this);
}

void C_actor::Release() {
    if (--m_uRefCount)
        return;

    delete this;   
}

bool C_actor::Init(I3D_frame* pFrame) {
	return ((bool(__thiscall*)(C_actor*, I3D_frame*))(0x00405DD0))(this, pFrame);

    if (!pFrame)
        return false;

    C_actor** pUserData = (C_actor**)pFrame->_pad0;

    if (pUserData) {
        C_actor* pOtherActor = *pUserData;

        if (pOtherActor != this)
            return false;
    }

    m_pFrame      = pFrame;
    pFrame->_pad0 = 0;
    m_uID         = g_pMission->m_vActors.size() + 1;
    m_uUnk6       = 0;

    if (m_pFrame) {
        I3D_frame* pTargetE = m_pFrame->FindChildFrame("targetE", 64);
        if (pTargetE) {
            pTargetE->SetOn(false);
        }
        I3D_frame* pTargetN = m_pFrame->FindChildFrame("targetN", 64);
        if (pTargetN) {
            pTargetN->SetOn(false);
        }
    }

    m_eState = IDLE;
    return true;
}

bool C_actor::SaveData(C_chunk* pChunk) { return false; }

bool C_actor::LoadData(C_chunk* pChunk) { return false; }

void C_actor::DuplicateFrom(C_actor* pOther) { }

int C_actor::SaveGameGetSize() {
    return ((int(__thiscall*)(C_actor*))(0x406770))(this);
}

bool C_actor::SaveGameSave(void** pBuf) {
    return ((bool(__thiscall*)(C_actor*, void**))(0x406870))(this, pBuf);
}

bool C_actor::SaveGameLoad(void** pBuf) {
    return ((bool(__thiscall*)(C_actor*, void**))(0x406B00))(this, pBuf);
}

void C_actor::GameInit() {
    ((void(__thiscall*)(C_actor*))(0x405FA0))(this);
}

void C_actor::GameDone() {
    ((void(__thiscall*)(C_actor*))(0x4063E0))(this);
}

void C_actor::ChangeState() { }

void C_actor::GetActorSpeed(S_vector& vSpeed) {
    ((void(__thiscall*)(C_actor*, S_vector&))(0x4091F0))(this, vSpeed);
}

void C_actor::Death() { }

bool C_actor::Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pOtherFrame) {
    return false;
}

bool C_actor::Hit(E_hit_type eHitType, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) {
    return false;
}

int32_t C_actor::GetMaterialID(I3D_frame* pFrame) {
    return 0;
}

bool C_actor::IsPossibleColl(C_actor* pActor, S_vector vUnk1, S_vector vUnk2, float fUnk, float& fUnk2, C_actor** pActorOut) {
    return false;
}

bool C_actor::GetBoundPoints2D(std::vc6_vector<S_vector2>* vBoundPoints, I3D_frame* pFrame) {
    return false;
}

C_program* C_actor::GetProgram() {
    return NULL;
}

bool C_actor::cbCollTester(tDynamicCollObject* pDynamicCollObj) {
    return false;
}

void C_actor::SoundsChange(int32_t iUnk) { }

void C_actor::ChangeSceneInit() { }

void C_actor::ChangeSceneDone() { }

void C_actor::SetActState(E_act_state state) {
	 ((void(__thiscall*)(C_actor*, E_act_state))(0x406DA0))(this, state);
}

const char* GetActorTypeName(EActorType eActorType) {
    switch (eActorType) {
        case ACT_Hidden: return "Hidden";
        case ACT_Player: return "Player";
        case ACT_Car: return "Car";
        case ACT_Script: return "Script";
        case ACT_Door: return "Door";
        case ACT_Railway: return "Railway";
        case ACT_VillaObject: return "VillaObject";
        case ACT_Bottle: return "Bottle";
        case ACT_Traffic: return "Traffic";
        case ACT_Pedestrians: return "Pedestrians";
        case ACT_Bridge: return "Bridge";
        case ACT_Dog: return "Dog";
        case ACT_Airplane: return "Airplane";
        case ACT_Turnout: return "Turnout";
        case ACT_Pumper: return "Pumper";
        case ACT_Human: return "Human";
        case ACT_RaceCamera: return "RaceCamera";
        case ACT_Wagon: return "Wagon";
        case ACT_Irenka: return "Irenka";
        case ACT_PublicPhysics: return "PublicPhysics";
        case ACT_Shot: return "Shot";
        case ACT_Clock: return "Clock";
        case ACT_Physical: return "Physical";
        case ACT_Truck: return "Truck";
        case ACT_RailGenerator: return "RailGenerator";
        default: break;
    }
    return "Unknown";
}
