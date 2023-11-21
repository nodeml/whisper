#include "utils.h"
#include <iostream>

namespace nodeml_portaudio
{
    namespace utils
    {
        void GetStreamParameters(PaStreamParameters &params, const Napi::Object object)
        {
            if (object.Has("channelCount"))
            {
                params.channelCount = object.Get("channelCount").ToNumber().Int32Value();
            }

            if (object.Has("device"))
            {
                std::cout << "GOT FROM JS " << object.Get("device").ToNumber().Int32Value() << std::endl;
                params.device = object.Get("device").ToNumber().Int32Value();
            }

            if (object.Has("sampleFormat"))
            {
                params.sampleFormat = object.Get("sampleFormat").ToNumber().Int64Value();
            }

            if (object.Has("suggestedLatency"))
            {
                params.suggestedLatency = object.Get("suggestedLatency").ToNumber().DoubleValue();
            }

            params.hostApiSpecificStreamInfo = NULL;
        }

        Napi::Value ToTypedArray(Napi::Env env, void *data, size_t dataLength, int format)
        {
            Napi::EscapableHandleScope scope(env);

            switch (format)
            {
            case paFloat32:
            {
                auto buffer = Napi::ArrayBuffer::New(env, data, dataLength * sizeof(float));
                return scope.Escape(Napi::TypedArrayOf<float>::New(env, dataLength, buffer, 0, napi_typedarray_type::napi_float32_array));
            }
            break;

            case paInt32:
            {
                auto buffer = Napi::ArrayBuffer::New(env, data, dataLength * sizeof(int32_t));
                return scope.Escape(Napi::TypedArrayOf<int32_t>::New(env, dataLength, buffer, 0, napi_typedarray_type::napi_int32_array));
            }
            break;

            case paInt24:
                return env.Undefined();
                break;

            case paInt16:
            {
                auto buffer = Napi::ArrayBuffer::New(env, data, dataLength * sizeof(int16_t));
                return scope.Escape(Napi::TypedArrayOf<int16_t>::New(env, dataLength, buffer, 0, napi_typedarray_type::napi_int16_array));
            }
            break;

            case paInt8:
            {
                auto buffer = Napi::ArrayBuffer::New(env, data, dataLength * sizeof(int8_t));
                return scope.Escape(Napi::TypedArrayOf<int32_t>::New(env, dataLength, buffer, 0, napi_typedarray_type::napi_int8_array));
            }
            break;

            case paUInt8:
            {
                auto buffer = Napi::ArrayBuffer::New(env, data, dataLength * sizeof(uint8_t));
                return scope.Escape(Napi::TypedArrayOf<uint8_t>::New(env, dataLength, buffer, 0, napi_typedarray_type::napi_uint8_array));
            }
            break;

            default:
                return env.Undefined();
                break;
            }
        }
    }
}
