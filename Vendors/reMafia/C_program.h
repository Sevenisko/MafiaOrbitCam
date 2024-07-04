#pragma once
#include <vector.h>

class I3D_frame;
class C_actor;

class C_program {
public:
    void Init();
    void Done();
    void SetSourceCode(const char* szSource);

    struct S_Instruction {
        unsigned int m_uCommand;
        void* m_pArguments;
    };

    void* m_pVTable;
    char* m_szProgramName;
    char* m_szSourceCode;
	std::vc6_vector<S_Instruction> m_vInstructions;
    int m_iCurInstructionID;
    int m_iCurLine;
    int m_iCurInstructionIndex;
    int m_iUnk8;
    int m_iUnk9;
    void* m_pSubcall;
    int m_iUnk11;
    int m_iUnk12;
    int m_iUnk13;
    int m_iUnk14;
    C_actor* m_pUserActor;
    float* m_fVariables;
    int m_iFloatCount;
    I3D_frame** m_pFrameVariables;
    int m_iFrameCount;
    C_actor** m_pActorVariables;
    int m_iActorCount;
    int m_iUnk15;
    bool m_bUnk;
    bool m_bUnk2;
    int m_iUnk16;
};