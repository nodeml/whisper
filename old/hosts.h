#pragma once

#include <napi.h>
#include <functional>

namespace nodeml_portaudio
{

    namespace hosts
    {
        Napi::Value GetHosts(const Napi::CallbackInfo &info);

        Napi::Value GetHostDevices(const Napi::CallbackInfo &info);

        Napi::Value GetDefaultHostIndex(const Napi::CallbackInfo &info);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
