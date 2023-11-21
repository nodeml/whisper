#pragma once

#include <napi.h>
#include <functional>
#include <portaudio.h>

namespace nodeml_portaudio
{

    enum EStreamType {
        Input,
        Output,
        Duplex
    };

    struct StreamInfo
    {
        Napi::ThreadSafeFunction tsfn;
        bool bHasBeenAquired = false;
        EStreamType type;
        int inputFormat;
        int outputFormat;
    };

    struct StreamCallbackInfo
    {
        void *dataIn = NULL;
        void *dataOut = NULL;
        unsigned long frameCount;
        const PaStreamCallbackTimeInfo *timeInfo;

        StreamInfo * streamInfo = NULL;
    };

    class Stream : public Napi::ObjectWrap<Stream>
    {

    public:
        static Napi::FunctionReference constructor;

        PaStream *stream = NULL;

        StreamInfo *streamInfo = NULL;

        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        Stream(const Napi::CallbackInfo &info);

        static Napi::Object FromPaStream(Napi::Env env, PaStream *paStream, StreamInfo * InStreamInfo);

        static Stream *FromObject(Napi::Value value);

        static Napi::Value Create(const Napi::CallbackInfo &info);

        Napi::Value IsActive(const Napi::CallbackInfo &info);

        Napi::Value Start(const Napi::CallbackInfo &info);

        Napi::Value Stop(const Napi::CallbackInfo &info);

        Napi::Value Close(const Napi::CallbackInfo &info);

        void prepare(const Napi::CallbackInfo &info);

        void cleanup();
    };
}