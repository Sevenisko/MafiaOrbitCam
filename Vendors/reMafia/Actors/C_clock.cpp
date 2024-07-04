#include "C_clock.h"
#include <I3D_math.h>

C_clock::C_clock(): 
    C_actor(), 
    m_pMalaRucicka(NULL), 
    m_pVelkaRucicka(NULL), 
    m_pSekundovaRucicka(NULL), 
    m_fSecond(0.0f), 
    m_fHour(0.0f), 
    m_fMinute(0.0f) { 
    m_eActorType = ACT_Clock;
}

void C_clock::AI(uint32_t uFrameTime) {
    SYSTEMTIME sSystemTime;
    GetLocalTime(&sSystemTime);

    S_vector vDir(0.0f, 1.0f, 0.0f);

    m_fHour   = -(sSystemTime.wHour * 0.5f) - 3.6f;
    m_fMinute = -(sSystemTime.wMinute * 0.1f) - 3.3f;
    m_fSecond = -(sSystemTime.wSecond * 0.1f) - 3.3f;

    m_pMalaRucicka->SetDir(vDir, m_fHour);
    m_pVelkaRucicka->SetDir(vDir, m_fMinute);
    if (m_pSekundovaRucicka)
        m_pSekundovaRucicka->SetDir(vDir, m_fSecond);
}

bool C_clock::Init(I3D_frame* pFrame) {
    if (pFrame && C_actor::Init(pFrame)) {
        m_pMalaRucicka      = m_pFrame->FindChildFrame("Mala", 1);
        m_pVelkaRucicka     = m_pFrame->FindChildFrame("Velka", 1);
        m_pSekundovaRucicka = m_pFrame->FindChildFrame("Sekundova", 1);
        if (m_pMalaRucicka || m_pVelkaRucicka) {
            return true;
        }
    }

    return false;
}

void C_clock::GameInit() {
    m_fHour = m_fMinute = m_fSecond = 0;
    C_actor::GameInit();
    m_bIsInited = true;

    SYSTEMTIME sSystemTime;
    GetLocalTime(&sSystemTime);

    S_vector vDir(0.0f, 1.0f, 0.0f);

    m_fHour   = -(sSystemTime.wHour * 0.5f) - 3.6f;
    m_fMinute = -(sSystemTime.wMinute * 0.1f) - 3.3f;
    m_fSecond = -(sSystemTime.wSecond * 0.1f) - 3.3f;

    m_pMalaRucicka->SetDir(vDir, m_fHour);
    m_pVelkaRucicka->SetDir(vDir, m_fMinute);
    if (m_pSekundovaRucicka)
        m_pSekundovaRucicka->SetDir(vDir, m_fSecond);
}

void C_clock::GameDone() {
    C_actor::GameDone();
}