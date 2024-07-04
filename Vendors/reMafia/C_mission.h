#pragma once
#include "common.h"
#include <vector.h>

class C_actor;
class C_anim_model;
class C_ParticleManager;
class C_game;
class C_tshift;
class C_web_path;
class C_roads;
class C_program;
class I3D_sound;
class I3D_scene;

class C_mission {
public:
    void Close();
    bool Open(const char* szName, bool bUnk1, uint32_t uUnk, bool bUnk2);
	C_actor* CreateActor(enum EActorType eType);
    void AddActor(C_actor* pActor, bool bUnk);

    static void Hook();

    std::vc6_vector<C_actor*> m_vActors;
    I3D_scene* m_pScene;
    std::vc6_vector<C_anim_model*> m_vAnimModels;
    C_game* m_pGame;
    C_tshift* m_pTShift;
    C_web_path* m_pWebPath;
    C_roads* m_pRoads;
	std::vc6_vector<C_actor*> m_vSceneActors;
    std::vc6_vector<C_actor*> m_vActors2;
    std::vc6_vector<C_program*> m_vPrograms;
    uint8_t pad6[4];
    char* m_szMissionName;
    C_ParticleManager* m_pParticleManager;
    uint8_t pad7[16];
    std::vc6_vector<C_actor*> m_vTransparentObjects;
    uint8_t pad8[28];
    std::vc6_vector<C_actor*> m_vModels;
    std::vc6_vector<C_actor*> m_vActiveActors;
    std::vc6_vector<C_actor*> m_vActorSounds;
};

static C_mission* g_pMission = *(C_mission**)0x63788C;