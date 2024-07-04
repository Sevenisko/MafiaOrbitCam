#pragma once
#include "LS3D.h"

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <d3d8.h>

#define MAX_DEFINED_KEYS    64
#define KEYBOARD_BUFFER_LEN 256
#define FVF_2D_PRIMITIVE    (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define CLEAR_TARGET  0x00000001l /* Clear target surface */
#define CLEAR_ZBUFFER 0x00000002l

enum MOUSE_BUTTONS_FLAGS { MOUSE_BUTTON_LEFT = (1 << 0), MOUSE_BUTTON_RIGHT = (1 << 1), MOUSE_BUTTON_MIDDLE = (1 << 2) };

// NOTE (Sev3n): Temporarily put here, just to reverse IGraph::Init method
struct I3D_stats_volume {
    uint8_t buf[32];
};

struct LS3D_VIEWPORT {
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    float MinZ;
    float MaxZ;
};

struct LS3DGAMMARAMP {
    WORD red[256];
    WORD green[256];
    WORD blue[256];
};

struct IGRAPH_INIT_DESC {
    HINSTANCE Instance;
    uint32_t CurrentAdapter;
    uint32_t X;
    uint32_t Y;
    uint32_t Width;
    uint32_t Height;
    RECT Rc;
    uint8_t BPP;
    uint8_t RefreshRate;
    uint8_t Unk2;
    uint8_t Unk3;
    uint32_t Flags;
    HMENU Menu;
};

#pragma pack(push, 1)
typedef struct _tgaheader {
    uint8_t IDLength;        /* 00h  Size of Image ID field */
    uint8_t ColorMapType;    /* 01h  Color map type */
    uint8_t ImageType;       /* 02h  Image type code */
    uint8_t CMapStart[2];    /* 03h  Color map origin */
    uint8_t CMapLength[2];   /* 05h  Color map length */
    uint8_t CMapDepth;       /* 07h  Depth of color map entries */
    uint16_t XOffset;        /* 08h  X origin of image */
    uint16_t YOffset;        /* 0Ah  Y origin of image */
    uint16_t Width;          /* 0Ch  Width of image */
    uint16_t Height;         /* 0Eh  Height of image */
    uint8_t PixelDepth;      /* 10h  Image pixel size */
    uint8_t ImageDescriptor; /* 11h  Image descriptor byte */
} TGAHEADER;
#pragma pack(pop)

// TODO(DavoSK): meybe later on unix / linux ?
//
// typedef struct _tagBITMAPFILEHEADER {
//     uint16_t bfType;
//     uint32_t bfSize;
//     uint16_t bfReserved1;
//     uint16_t bfReserved2;
//     uint32_t bfOffBits;
// } BITMAPFILEHEADER;
//
// typedef struct _tagBITMAPINFOHEADER {
//     uint32_t biSize;
//     uint32_t biWidth;
//     uint32_t biHeight;
//     uint16_t biPlanes;
//     uint16_t biBitCount;
//     uint32_t biCompression;
//     uint32_t biSizeImage;
//     uint32_t biXPelsPerMeter;
//     uint32_t biYPelsPerMeter;
//     uint32_t biClrUsed;
//     uint32_t biClrImportant;
// } BITMAPINFOHEADER;

enum DX_STATE { ALPHA = 0, BLEND, CLIP, CULL, COLOR_OP, COLOR_OP1, COLOR_OP2, ALPHA_OP, ZBIAS, ADDRESS_U, ADDRESS_V, ALPHA_REF, ZCOMPARE, ZWRITABLE, ZENABLE, CLAMP_TEXTURE, FOG = 16 };

enum PRIMITIVE_TYPE { PT_POINTLIST = 0, PT_LINELIST = 1, PT_LINESTRIP = 2, PT_TRIANGLELIST = 3, PT_TRIANGLEFAN = 4, PT_TRIANGLESTRIP = 5, PT_UNKNOWN = 6 };

