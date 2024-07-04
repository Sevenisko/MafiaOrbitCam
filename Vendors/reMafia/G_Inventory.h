#pragma once
struct S_GameItem
{
  int32_t iItemID;
  int32_t iAmmoLoaded;
  int32_t iAmmoHidden;
  int32_t iUnk;
};

class G_Inventory {
public:
  uint8_t pad0[8];
  int iNumItems;
  uint8_t pad1[20];
  S_GameItem sSelectedItem;
  S_GameItem sSmallItems[4];
  S_GameItem sUnkWeapon1;
  S_GameItem sCoatWeapon;
  S_GameItem sUnkItems[5];
  int m_iUnk;
};
