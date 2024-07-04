#pragma once

#include <Utils.h>

enum E_item_flags {
    ITEM_IS_LOCKED    = 0x1,
    ITEM_IS_GUN       = 0x2,
    ITEM_IS_MELEE     = 0x4,
    ITEM_IS_BIG       = 0x20,
    ITEM_IS_SNIPER    = 0x40,
    ITEM_IS_EXPLOSIVE = 0xA0,
};

enum E_item_setting { IS_RejectCartridgeTime, IS_ReloadPause, IS_LoadCartridgeTime, IS_ByteSkip, IS_ShootDelay };

struct S_item_property {
    E_item_setting eType;
    int* iValue;
};

struct S_item {
    char szInternalName[32];
    E_item_flags eFlags;
    char szModelName[32];
    uint8_t pad0[8];
    int iReloadAnimID;
    uint8_t pad1[4];
    int iShootSoundID;
    int iReloadSoundID;
    int iItemType;
    int iMagCapacity;
    int iTotalAmmo;
    uint8_t pad2[12];
    float fRangeOfFire;
    float fDamage;
    uint8_t pad3[8];
    float fRecoil;
    float fAccuracy;
    uint8_t properties[48];
};
