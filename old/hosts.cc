#include "hosts.h"
#include <portaudio.h>

namespace nodeml_portaudio
{
    namespace hosts
    {
        Napi::Value GetHosts(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();
            try
            {
                auto availableApis = Pa_GetHostApiCount();

                auto apisArr = Napi::Array::New(env, availableApis);

                for (PaHostApiIndex i = 0; i < availableApis; i++)
                {
                    auto obj = Napi::Object::New(env);
                    auto apiInfo = Pa_GetHostApiInfo(i);

                    obj.Set("id", Napi::Number::New(env, apiInfo->type));
                    obj.Set("name", Napi::String::New(env, apiInfo->name));
                    obj.Set("deviceCount", Napi::Number::New(env, apiInfo->deviceCount));
                    obj.Set("defaultInputDevice", Napi::Number::New(env, apiInfo->defaultInputDevice));
                    obj.Set("defaultOutputDevice", Napi::Number::New(env, apiInfo->defaultOutputDevice));

                    apisArr[i] = obj;
                }

                return apisArr;
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }

        Napi::Value GetHostDevices(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();
            try
            {
                auto hostId = info[0].ToNumber().Int32Value();

                auto hostIndex = Pa_HostApiTypeIdToHostApiIndex(static_cast<PaHostApiTypeId>(hostId));

                auto hostInfo = Pa_GetHostApiInfo(hostIndex);

                auto apisArr = Napi::Array::New(env, hostInfo->deviceCount);

                for (int deviceIndexHost = 0; deviceIndexHost < hostInfo->deviceCount; deviceIndexHost++)
                {
                    auto obj = Napi::Object::New(env);

                    auto deviceId = Pa_HostApiDeviceIndexToDeviceIndex(hostIndex,deviceIndexHost);

                    auto deviceInfo = Pa_GetDeviceInfo(deviceId);

                    

                    obj.Set("id", Napi::Number::New(env, deviceId));
                    obj.Set("name", Napi::String::New(env, deviceInfo->name));
                    obj.Set("hostId", Napi::Number::New(env, hostId));
                    obj.Set("maxInputChannels", Napi::Number::New(env, deviceInfo->maxInputChannels));
                    obj.Set("maxOutputChannels", Napi::Number::New(env, deviceInfo->maxOutputChannels));
                    obj.Set("defaultSampleRate", Napi::Number::New(env, deviceInfo->defaultSampleRate));
                    obj.Set("defaultLowInputLatency", Napi::Number::New(env, deviceInfo->defaultLowInputLatency));
                    obj.Set("defaultLowOutputLatency", Napi::Number::New(env, deviceInfo->defaultLowOutputLatency));
                    obj.Set("defaultHighInputLatency", Napi::Number::New(env, deviceInfo->defaultHighInputLatency));
                    obj.Set("defaultHighOutputLatency", Napi::Number::New(env, deviceInfo->defaultHighOutputLatency));

                    apisArr[deviceIndexHost] = obj;
                }

                return apisArr;
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }

        Napi::Value GetDefaultHostIndex(const Napi::CallbackInfo &info)
        {
            auto env = info.Env();
            try
            {
                return Napi::Number::New(env,Pa_GetDefaultHostApi());
            }
            catch (const std::exception &e)
            {
                throw Napi::Error::New(env, e.what());
            }
        }

        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {
            exports.Set("getHosts", Napi::Function::New(env, GetHosts));
            exports.Set("getHostDevices",Napi::Function::New(env,GetHostDevices));
            exports.Set("getDefaultHostIndex",Napi::Function::New(env,GetDefaultHostIndex));
            return exports;
        }

    }
}