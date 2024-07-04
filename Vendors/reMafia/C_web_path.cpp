#include "C_web_path.h"

char* GetWebNodeTypeName(E_web_node_type eType) {
    switch (eType) {
    case WPT_Pedestrian: return "Pedestrian";
    case WPT_AI: return "AI";
    case WPT_Traffic: return "Traffic";
    case WPT_TramStation: return "TramStation";
    case WPT_Special: return "Special";
    case WPT_NadzemkaOnboard: return "Train Onboard";
    case WPT_NadzemkaWaypoint: return "Train Waypoint";
    case WPT_NadzemkaStop: return "Train Stop";
    case WPT_TramUnknown: return "Tram Unknown";
    case WPT_AIUnknown: return "AI Unknown";
    default: return "Unknown";
    }
}

S_vector GetWebNodeTypeColor(E_web_node_type eType) {
    switch (eType) {
    case WPT_Pedestrian: return S_vector(0, 0, 1);
    case WPT_AI: return S_vector(0, 1, 0);
    case WPT_Traffic: return S_vector(1, 1, 0);
    case WPT_TramStation: return S_vector(1, 0, 0);
    case WPT_Special: return S_vector(0.59f, 0, 1);
    case WPT_NadzemkaOnboard: return S_vector(1, 0.5f, 0.5f);
    case WPT_NadzemkaWaypoint: return S_vector(0.75f, 0.75f, 0);
    case WPT_NadzemkaStop: return S_vector(0.65f, 0, 0);
    case WPT_TramUnknown: return S_vector(0.85f, 0.25f, 0.25f);
    case WPT_AIUnknown: return S_vector(0.75f, 0, 0);
    default: return S_vector(0.65f, 0.65f, 0.65f);
    }
}

char* GetWebConnTypeName(E_web_connection_type eType) {
    switch (eType) {
    case WCT_Pedestrian: return "Pedestrian";
    case WCT_AI: return "AI";
    case WCT_TrafficForward: return "Traffic Forward";
    case WCT_TrafficBackward: return "Traffic Backward";
    case WCT_Nadzemka: return "Train";
    case WCT_Other: return "Other";
    default: return "Unknown";
    }
}

S_vector GetWebConnTypeColor(E_web_connection_type eType) {
    switch (eType) {
    case WCT_Pedestrian: return S_vector(0, 0, 1);
    case WCT_AI: return S_vector(0, 1, 0);
    case WCT_TrafficForward: return S_vector(1, 1, 0);
    case WCT_TrafficBackward: return S_vector(0, 1, 1);
    case WCT_Nadzemka: return S_vector(0.75f, 0.75f, 0);
    case WCT_Other: return S_vector(0.59f, 0, 1);
    default: return S_vector(0.45f, 0.45f, 0.45f);
    }
}
