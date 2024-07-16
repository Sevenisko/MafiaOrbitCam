#pragma once
#include "G_Camera.h"
#include "common.h"
#include <vector.h>

class C_mission;
class C_player;
class C_human;
class C_actor;
class C_program;
class C_using_object;
class C_schvestky;

class C_game {
public:
    void Done();
    bool Init();
	void AddTemporaryActor(C_actor* pActor);
    void RemoveTemporaryActor(C_actor* pActor);

    char pad0[68];
    C_mission* m_pMission;
    int m_iState;
    G_Camera m_eCamera;
    char pad2[8];
    C_human* m_pHuman;
	std::vc6_vector<C_actor*> m_vActors;
    char pad3[36];
	std::vc6_vector<C_actor*> m_vTempActors;
    char pad4[10428];
    C_using_object* m_pUsingObject;
    char pad5[268];
	std::vc6_vector<C_program*> m_vPrograms;
    int m_iUnk;
    char pad6[656];
    C_schvestky* m_pSchvestky;
    char pad7[2202];
    bool m_bUpdateScore;
    char pad8[3];
    bool m_bScoreOn;
    int m_iGameScore;
    char pad9[119];
};