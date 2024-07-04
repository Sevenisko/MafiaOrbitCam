#pragma once
#include "common.h"
#include "I3D_math.h"

class C_actor;
class I3D_frame;

class C_using_object {
public:
	struct S_using_object {
		S_vector m_vPosition;
		float m_fDistance;
		int iUnkInt1;
		uint32_t m_uFlags;
		I3D_frame *m_pFrame;
		C_actor *m_pActor;
		int iUnkInt5;
		int iUnkInt6;
		int iUnkInt7;
		int32_t m_iUseTextID;
		int iUnkInt9;
	};
};