/**
dbzk_fps Copyright (c) 2023 Bryce Q.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

// Internal Functionality
#include "ConfigManager.h"
#include "../Settings/PlayerSettings.h"
// System Libraries
#include <iostream>
#include <fstream>
// Third Party Libraries
#include <inipp.h>
// Variables

auto& PlayerSettingsConf = dbzk_fps::PlayerSettings::Get();
// Namespaces
using namespace std;
// Singleton Instance
dbzk_fps::ConfigManager dbzk_fps::ConfigManager::cm_Instance; // Seemingly need this declared in PlayerSettings.cpp so a bunch of linker errors don't happen.

namespace dbzk_fps {
    inipp::Ini<char> config;
    std::ifstream is("Config.ini");

    void ConfigManager::Init() {
        ifstream configName("Config.ini");
        config.parse(configName);
        config.generate(cout);
        config.default_section(config.sections["Settings"]);
        config.interpolate();
    }

    void ConfigManager::SaveConfig() { // TODO: Find a way of writing configs with IniPP.
        if (AlreadyReadConfig) {
            // FOV Settings
            //ini.WriteBoolean("FieldOfView", "UseCustomFOV",              PlayerSettingsConf.FOV.UseCustomFOV,      0);
            //ini.WriteBoolean("FieldOfView", "UseAdaptiveFOVScaling",        PlayerSettingsConf.FOV.VertPlusScaling,   0);
            //ini.WriteInteger("FieldOfView", "FieldOfView",               PlayerSettingsConf.FOV.FieldOfView,       0);
            // Sync and Framerate Settings
            //ini.WriteInteger("Framerate",   "MaxFPS",                    PlayerSettingsConf.SYNC.MaxFPS,           0);
            // Input Settings
            //ini.WriteBoolean("Input",       "KBMPrompts",                PlayerSettingsConf.IS.KBMPrompts,         0);
            //ini.WriteBoolean("Input",       "DisableSteamInput",         PlayerSettingsConf.IS.DisableSteamInput,  0);
            //ini.WriteInteger("Input",       "InputType",                   PlayerSettingsConf.IS.InputDeviceType,    0);
            // Misc Settings
            //ini.WriteBoolean("Misc",        "SkipOpeningVideos",         PlayerSettingsConf.MS.SkipOpeningVideos,  0);
            //ini.WriteBoolean("Misc",        "CameraTweaks",              PlayerSettingsConf.MS.CameraTweaks,       0);
            // Launcher Settings
            //ini.WriteBoolean("Launcher",    "IgnoreUpdates",             PlayerSettingsConf.LS.IgnoreUpdates,      0);
        }
    }

    void ConfigManager::ReadConfig() {
        Init();
        // Sync and Framerate Settings
        inipp::extract(config.sections["Framerate"]["MaxFPS"], PlayerSettingsConf.SYNC.MaxFPS);
        AlreadyReadConfig = true; // After the INI file has successfully been read for the first time, allow writing.
    }
}