#include <napi.h>
#include <addon/Model.h>


Napi::Object InitModule(Napi::Env env, Napi::Object exports)
{
    nodeml_whisper::Model::Init(env,exports);
    
    return exports;
}

NODE_API_MODULE(nodeml_whisper, InitModule);