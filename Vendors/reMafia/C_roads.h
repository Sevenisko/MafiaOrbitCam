#pragma once

#include <LS3D.h>
#include <vector.h>

class C_chunk;

#pragma pack(push, 1)
struct S_roads_lane {
    uint16_t uType;
    float fLineWidth;
    uint16_t pad1;
};
#pragma pack(pop)

struct S_roads_direction_link {
    int16_t uCrosspointLink;
    uint16_t pad0;
    float fDistance;
    float fAngle;
    uint16_t uPriority1;
    uint16_t uPriority2;
    uint16_t uInfo;
    S_roads_lane sLanes[4];
};

struct S_roads_crosspoint {
    S_vector vPos;
    bool bSemaphore;
    uint8_t pad0[3];
    float fSpeed;
    int16_t uWaypointLinks[4];
    S_roads_direction_link sDirectionLinks[4];
};

struct S_roads_waypoint {
    S_vector vPos;
    float fSpeed;
    int16_t uPrevWaypoint;
    int16_t uNextWaypoint;
    int16_t uPrevCrosspoint;
    int16_t uNextCrosspoint;
};

class C_roads {
public:
    virtual void SortNodesByX();
    virtual bool SaveData(C_chunk* pChunk);
    virtual bool LoadData(C_chunk* pChunk);
    virtual void RecompileDistances();

    int16_t GetNearNode(const S_vector& vPos);
    int16_t FindCrossing(int16_t firstId, int16_t secondId);

    std::vc6_vector<S_roads_crosspoint> m_vCrosspoints;
    std::vc6_vector<S_roads_waypoint> m_vWaypoints;
};