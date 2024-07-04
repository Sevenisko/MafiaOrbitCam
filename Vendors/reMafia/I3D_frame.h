#pragma once
#include "C_str.hpp"
#include "LS3D.h"

enum I3D_FRAME_FAGS { FRMFLAGS_ON = 0x1, 
    FRMFLAGS_VALID_WMATRIX = 0x20,
    FRMFLAGS_IN_FFTREE = 0x20000000, 
    FRMFLAGS_UNK1 = 0x80000000 
};

class I3D_frame {
public:
    I3D_frame() {};
    I3DMETHOD_(int32_t, Release)() {
        return ((int32_t(__thiscall*)(I3D_frame*))(0x1001AAE0))(this);
    }
    I3DMETHOD_(void, SetWorldPos)(const S_vector& vPos) {
        ((void(__stdcall*)(I3D_frame*, const S_vector&))(0x1001B160))(this, vPos);
    }
    I3DMETHOD_(void, SetWorldPosDir)(const S_vector& vPos, const S_vector& vDir) {
        ((void(__stdcall*)(I3D_frame*, const S_vector&, const S_vector&))(0x1001B230))(this, vPos, vDir);
    }
    I3DMETHOD_(void, SetDir)(const S_vector& vAxis, float fRoll) {
        ((void(__stdcall*)(I3D_frame*, const S_vector&, float))(0x1001AB50))(this, vAxis, fRoll);
    }
    I3DMETHOD_(void, SetWorldDir)(const S_vector& vAxis, float fRoll) {
        ((void(__stdcall*)(I3D_frame*, const S_vector&, float))(0x1001B040))(this, vAxis, fRoll);
    }
    I3DMETHOD_(void, Tick)(int32_t iTime) {

    }
    I3DMETHOD_(void, Update)() {
        ((void(__stdcall*)(I3D_frame*))(0x1001CA60))(this);
    }
    I3DMETHOD_(void, SetCallback)(I3D_CALLBACK pCallback, uint32_t uUnk) { 
        ((void(__stdcall*)(I3D_frame*, I3D_CALLBACK, uint32_t))(0x1001B3C0))(this, pCallback, uUnk);
    }
    I3DMETHOD(SetProperty)(const char* pSzProperty) { 
        ((void(__stdcall*)(I3D_frame*, const char*))(0x1001C700))(this, pSzProperty);
    }
    I3DMETHOD_(void, SetOn)(bool bOn) { 
        ((void(__stdcall*)(I3D_frame*, bool))(0x1001B400))(this, bOn);
    }
    I3DMETHOD(SetName)(const char* pSzName) { 
        return ((LS3D_RESULT(__stdcall*)(I3D_frame*, const char*))(0x1001B6F0))(this, pSzName);
    }
    I3DMETHOD(LinkTo)(I3D_frame* pParent, uint32_t uFlags) { 
        return ((LS3D_RESULT(__stdcall*)(I3D_frame*, I3D_frame*, uint32_t))(0x1001B9D0))(this, pParent, uFlags);
    }
    I3DMETHOD_(I3D_frame*, GetChild)(uint32_t uIndex) { 
        return ((I3D_frame*(__stdcall*)(I3D_frame*, uint32_t))(0x1001BFD0))(this, uIndex);
    }
    I3DMETHOD(EnumFrames)(I3D_ENUMPROC* pProc, uint32_t uUser, uint32_t uFlags = ENUMF_ALL, const char* pSzMask = NULL) { 
        return ((LS3D_RESULT(__stdcall*)(I3D_frame*, I3D_ENUMPROC*, uint32_t, uint32_t, const char*))(0x1001C2A0))(this, pProc, uUser, uFlags, pSzMask);
    }
    I3DMETHOD_(I3D_frame*, FindChildFrame)(const char* pSzFrameName, uint32_t uFlags = ENUMF_ALL) { 
        return ((I3D_frame*(__stdcall*)(I3D_frame*, const char*, uint32_t))(0x1001C330))(this, pSzFrameName, uFlags);
    }
    I3DMETHOD(Duplicate)(const I3D_frame* pFrame) {
        return ((LS3D_RESULT(__stdcall*)(I3D_frame*, const I3D_frame*))(0x1001C5B0))(this, pFrame);
    }

