#include "formats.h"

namespace nodeml_portaudio
{

    namespace formats
    {
        Napi::Object Init(Napi::Env env, Napi::Object exports)
        {

// #define paFloat32        ((PaSampleFormat) 0x00000001) /**< @see PaSampleFormat */
// #define paInt32          ((PaSampleFormat) 0x00000002) /**< @see PaSampleFormat */
// #define paInt24          ((PaSampleFormat) 0x00000004) /**< Packed 24 bit format. @see PaSampleFormat */
// #define paInt16          ((PaSampleFormat) 0x00000008) /**< @see PaSampleFormat */
// #define paInt8           ((PaSampleFormat) 0x00000010) /**< @see PaSampleFormat */
// #define paUInt8          ((PaSampleFormat) 0x00000020) /**< @see PaSampleFormat */
            auto myExports = Napi::Object::New(env);
            myExports.Set("float32",Napi::Number::New(env,paFloat32));
            myExports.Set("int32",Napi::Number::New(env,paInt32));
            myExports.Set("int24",Napi::Number::New(env,paInt24));
            myExports.Set("int16",Napi::Number::New(env,paInt16));
            myExports.Set("int8",Napi::Number::New(env,paInt8));
            myExports.Set("uint8",Napi::Number::New(env,paUInt8));

            exports.Set("formats",myExports);
            return exports;
        }

    }

}