enum IGRAPH_INIT_FLAGS {
    INITFLAG_FULLSCREEN            = 0x2,
    INITFLAG_TRIPLEBUFFERING       = 0x4,
    INITFLAG_WINDOWED              = 0x8,
    INITFLAG_HWTRANSANDLIGHT       = 0x10,
    INITFLAG_DEBUGLOG              = 0x40,
    INITFLAG_DONTCREATECHILDWINDOW = 0x80,
    INITFLAG_VSYNC                 = 0x100,
    INITFLAG_LOCKABLESTENCILFORMAT = 0x400,
    INITFLAG_SLOWREFRASTERIZER     = 0x800,
    INITFLAG_2XMSAA                = 0x1000,
    INITFLAG_ENABLEMSAA            = 0x3000,
    INITFLAG_MULTIPASSRENDERING    = 0x4000,
    INITFLAG_DEBUGMODE             = 0x10000,
    INITFLAG_USETEXMANAGER         = 0x100000,
};

enum IGRAPH_HWCAPS {
    HWCAPS_HARDWARE_TRANSFORM   = 0x1,
    HWCAPS_SINGLEPASS_RENDERING = 0x80,
};

enum LS3D_STREAM_TYPE { ST_BORDER = 0, ST_FILLED = 1 };

class IShow;
class ITexture;

