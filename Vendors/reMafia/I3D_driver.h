#pragma once
#include "LS3D.h"
#include "I3D_math.h"
#include "Memory.h"

enum I3DSTATSINDEX;

class I3D_scene;
class I3D_visual;
class I3D_frame;
class I3D_material;
class I3D_anim_material;
class ISND_sound;

struct I3D_triface;
struct I3D_text_coor;

class I3D_face_group;
class I3D_mesh_level;
class I3D_mesh_object;
class I3D_animation_set;
class I3D_pose;
class I3D_pose_set;

class I3D_driver {
public:
    /* public interface */
    I3DMETHOD(Init)(uint32_t iUnk) { }
    I3DMETHOD(Close)() { }
    I3DMETHOD_(void, Tick)(uint32_t uDeltaTime) { }
    I3DMETHOD_(uint32_t, GetTickTime)() const { }
    I3DMETHOD(GetStats)(I3DSTATSINDEX eStatIndex, void* pOutStats) { }
    I3DMETHOD(SetState)(I3D_RENDERSTATE eRenderState, int iRenderState) { }
    I3DMETHOD_(int, GetState)(I3D_RENDERSTATE eRenderState) const { }
    I3DMETHOD(Render)(I3D_scene* const pScene) { }
    I3DMETHOD_(void, DrawPoint)(const S_vector& vPos, const S_vector& vColor, uint32_t uAlpha) { }
    I3DMETHOD_(void, DrawPoint2D)(int iX, int iY, const S_vector& vColor, uint32_t uAlpha) { }
    I3DMETHOD_(void, DrawLine)(const S_vector& vP1, const S_vector& vP2, const S_vector& vColor, uint32_t uAlpha) { }
    I3DMETHOD_(void, DrawLines)(const S_vector* pPoints, uint32_t uPointsCnt, uint16_t const* pIndices, uint32_t uIndicesCnt, struct S_vector const& vColor, uint32_t uAlpha) { }
    I3DMETHOD(DrawTriangles)(void const* pVertices, uint32_t uVerticesCnt, uint16_t const* pIndices, uint32_t uIndicesCnt, S_vector const& vColor, uint32_t uAlpha, uint32_t uTextCoordCnt, I3D_text_coor* pTextCoords) { }
    I3DMETHOD_(int, sub_10017580)(const S_vector& vPos, float fUnk, const S_vector& vColor, int iUnk, uint8_t uAlpha) { }
    I3DMETHOD_(void, DrawTextA)(const S_vector& vPos, const char* pSzText, uint8_t uAlpha, float fTextSize) { }
    I3DMETHOD_(void, DrawText2D)(int iWidth, int iHeight, const char* pSzText, uint8_t uAlpha, float fTextSize) { }
    I3DMETHOD_(void, DrawSprite)(const S_vector& vPos, I3D_material* const pMaterial, uint8_t uAlpha, float fTextSize) { }
    I3DMETHOD_(void, DrawBox)(const I3D_bbox& bbox, const struct S_vector& vPos, const S_vector& vColor, uint8_t uAlpha) { }
    I3DMETHOD_(void, DrawSphere)(const S_matrix& matrix, const I3D_bsphere& sphere, const S_vector& vColor, uint8_t uAlpha) { }
    I3DMETHOD_(I3D_visual*, CreateVisual)(I3D_VISUAL_TYPE eVisualType) { }
    I3DMETHOD_(I3D_frame*, CreateFrame)(I3D_FRAME_TYPE eFrameType) { }
    I3DMETHOD_(I3D_material*, CreateMaterial)() { }
    I3DMETHOD_(I3D_anim_material*, CreateAnimMaterial)() { }
    I3DMETHOD_(I3D_triface*, CreateTrifaceArray)(int iCnt) { }
    I3DMETHOD_(I3D_face_group*, CreateFaceGroup)() { }
    I3DMETHOD_(I3D_mesh_level*, CreateMeshLevel)() { }
    I3DMETHOD_(I3D_mesh_object*, CreateMesh)() { }
    I3DMETHOD_(I3D_animation_set*, CreateAnimationSet)() { }
    I3DMETHOD_(I3D_pose*, CreatePose)() { }
    I3DMETHOD_(I3D_pose_set*, CreatePoseSet)() { }
    I3DMETHOD_(const char*, FrameType2String)(I3D_FRAME_TYPE eFrameType) { }
    I3DMETHOD_(const char*, VisualType2String)(I3D_VISUAL_TYPE eVisualType) { }
    I3DMETHOD(EnumVisualTypes)(I3D_ENUMVISUALPROC* pCallback, bool bUnk) const { }
    I3DMETHOD_(I3D_frame*, FindFrameFast)(I3D_scene* const pScene, const char* pSzFrameName) const { }
    I3DMETHOD_(void, OptimalizeFastFindData)() { }
    I3DMETHOD(EnumFFTree)(I3D_ENUMVISUALPROC* pCallback, uint32_t uUnk) const { }
    I3DMETHOD_(void, DeleteFromSceneTickList)(I3D_frame* pFrame) { }
    I3DMETHOD_(void, DeleteFromAllShadows)(I3D_frame* pFrame) { }
    I3DMETHOD_(void, DeleteFromAllTargets)(I3D_frame* pFrame) { }
    I3DMETHOD_(void, sub_100658F0)(void) { }
    I3DMETHOD_(void, nullsub_9)(void) { }
};

typedef I3D_driver*(__cdecl* I3DGetDriver_proc)();

I3D_driver* I3DGetDriver() {
    static HMODULE ls3df = GetModuleHandleA("LS3DF.dll");

    if (ls3df) {
        I3DGetDriver_proc I3DGetDriver_t = (I3DGetDriver_proc)GetProcAddress(ls3df, "I3DGetDriver");

        return I3DGetDriver_t();
    }

    return nullptr;
}
