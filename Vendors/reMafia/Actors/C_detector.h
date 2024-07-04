#pragma once

#include "C_actor.h"
#include <C_using_object.h>
#include <C_program.h>

class C_detector : public C_actor {
public:
    C_program m_sProgram;
    bool m_bSignal;
    bool m_bHasUsingObject;
    bool m_bIsWaitingForHit;
    bool m_bHasEvent;
    C_using_object::S_using_object m_sUsingObject;
    int m_iUnk1;
    tDynamicCollObject** m_ppColls;
    S_matrix* m_pWorldMatrices;
    S_matrix* m_pLocalMatrices;
    int m_iNumColls;
};