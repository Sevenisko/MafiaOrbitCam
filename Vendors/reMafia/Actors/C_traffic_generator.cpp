#include "C_traffic_generator.h"

C_traffic_generator::C_traffic_generator() {
	printf("b - %s\n", __FUNCTION__);
	uint32_t vtable = *(uint32_t*)((uint32_t)this);
	((void(__thiscall*)(C_traffic_generator*))(0x0044D410))(this);
	*(uint32_t*)((uint32_t)this) = vtable;	
	printf("e - %s\n", __FUNCTION__);	
}

C_traffic_generator::~C_traffic_generator() {
	printf("b - %s\n", __FUNCTION__);
	//uint32_t vtable = *(uint32_t*)((uint32_t)this);
	//((void(__thiscall*)(C_traffic_generator*))(0x0044D483))(this);
	//*(uint32_t*)((uint32_t)this) = vtable;
	printf("e - %s\n", __FUNCTION__);	
}

void C_traffic_generator::AI(uint32_t uFrameTime) {
	printf("b - %s\n", __FUNCTION__);
	((void(__thiscall*)(C_traffic_generator*, int32_t))(0x0044EDB0))(this, uFrameTime);
	printf("e - %s\n", __FUNCTION__);
}

bool C_traffic_generator::Init(I3D_frame* pFrame) {
	printf("b - %s\n", __FUNCTION__);
	bool res = C_actor::Init(pFrame);
	printf("e - %s\n", __FUNCTION__);
	return res;
}

bool C_traffic_generator::SaveData(C_chunk* pChunk) {
	printf("b - %s\n", __FUNCTION__);
	bool res = ((bool(__thiscall*)(C_traffic_generator*, C_chunk*))(0x0044D670))(this, pChunk);
	printf("e - %s\n", __FUNCTION__);
	return res;
}

bool C_traffic_generator::LoadData(C_chunk* pChunk) {
	printf("b - %s\n", __FUNCTION__);
	bool res = ((bool(__thiscall*)(C_traffic_generator*, C_chunk*))(0x0044D820))(this, pChunk);
	printf("e - %s\n", __FUNCTION__);
	return res;
}

void C_traffic_generator::DuplicateFrom(C_actor* pOther) {
	printf("b - %s\n", __FUNCTION__);
	((void(__thiscall*)(C_traffic_generator*, C_actor*))(0x0044D510))(this, pOther);
	printf("e - %s\n", __FUNCTION__);
}

int32_t C_traffic_generator::SaveGameGetSize() {
	printf("%s\n", __FUNCTION__);
	return ((int32_t(__thiscall*)(C_traffic_generator*))(0x0044DE00))(this);
}

bool C_traffic_generator::SaveGameSave(void** pBuf) {
	printf("%s\n", __FUNCTION__);
	return ((bool(__thiscall*)(C_traffic_generator*, void**))(0x0044DE20))(this, pBuf);
}

bool C_traffic_generator::SaveGameLoad(void** pBuf) {
	printf("%s\n", __FUNCTION__);
	return ((bool(__thiscall*)(C_traffic_generator*, void**))(0x0044E120))(this, pBuf);
}

void C_traffic_generator::GameInit() {
	printf("%s\n", __FUNCTION__);
	((void(__thiscall*)(C_traffic_generator*))(0x0044E500))(this);
	printf("e - %s\n", __FUNCTION__);
}

void C_traffic_generator::GameDone() {
	printf("b - %s\n", __FUNCTION__);
	((void(__thiscall*)(C_traffic_generator*))(0x0044E7F0))(this);
	printf("e - %s\n", __FUNCTION__);
}