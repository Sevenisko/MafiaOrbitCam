#pragma once

#include <LS3D.h>
#include "S_Collection.h"
#include <vector.h>

enum E_web_node_type : uint16_t {
    WPT_Pedestrian = 0x1,
    WPT_AI = 0x2,
    WPT_Traffic = 0x4,
    WPT_TramStation = 0x8,
    WPT_Special = 0x10,
    WPT_NadzemkaOnboard = 0x201,
    WPT_NadzemkaWaypoint = 0x1004,
    WPT_NadzemkaStop = 0x1008,
    WPT_TramUnknown = 0x1804,
    WPT_AIUnknown = 0x8002
};

extern char* GetWebNodeTypeName(E_web_node_type eType);
extern S_vector GetWebNodeTypeColor(E_web_node_type eType);

enum E_web_connection_type : uint16_t {
    WCT_Pedestrian = 0x1,
    WCT_AI = 0x2,
    WCT_TrafficForward = 0x4,
    WCT_Nadzemka = 0x84,
    WCT_TrafficBackward = 0x8400,
    WCT_Other = 0x1000
};

extern char* GetWebConnTypeName(E_web_connection_type eType);
extern S_vector GetWebConnTypeColor(E_web_connection_type eType);

#pragma pack(push, 1)
class C_web_connection {
public:
    uint16_t uEndPoint;
    E_web_connection_type eType;
    float fLength;
};
#pragma pack(pop)

#pragma pack(push, 1)
class C_web_node {
public:
    S_vector m_vPos;
    E_web_node_type m_eType;
    uint16_t m_uID;
    uint32_t m_uRadius;
    int m_iUnk;
    C_web_connection* m_pLinks;
    uint8_t m_uUnk;
    uint8_t m_uNumLinks;
};
#pragma pack(pop)

class C_web_path {
public:
    void* m_pVtbl;
    C_web_node* m_pNodes;
    int m_iUnk;
    int m_iNumPoints;
    uint8_t data[12];
};
