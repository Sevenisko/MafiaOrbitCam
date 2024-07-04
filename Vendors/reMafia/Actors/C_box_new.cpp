#include "C_box_new.h"
#include "C_chunk.h"

C_box_new::C_box_new(){
	C_PhThing::C_PhThing();
    m_eActorType = ACT_Physical;
}

bool C_box_new::Init(I3D_frame* pFrame) {
	return ((bool(__thiscall*)(C_box_new*, I3D_frame*))(0x00567540))(this, pFrame);
}

bool C_box_new::SaveData(C_chunk* pChunk) { 
    return ((bool(__thiscall*)(C_box_new*, C_chunk*))(0x00567AB0))(this, pChunk);
}

bool C_box_new::LoadData(C_chunk* pChunk) { 
    //((void(__thiscall*)(C_box_new*, C_chunk*))(0x00567730))(this, pChunk);
	uint8_t uPhysicalType;
	pChunk->Read(&uPhysicalType, 1u);
	
	if(uPhysicalType == 1) {
		pChunk->Read(&m_bIsBox, 1u);
		pChunk->Read(&m_fUnk0, 4u);
		pChunk->Read(&m_fUnk1, 4u);
		pChunk->Read(&m_fMass, 4u);
		pChunk->Read(&m_fFriction, 4u);
		pChunk->Read(&m_fUnk4, 4u);
		pChunk->Read(&mv_UnkVec6.x, 4u);
	}

	if(uPhysicalType == 2)  {
		pChunk->Read(&m_bIsBox, 1u);
		pChunk->Read(&m_fUnk0, 4u);
		pChunk->Read(&m_fUnk1, 4u);
		pChunk->Read(&m_fMass, 4u);
		pChunk->Read(&m_fFriction, 4u);
		pChunk->Read(&m_fUnk4, 4u);
		pChunk->Read(&mv_UnkVec6.x, 4u);
		pChunk->Read(&m_bUnkBool, 1u);
		pChunk->Read(&mv_UnkVec6.y, 4u);
	}

	return true;
}

void C_box_new::DuplicateFrom(C_actor* pOther) {
    ((void(__thiscall*)(C_box_new*, C_actor*))(0x00567C20))(this, pOther);
}

void C_box_new::GameInit() {
    ((void(__thiscall*)(C_box_new*))(0x00567650))(this);   
	/*
	if(m_bIsBox) {
		S_THING_BOX sThingBox;
		this->InitBox(&sThingBox);
	} else {
		S_THING_WHEEL sThingWheel;
		this->InitWheel(&sThingWheel);
	}*/
}

bool C_box_new::Collision(C_actor* pActor, S_vector const& vUnk1, S_vector const& vUnk2, S_vector const& vUnk3, float fUnk, I3D_frame* pOtherFrame) {
    m_bWasHitted = true;
    return C_PhThing::Collision(pActor, vUnk1, vUnk2, vUnk3, fUnk, pOtherFrame);
}

bool C_box_new::Hit(E_hit_type eHitType, S_vector const& vDir, S_vector const& vWorldPos, S_vector const& vUnk3, float fDamage, C_actor* pAttacker, uint32_t uUnk, I3D_frame* pFrame) {
    m_bWasHitted = true;
	return C_PhThing::Hit(eHitType, vDir, vWorldPos, vUnk3, fDamage, pAttacker, uUnk, pFrame);
}
