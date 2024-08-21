#pragma once

#include <Ini.hpp>
#include <fstream>
#include <string>
#include <vector>

struct ModConfig {
    ModConfig(const std::string& szFileName) {
        IniFile ini(szFileName);

        if (ini.IsValid()) {
            if (ini["#root"]["EnableMod"])
                bEnableCamOrbit = ini["#root"]["EnableMod"].Get<bool>();
            if (ini["#root"]["EnableDebugStats"])
                bEnableDebugStats = ini["#root"]["EnableDebugStats"].Get<bool>();

            if (ini["Input"]["InvertMouseX"])
                bInvertMouseX = ini["Input"]["InvertMouseX"].Get<bool>();
            if (ini["Input"]["InvertMouseY"])
                bInvertMouseY = ini["Input"]["InvertMouseY"].Get<bool>();
            if (ini["Input"]["MouseSensitivity"])
                fMouseSensitivity = ini["Input"]["MouseSensitivity"].Get<float>();
            if (ini["Input"]["SniperMouseSensitivity"])
                fSniperMouseSensitivity = ini["Input"]["SniperMouseSensitivity"].Get<float>();

            if (ini["Camera"]["Offset"])
                vCameraOffset = ini["Camera"]["Offset"].Get<S_vector>();


            if (ini["Vehicle"]["FrontFacingTime"])
                fCarFrontFacingTime = ini["Vehicle"]["FrontFacingTime"].Get<float>();
            if (ini["Vehicle"]["EnableRotationRoll"])
                bEnableCarRotRoll = ini["Vehicle"]["EnableRotationRoll"].Get<bool>();
            if (ini["Vehicle"]["RotationRollIntensity"])
                fCarRotRollIntensity = ini["Vehicle"]["RotationRollIntensity"].Get<float>();

            m_bIsLoaded = true;
        }
    }

    bool IsLoaded() const {
        return m_bIsLoaded;
    }

    void SaveConfig(const std::string& szFileName) {
        IniFile ini;

        ini["#root"]["EnableMod"].Set(bEnableCamOrbit);
        ini["#root"]["EnableDebugStats"].Set(bEnableDebugStats);

        ini["Input"]["InvertMouseX"].Set(bInvertMouseX);
        ini["Input"]["InvertMouseY"].Set(bInvertMouseY);
        ini["Input"]["MouseSensitivity"].Set(fMouseSensitivity);
        ini["Input"]["SniperMouseSensitivity"].Set(fSniperMouseSensitivity);

        ini["Camera"]["Offset"].Set(vCameraOffset);

        ini["Vehicle"]["FrontFacingTime"].Set(fCarFrontFacingTime);
        ini["Vehicle"]["EnableRotationRoll"].Set(bEnableCarRotRoll);
        ini["Vehicle"]["RotationRollIntensity"].Set(fCarRotRollIntensity);

        ini.Write(szFileName);
    }

    bool bEnableCamOrbit   = true;
    bool bEnableDebugStats = false;

    bool bInvertMouseX            = false;
    bool bInvertMouseY            = false;
    float fMouseSensitivity       = 0.1f;
    float fSniperMouseSensitivity = 0.025f;

    S_vector vCameraOffset = { 0, 0, 0 };

    float fCarFrontFacingTime  = 1.5f;
    bool bEnableCarRotRoll     = false;
    float fCarRotRollIntensity = 0.045f;

private:
    bool m_bIsLoaded = false;
};