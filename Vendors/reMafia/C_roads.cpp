#include "C_roads.h"

void C_roads::SortNodesByX() {
    ((void(__thiscall*)(C_roads*))(0x55C890))(this);
}

bool C_roads::SaveData(C_chunk* pChunk) {
    return ((bool(__thiscall*)(C_roads*, C_chunk*))(0x559D40))(this, pChunk);
}

bool C_roads::LoadData(C_chunk* pChunk) {
    return ((bool(__thiscall*)(C_roads*, C_chunk*))(0x559E60))(this, pChunk);
}

void C_roads::RecompileDistances() {
    ((void(__thiscall*)(C_roads*))(0x55B370))(this);
}

int16_t C_roads::GetNearNode(const S_vector& vPos) {
    return ((int16_t(__thiscall*)(C_roads*, const S_vector&))(0x55B370))(this, vPos);
}

int16_t C_roads::FindCrossing(int16_t firstId, int16_t secondId) {
    return ((int16_t(__thiscall*)(C_roads*, int16_t, int16_t))(0x55B570))(this, firstId, secondId);
}
