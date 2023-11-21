#include <nodeml_whisper/Model.h>
#include "Model.h"

namespace nodeml_whisper
{
    Napi::FunctionReference Model::constructor;

    Napi::Object Model::Init(Napi::Env env, Napi::Object exports)
    {
        auto func = DefineClass(env, "Model",
                                {Model::StaticMethod("create", &Model::Create)});

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        exports.Set("Model", func);
        return exports;
    }


    Model::Model(const Napi::CallbackInfo &info) : ObjectWrap(info) {
        model = nullptr;
    }

    Napi::Object Model::FromNativeModel(Napi::Env env,whisper_context * nativeModel){
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

    Model * Model::FromObject(Napi::Value value){
        return Napi::ObjectWrap<Model>::Unwrap(value.ToObject());
    }

    Napi::Value Model::Create(const Napi::CallbackInfo &info){
        auto env = info.Env();

        try
        {
            auto modelPath = info[0].ToString().Utf8Value();

            whisper_context_params cparams;

            whisper_context * ctx = whisper_init_from_file_with_params(modelPath.c_str(), cparams);

            if(ctx == nullptr){
                throw Napi::Error::New(env,"Failed to load model");
            }

            return FromNativeModel(env,ctx);
        }
        catch (const std::exception &e)
        {
            throw Napi::Error::New(env, e.what());
        }
    }
}
