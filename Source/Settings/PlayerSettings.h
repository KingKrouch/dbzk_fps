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

#ifndef dbzk_fps_PLAYERSETTINGS_H
#define dbzk_fps_PLAYERSETTINGS_H

namespace dbzk_fps {
    class PlayerSettings {
    public:
        static PlayerSettings& Get() {
            return ps_Instance;
        }

        struct ResolutionSettings {

        };

        struct FOVSettings {

        };

        struct SyncSettings {
            int  MaxFPS       = 9999;
        };

        struct RenderingSettings {

        };

        struct InputSettings {

        };

        struct MiscSettings {

        };

        struct LauncherSettings {

        };

        struct InternalSettings {

        };

        struct ResolutionSettings RES {};
        struct FOVSettings FOV {};
        struct SyncSettings SYNC {};
        struct RenderingSettings RS {};
        struct InputSettings IS {};
        struct MiscSettings MS {};
        struct LauncherSettings LS {};
        struct InternalSettings INS {};

    private:
        PlayerSettings() {}
        static PlayerSettings ps_Instance;
    };
}

#endif //dbzk_fps_PLAYERSETTINGS_H
