#include <addon/Model.h>
#include <addon/utils.h>
#include <addon/WhisperInference.h>
#include "Model.h"

namespace nodeml_whisper
{
    Napi::FunctionReference Model::constructor;

    Napi::Object Model::Init(Napi::Env env, Napi::Object exports)
    {
        auto func = DefineClass(env, "Model",
                                {Model::InstanceMethod("predict", &Model::Predict),Model::InstanceMethod("free", &Model::Free)
                                });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("Model", func);
        exports.Set("createModel", Napi::Function::New(env, &Model::Create));
        return exports;
    }

    Model::Model(const Napi::CallbackInfo &info) : ObjectWrap(info)
    {
        model = nullptr;
    }

    Model::~Model()
    {
        if (model != nullptr)
        {
            whisper_free(model);
            model = nullptr;
        }
    }

    Napi::Object Model::FromNativeModel(Napi::Env env, whisper_context *nativeModel)
    {
        try
        {
            Napi::EscapableHandleScope scope(env);
            auto newModel = Model::constructor.New({});
            Napi::ObjectWrap<Model>::Unwrap(newModel)->model = nativeModel;
            return scope.Escape(newModel).ToObject();
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }
    }

    Model *Model::FromObject(Napi::Value value)
    {
        return Napi::ObjectWrap<Model>::Unwrap(value.ToObject());
    }

    Napi::Value Model::Create(const Napi::CallbackInfo &info)
    {
        auto env = info.Env();

        try
        {
            auto modelPath = info[0].ToString().Utf8Value();

            whisper_context_params cparams;

            cparams.use_gpu = info.Length() >= 2 && info[1].IsBoolean() ? info[1].ToBoolean().Value() : false;

            whisper_context *ctx = whisper_init_from_file_with_params(modelPath.c_str(), cparams);

            if (ctx == nullptr)
            {
                throw Napi::Error::New(env, "Failed to load model");
            }

            return FromNativeModel(env, ctx);
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }
    }

    Napi::Value Model::Predict(const Napi::CallbackInfo &info)
    {
        auto env = info.Env();

        if (model == nullptr)
        {
            throw Napi::Error::New(env, "Model is not valid");
        }

        try
        {
            auto data = info[0].As<Napi::TypedArrayOf<float>>();
            std::vector<float> dataVec;
            dataVec.resize(data.ElementLength());

            whisper_full_params params;

            utils::getWhisperInferenceParams(info.Length() >= 1 && info[0].IsObject() ? info[0].ToObject() : Napi::Object::New(env), params);

            WhisperInferenceConfig config;

            config.data = new float[data.ElementLength()];

            memcpy(config.data, data.Data(), data.ElementLength() * sizeof(float));
            config.total = data.ElementLength();
            config.model = model;
            config.params = params;
            config.mutex = &mutex;

            auto worker = new WhisperInference(env, config);

            worker->Queue();

            return worker->GetPromise();
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }
    }
    Napi::Value Model::Free(const Napi::CallbackInfo &info)
    {
        auto env = info.Env();

        try
        {
            if (model != nullptr)
            {
                whisper_free(model);
                model = nullptr;
            }
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }

        return env.Undefined();
    }
}
