#pragma once
#include <napi.h>
#include <whisper.h>

namespace nodeml_whisper
{

    // enum EStreamType {
    //     Input,
    //     Output,
    //     Duplex
    // };

    // struct StreamInfo
    // {
    //     Napi::ThreadSafeFunction tsfn;
    //     bool bHasBeenAquired = false;
    //     EStreamType type;
    //     int inputFormat;
    //     int outputFormat;
    // };

    // struct StreamCallbackInfo
    // {
    //     void *dataIn = NULL;
    //     void *dataOut = NULL;
    //     unsigned long frameCount;
    //     const PaStreamCallbackTimeInfo *timeInfo;

    //     StreamInfo * streamInfo = NULL;
    // };

    class Model : public Napi::ObjectWrap<Model>
    {

    public:
        static Napi::FunctionReference constructor;

        whisper_context * model;

        std::mutex mutex;

        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        Model(const Napi::CallbackInfo &info);

        ~Model();

        static Napi::Object FromNativeModel(Napi::Env env,whisper_context * nativeModel);

        static Model *FromObject(Napi::Value value);

        static Napi::Value Create(const Napi::CallbackInfo &info);

        Napi::Value Predict(const Napi::CallbackInfo &info);

        Napi::Value Free(const Napi::CallbackInfo &info);
    };
}