class IGraph {
public:
    I3DMETHOD_(IShow*, CreateIShow)() { }
    I3DMETHOD(CreateITexture)(const char* pSzDiffuse, const char* pSzAlpha, uint32_t uFlags, ITexture** pTextureOut) { }
    I3DMETHOD_(ITexture*, CreateITexture2)() { }
    I3DMETHOD_(bool, SetTexture)(ITexture* pITexture) { }
    I3DMETHOD_(HWND, GetMainHWND)() {}
    I3DMETHOD_(HWND, GetChildHWND)() { }
    I3DMETHOD_(void, SetAppName)(const char* pSzName) { }
    I3DMETHOD_(void, SetAppNameDebug)(const char* pSzFormat, ...) { }
    I3DMETHOD_(const char*, GetAppName)() { }
    I3DMETHOD_(bool, AddDlgHWND)(HWND) { }
    I3DMETHOD_(bool, DeleteDlgHWND)(HWND) { }
    I3DMETHOD_(void, ProcessWinMessages)() { }
    I3DMETHOD_(void, Clear)(uint32_t color, float depth, uint32_t flags) { }
    I3DMETHOD_(void, BeginScene)() { }
    I3DMETHOD_(void, EndScene)() { }
    I3DMETHOD_(void, Present)() { }
    I3DMETHOD_(void, ResetRenderProps)() { }
    I3DMETHOD_(int32_t, SetState)(DX_STATE, uint32_t) { }
    I3DMETHOD_(int32_t, GetState)(DX_STATE) { }
    I3DMETHOD(SetViewPort)(LS3D_VIEWPORT*) { }
    I3DMETHOD_(LS3D_VIEWPORT*, GetViewPort)() { }
    I3DMETHOD_(void, SetWorldMatrix)(const S_matrix& matrix) { }
    I3DMETHOD(DrawPrimitiveList)(PRIMITIVE_TYPE primitiveType, uint32_t primitiveCount, void* buffer, LS3D_STREAM_TYPE streamType) { }
    I3DMETHOD(DrawIndexedPrimitiveList)(PRIMITIVE_TYPE primitiveType, uint32_t primitiveCount, void* buffer, uint32_t indicesCount, void* indicesBuffer, LS3D_STREAM_TYPE streamType) { }
    I3DMETHOD(Init)(IGRAPH_INIT_DESC*) { }
    I3DMETHOD_(IGRAPH_INIT_DESC*, GetResInitDesc)() { }
    I3DMETHOD_(bool, IsInit)() { }
    I3DMETHOD_(void, Close)() { }
    I3DMETHOD_(int32_t, Scrn_sx)() { }
    I3DMETHOD_(int32_t, Scrn_sy)() { }
    I3DMETHOD_(int32_t, BPP)() { }
    I3DMETHOD_(_D3DFORMAT, GetD3DFormat)() { }
    I3DMETHOD_(const char*, GetFormatName)(_D3DFORMAT) { }
    I3DMETHOD(GetImageInfo)(const char*, uint16_t*, uint16_t*, uint8_t*, uint32_t, int32_t*) { }
    I3DMETHOD_(bool, SaveShot)(const char* pSzFileName, RECT* pRect, uint32_t uFlags, void* pUnk) { }
    I3DMETHOD_(LS3D_RESULT, KeyboardInit)(uint32_t) { }
    I3DMETHOD_(bool, IsKeyboardInit)() { }
    I3DMETHOD_(void, KeyboardClose)() { }
    I3DMETHOD(UpdateKeyboardData)() { }
    I3DMETHOD_(uint32_t, ReadKey)() { }
    I3DMETHOD_(uint32_t, ReadChar)() { }
    I3DMETHOD_(char*, ReadCharBuffer)() { }
    I3DMETHOD_(void, ClearCharBuffer)() { }
    I3DMETHOD_(bool, TestKey)(uint8_t) { }
    I3DMETHOD(DefineKeys)(const char*, uint32_t) { }
    I3DMETHOD_(int64_t, ReadKeys)() { }
    I3DMETHOD_(bool, KeyShift)() { }
    I3DMETHOD_(bool, KeyCtrl)() { }
    I3DMETHOD_(bool, KeyAlt)() { }
    I3DMETHOD_(uint32_t, GetShiftKeys)(bool) { }
    I3DMETHOD_(char, TranslateKey)(uint8_t, uint32_t) { }
    I3DMETHOD_(const wchar_t*, GetKeyName)(int32_t) { }
    I3DMETHOD_(LS3D_RESULT, MouseInit)(uint32_t) { }
    I3DMETHOD_(bool, IsMouseInit)() { }
    I3DMETHOD_(void, MouseClose)() { }
    I3DMETHOD(UpdateMouseData)() { }
    I3DMETHOD_(int32_t, Mouse_rx)() { }
    I3DMETHOD_(int32_t, Mouse_ry)() { }
    I3DMETHOD_(int32_t, Mouse_rz)() { }
    I3DMETHOD_(void, SetMousePos)(int32_t, int32_t) { }
    I3DMETHOD_(int32_t, Mouse_x)() { }
    I3DMETHOD_(int32_t, Mouse_y)() { }
    I3DMETHOD_(uint32_t, GetMouseButtons)() { }
    I3DMETHOD_(void, NullTimer)() { }
    I3DMETHOD_(uint32_t, GetTimer)(uint32_t, uint32_t) { }
    I3DMETHOD(AddMapsDir)(const char*, bool) { }
    I3DMETHOD(DeleteMapsDir)(const char*) { }
    I3DMETHOD_(void, ClearMapDirs)() { }
    I3DMETHOD_(ITexture**, GetCachedTextures)() { }
    I3DMETHOD_(uint32_t, GetCachedTextureCnt)() { }
    I3DMETHOD_(ITexture**, GetUncachedTextures)() { }
    I3DMETHOD_(uint32_t, GetUncachedTextureCnt)() { }
    I3DMETHOD_(bool, SetGammaRamp)(LS3DGAMMARAMP*, uint32_t) { }
    I3DMETHOD_(bool, GetGammaRamp)(LS3DGAMMARAMP*) { }
    I3DMETHOD_(bool, ResizeImage)(int32_t, int32_t, uint32_t, uint8_t*, int32_t, int32_t, uint32_t, uint8_t**, uint32_t, int32_t) { }
};

typedef IGraph*(__cdecl* GetIGraph_proc)();

IGraph* GetIGraph() {
    static HMODULE ls3df = GetModuleHandleA("LS3DF.dll");

    if (ls3df) {
        GetIGraph_proc GetIGraph_t = (GetIGraph_proc)GetProcAddress(ls3df, "GetIGraph");

        return GetIGraph_t();
    }

    return nullptr;
}