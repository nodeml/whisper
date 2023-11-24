#pragma once

#include <napi.h>
#include <functional>
#include <whisper.h>

namespace nodeml_whisper
{

    namespace utils
    {
        void getWhisperInferenceParams(Napi::Object obj,whisper_full_params &result);

        Napi::Object Init(Napi::Env env, Napi::Object exports);
    }
}
