#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <Ini.hpp>

struct ModConfig {
    ModConfig(const std::string& szFileName) {
        IniFile ini(szFileName);

        if (ini.IsValid()) {
            bEnableCamOrbit = ini["#root"]["EnableMod"].Get<bool>();
            bEnableDebugStats = ini["#root"]["EnableDebugStats"].Get<bool>();

            bInvertMouseX   = ini["Input"]["InvertMouseX"].Get<bool>();
            bInvertMouseY   = ini["Input"]["InvertMouseY"].Get<bool>();
            fMouseSensitivity = ini["Input"]["MouseSensitivity"].Get<float>();

            fCarFrontFacingTime  = ini["Vehicle"]["FrontFacingTime"].Get<float>();
            bEnableCarRotRoll = ini["Vehicle"]["EnableRotationRoll"].Get<bool>();
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

        ini["Vehicle"]["FrontFacingTime"].Set(fCarFrontFacingTime);
        ini["Vehicle"]["EnableRotationRoll"].Set(bEnableCarRotRoll);
        ini["Vehicle"]["RotationRollIntensity"].Set(fCarRotRollIntensity);

        ini.Write(szFileName);
    }

    bool bEnableCamOrbit = true;
    bool bEnableDebugStats = false;

    bool bInvertMouseX = false;
    bool bInvertMouseY = false;
    float fMouseSensitivity = 0.1f;

    float fCarFrontFacingTime  = 1.5f;
    bool bEnableCarRotRoll = true;
    float fCarRotRollIntensity = 0.045f;

private:
    bool m_bIsLoaded = false;
};