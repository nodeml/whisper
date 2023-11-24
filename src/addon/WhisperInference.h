#pragma once

#include <napi.h>
#include <functional>
#include <future>
#include <whisper.h>
#include <iostream>

namespace nodeml_whisper
{

    struct WhisperInferenceConfig
    {
        float *data;
        int total;
        whisper_context *model;
        whisper_full_params params;
        std::mutex *mutex;
    };

    class WhisperInference : public Napi::AsyncWorker
    {
    public:
        WhisperInference(Napi::Env env, WhisperInferenceConfig config);
        ~WhisperInference();
        void Execute() override;
        void OnOK() override;
        void OnError(const Napi::Error &e) override;
        Napi::Promise GetPromise();

    private:
        Napi::Promise::Deferred promise;
        std::vector<std::string> result;
        WhisperInferenceConfig _config;
    };

    WhisperInference::WhisperInference(Napi::Env env, WhisperInferenceConfig config)
        : promise(Napi::Promise::Deferred::New(env)), _config(config), AsyncWorker(env)
    {
    }

    WhisperInference::~WhisperInference()
    {
        delete[] _config.data;
    }

    void WhisperInference::Execute()
    {
        _config.mutex->lock();

        if (whisper_full(_config.model, _config.params, _config.data, _config.total))
        {
            throw Napi::Error::New(Env(), "Failed To Process Audio");
        }

        auto n_segments = whisper_full_n_segments(_config.model);

        for (int i = 0; i < n_segments; ++i)
        {
            auto text = whisper_full_get_segment_text(_config.model, i);
            result.push_back(std::string(text));
        }
    }

    void WhisperInference::OnOK()
    {
        _config.mutex->unlock();
        
        auto arr = Napi::Array::New(Env(), result.size());

        for (auto i = 0; i < arr.Length(); i++)
        {
            arr[i] = Napi::String::New(Env(), result.at(i));
        }

        promise.Resolve(arr);
    }

    void WhisperInference::OnError(const Napi::Error &e)
    {
        _config.mutex->unlock();
        promise.Reject(e.Value());
    }

    Napi::Promise WhisperInference::GetPromise()
    {
        return promise.Promise();
    }
}