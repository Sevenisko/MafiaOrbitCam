#include "C_actor.h"

class C_clock: public C_actor {
public:
    C_clock();
    void AI(uint32_t iFrameTime) override;
    bool Init(I3D_frame* pFrame) override;
    void GameInit() override;
    void GameDone() override;
private:
    I3D_frame* m_pMalaRucicka;
    I3D_frame* m_pVelkaRucicka;
    I3D_frame* m_pSekundovaRucicka;
    float m_fSecond;
    float m_fHour;
    float m_fMinute;
};