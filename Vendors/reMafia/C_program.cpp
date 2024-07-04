#include "C_program.h"

void C_program::Init() {
    ((void(__thiscall*)(C_program*))(0x461580))(this);
}

void C_program::Done() {
    ((void(__thiscall*)(C_program*))(0x4616A0))(this);
}

void C_program::SetSourceCode(const char* szSource) {
    ((void(__thiscall*)(C_program*, const char*))(0x461530))(this, szSource);
}
