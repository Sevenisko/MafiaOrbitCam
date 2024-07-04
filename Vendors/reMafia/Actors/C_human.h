#pragma once
#include "C_actor.h"
#include "G_Inventory.h"
#include "g_collision.h"
#include <string>

class C_car;

struct C_human_properties {
    float m_fPropStrength;
    float m_fPropHealth;
    float m_fPropHealthHandL;
    float m_fPropHealthHandR;
    float m_fPropHealthLegL;
    float m_fPropHealthLegR;
    float m_fPropReactions;
    float m_fPropSpeed;
    float m_fPropAggresivity;
    float m_fPropIntelligence;
    float m_fPropShooting;
    float m_fPropSight;
    float m_fPropHearing;
    float m_fPropDriving;
    float m_fPropMass;
    float m_fPropMorale;
};

enum CharacterVoice {
    CV_Paulie     = 0,
    CV_Sam        = 1,
    CV_Salieri    = 2,
    CV_Tommy      = 3,
    CV_Frank      = 4,
    CV_Luigi      = 5,
    CV_Ralph      = 6,
    CV_Vincenzo   = 7,
    CV_Man        = 8,
    CV_Salvatore  = 9,
    CV_Cop0       = 10,
    CV_Com1       = 11,
    CV_PortGuard0 = 12,
    CV_PortGuard1 = 13,
    CV_PortGuard2 = 14,
    CV_PortGuard3 = 15,
    CV_PortGuard4 = 16,
    CV_Enemy0     = 17,
    CV_Enemy1     = 18,
    CV_Enemy2     = 19,
    CV_Enemy3     = 20,
    CV_Enemy4     = 21,
    CV_Hoolig     = 22,
    CV_Sergio     = 23,
    CV_Enemy5     = 24,
    CV_Joe        = 25,
    CV_PedMan0    = 26,
    CV_PedMan1    = 27,
    CV_PedMan2    = 28,
    CV_PedWoman0  = 29,
    CV_PedWoman1  = 30,
    CV_PedWoman2  = 31,
    CV_NoVoice    = 32,
    CV_Lucas      = 33,
};

static char* szCharacterVoiceNames[] = { "Paulie", "Sam",    "Salieri", "Tommy",  "Frank",  "Luigi",  "Ralph",  "Vincenzo", "Man", "Salvatore", "Cop0",    "Com1",    "PortGuard0", "PortGuard1", "PortGuard2", "PortGuard3", "PortGuard4",
                                         "Enemy0", "Enemy1", "Enemy2",  "Enemy3", "Enemy4", "Hoolig", "Sergio", "Enemy5",   "Joe", "PedMan0",   "PedMan1", "PedMan2", "PedWoman0",  "PedWoman1",  "PedWoman2",  "NoVoice",    "Lucas" };

class C_human: public C_actor {
public:
    int m_iAnimID;
    int m_iAnimState;
    uint8_t pad0[28];
    uint32_t m_uFlags;
    C_actor* m_pUsedActorEnter;
    C_actor* m_pUsedActorLeave;
    C_car* m_pCurrentCar;
    uint8_t pad1[4];
    C_car* m_pCurrentCar2;
    int m_iSeatID;
    uint8_t pad2[12];
    void* m_pAnimationMachine;
    uint8_t pad3[268];
    bool m_bIsDead;
    uint8_t pad3_0_0[23];
    bool m_bIsCrouching;
    bool m_bIsAiming;
    uint8_t pad3_0_1[26];
    S_vector m_vShootTarget;
    uint8_t pad3_0_2[36];
    bool m_bDoChangeWeaponModel;
    uint8_t pad3_0_3[15];
    int m_iDeathAnimID;
    uint8_t pad3_0_4[152];
    void* m_pShotSkeleton;
    uint8_t pad3_0_5[300];
    int m_iCanWork;
    uint8_t pad3_0_6[80];
    int m_iUnk2;
    uint8_t pad3_0_7[24];
    void* m_pFakeShadow;
    G_Inventory sInventory;
    I3D_frame* m_pWeaponR;
    I3D_frame* m_pWeaponL;
    I3D_frame* m_pGunR;
    I3D_frame* m_pGunL;
    I3D_frame* m_pTargetR;
    I3D_frame* m_pTargetL;
    I3D_frame* m_pBaseMesh;
    uint8_t pad4[8];
    I3D_frame* m_pTargetE;
    I3D_frame* m_pTargetN;
    uint8_t pad5[84];
    int m_iUnk3;
    uint8_t pad5_1[24];
    C_human_properties m_sInitProperties;
    C_human_properties m_sProperties;
    CharacterVoice m_eVoice;
	std::vc6_vector<tDynamicCollObject*> m_vCollisions;
    bool m_bHasCollisions;
    uint8_t pad7[67];
    int m_iWingmanID;
    uint8_t pad7_0_2[4];
    I3D_frame* m_pUnkFrame;
    uint8_t pad7_0_1[20];
    I3D_frame* m_pBox;
    uint8_t pad7_0_0[4];
    I3D_frame* m_pDeadBody;
    uint8_t pad7_0[663];
    void* m_pUnkAudioSource1;
    uint8_t pad7_1[152];
    S_vector m_vDeathPos1;
    S_vector m_vDeathPos2;
    float m_fUnk2;
    float m_fUnk3;
    uint8_t pad7_2[28];
    C_actor* m_pAttacker;
    bool m_bLimitedSpeed;
    bool m_bIsGangsta;
    uint8_t pad9[34];
    
    friend class C_debug;
};