#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_portaudio
{

    namespace utils
    {
        void GetStreamParameters(PaStreamParameters &params,const Napi::Object object);

        
        Napi::Value ToTypedArray(Napi::Env env, void * data,size_t dataLength,int format);
        //Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
