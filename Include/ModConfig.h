#pragma once

#include <Ini.hpp>
#include <fstream>
#include <string>
#include <vector>

struct ModConfig {
    ModConfig(const std::string& szFileName) {
        IniFile ini(szFileName);

        if (ini.IsValid()) {
            if (ini["#root"].HasValue("EnableMod"))
                bEnableCamOrbit = ini["#root"]["EnableMod"];
            if (ini["#root"].HasValue("EnableDebugStats"))
                bEnableDebugStats = ini["#root"]["EnableDebugStats"];

            if (ini["Input"].HasValue("InvertMouseX"))
                bInvertMouseX = ini["Input"]["InvertMouseX"];
            if (ini["Input"].HasValue("InvertMouseY"))
                bInvertMouseY = ini["Input"]["InvertMouseY"];
            if (ini["Input"].HasValue("MouseSensitivity"))
                fMouseSensitivity = ini["Input"]["MouseSensitivity"];
            if (ini["Input"].HasValue("SniperMouseSensitivity"))
                fSniperMouseSensitivity = ini["Input"]["SniperMouseSensitivity"];

            if (ini["Camera"].HasValue("EnableInterpolation"))
                bEnableCameraInterpolation = ini["Camera"]["EnableInterpolation"];
            if (ini["Camera"].HasValue("EnableOnFoot"))
                bEnableOnFootOrbitCam = ini["Camera"]["EnableOnFoot"];
            if (ini["Camera"].HasValue("EnableInCar"))
                bEnableInCarOrbitCam = ini["Camera"]["EnableInCar"];
            if (ini["Camera"].HasValue("Offset"))
                vCameraOffset = ini["Camera"]["Offset"];

            if (ini["Vehicle"].HasValue("FrontFacingTime"))
                fCarFrontFacingTime = ini["Vehicle"]["FrontFacingTime"];
            if (ini["Vehicle"].HasValue("EnableRotationRoll"))
                bEnableCarRotRoll = ini["Vehicle"]["EnableRotationRoll"];
            if (ini["Vehicle"].HasValue("RotationRollIntensity"))
                fCarRotRollIntensity = ini["Vehicle"]["RotationRollIntensity"];

            m_bIsLoaded = true;
        }
    }

    bool IsLoaded() const {
        return m_bIsLoaded;
    }

    void SaveConfig(const std::string& szFileName) {
        IniFile ini;

        ini["#root"]["EnableMod"]        = bEnableCamOrbit;
        ini["#root"]["EnableDebugStats"] = bEnableDebugStats;

        ini["Input"]["InvertMouseX"]           = bInvertMouseX;
        ini["Input"]["InvertMouseY"]           = bInvertMouseY;
        ini["Input"]["MouseSensitivity"]       = fMouseSensitivity;
        ini["Input"]["SniperMouseSensitivity"] = fSniperMouseSensitivity;

        ini["Camera"]["EnableInterpolation"] = bEnableCameraInterpolation;
        ini["Camera"]["EnableOnFoot"] = bEnableOnFootOrbitCam;
        ini["Camera"]["EnableInCar"] = bEnableInCarOrbitCam;
        ini["Camera"]["EnableInterpolation"] = bEnableCameraInterpolation;
        ini["Camera"]["Offset"]              = vCameraOffset;

        ini["Vehicle"]["FrontFacingTime"]       = fCarFrontFacingTime;
        ini["Vehicle"]["EnableRotationRoll"]    = bEnableCarRotRoll;
        ini["Vehicle"]["RotationRollIntensity"] = fCarRotRollIntensity;

        ini.Write(szFileName);
    }

    bool bEnableCamOrbit   = true;
    bool bEnableDebugStats = false;

    bool bInvertMouseX            = false;
    bool bInvertMouseY            = false;
    float fMouseSensitivity       = 0.1f;
    float fSniperMouseSensitivity = 0.025f;

    bool bEnableCameraInterpolation = true;
    bool bEnableOnFootOrbitCam = true;
    bool bEnableInCarOrbitCam = true;
    S_vector vCameraOffset          = { 0, 0, 0 };

    float fCarFrontFacingTime  = 1.5f;
    bool bEnableCarRotRoll     = false;
    float fCarRotRollIntensity = 0.045f;

private:
    bool m_bIsLoaded = false;
};