    // TODO(DavoSK): find out when compiler is generating scalar deleting destructor
    // in original game destructor is generated here after 'Duplicate' and as scalar deleting constructor
    // here MSVC generates vector deleting consturctor this can be a problem
    virtual ~I3D_frame() {

    }

    // NOTE(DavoSK): theese 3 following functions are __thiscall calling convention (weird)
    virtual void UpdateV() {

    }
    virtual void SaveImage() {

    }
    virtual void LoadImageA() {

    }

    void RemoveFromOwner() {

    }
    void RemoveFromFFTree() {
        ((void(__thiscall*)(I3D_frame*))(0x1001B430))(this);
    }
    void AddToFFTree() {
        ((void(__thiscall*)(I3D_frame*))(0x1001B500))(this);
    }
    void UpdateHRBoundVolW() {
        ((void(__stdcall*)(I3D_frame*))(0x1001CF90))(this);
    }
    __declspec(dllexport) void UpdateWMatrixProc() {
        ((void(__thiscall*)(I3D_frame*))(0x1001C7D0))(this);
    }
    
    inline const S_matrix& I3D_frame::GetWorldMat() {
        if (!(m_uFlags & FRMFLAGS_VALID_WMATRIX)) {
            UpdateWMatrixProc();
        }

        return m_mWorldMat;
    }
	
	inline void SetPos(const S_vector& vPos) {
		 if (!(m_uFlags & 0x80u)) {
			m_mLocalMat.e[12] = vPos.x;
			m_mLocalMat.e[13] = vPos.y;
			m_mLocalMat.e[14] = vPos.z;
			m_uFlags = m_uFlags & 0xBFFFFEDF | 0x40000000;
		}
	}

	inline const S_vector& GetPos() {
		return *(S_vector*)&m_mLocalMat.e[12];
	}

	inline const S_vector& GetWorldPos() {
        return *(S_vector*)&m_mWorldMat.e[12];
	}

	inline void SetRot(const S_quat& aRot) {
		m_aRot = aRot;
		m_aRot.Normalize();
		m_uFlags = m_uFlags & 0xfffffecb | 0x40000008;
	}

	const S_quat& GetRot() const { return m_aRot;  }

    const S_vector& GetWorldDir() {
        return *(S_vector*)&m_mWorldMat.e[8];
    }

	inline void SetScale(const S_vector& vScale) {
		m_aScale = vScale;

		if (!(m_uFlags & 8)) {
			m_aRot.Make(m_mLocalMat);
		}

		m_uFlags = m_uFlags & 0xBFFFFEC3 | 0x40000008;
	}

	const S_vector& GetScale() const { return m_aScale; }


    inline uint32_t GetChildrenCount() const {
        return m_uChildrenCount;
    }
    // protected: // Note(Sev3n): WiP
    int32_t m_iRefCount;
    uint32_t _pad0;
    char* m_pSzProperties;
    S_matrix m_mWorldMat;
    S_matrix m_mLocalMat;
    S_vector m_aScale;
    S_quat m_aRot;
    uint32_t m_uFlags;
    I3D_bound_volume m_sLocalBVol;
    I3D_bound_volume m_sWorldBVol;
    char* m_pSzName;
    char* m_pSzModelName;
    I3D_CALLBACK* m_pCallback;
    I3D_frame* m_pOwner;
    I3D_FRAME_TYPE m_eFrameType;
    uint32_t _pad1;
    uint32_t _pad2;
    uint32_t m_uChildrenCount;
    I3D_frame* m_pPrev;
    I3D_frame* m_pNext;
    I3D_frame* m_pPrevPrev;
    I3D_frame* m_pNextNext;
    char _pad3[16];
    I3D_bbox m_uDummyBBOX;
    char _pad4[42];
};

static const char* g_pSzFrameNames[] = {
	"NULL",
    "VISUAL",
    "LIGHT",
    "CAMERA",
    "SOUND",
    "SECTOR",
    "DUMMY",
    "TARGET",
    "USER",
    "MODEL",
    "JOINT",
    "VOLUME",
    "OCCLUDER",
    "SCENE",
    "AREA", 
    "SHADOW",
    "LANDSACPE",
    "EMITOR",
    "LAST"
};
// constexpr int test = sizeof(I3D_frame);
// constexpr int offset = offsetof(I3D_frame, m_pNext);
// I3D_frame - 388b