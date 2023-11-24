
// if(obj.Has("translate")){
//     result.translate = obj.Get("translate").ToBoolean().Value();
// }

// if(obj.Has("singleSegment")){
//     result.single_segment = obj.Get("singleSegment").ToBoolean().Value();
// }

// if(obj.Has("maxTokens")){
//     result.max_tokens = obj.Get("maxTokens").ToNumber().Int32Value();
// }

// if(obj.Has("language")){
//     result.language = obj.Get("language").ToString().Utf8Value().c_str();
// }

// if(obj.Has("numThreads")){
//     result.n_threads = obj.Get("numThreads").ToNumber().Int32Value();
// }

// if(obj.Has("audioContext")){
//     result.audio_ctx = obj.Get("audioContext").ToNumber().Int32Value();
// }

// if(obj.Has("speedUp")){
//     result.speed_up = obj.Get("speedUp").ToBoolean().Value();
// }

// if(obj.Has("tinydiarize")){
//     result.tdrz_enable = obj.Get("tinydiarize").ToBoolean().Value();
// }

export interface IModelInferenceParams {
    translate: boolean;
    singleSegment: boolean;
    maxTokens: string;
    language: string;
    numThreads: number;
    audioContext: number;
    speedUp: boolean;
    tinydiarize: boolean;
}

export declare function createModel(modelPath: string,useGpu: boolean = false): Model

export declare class Model {
    predict(data: Float32Array, params?: Partial<IModelInferenceParams>): Promise<string[]>;
    free: () => void;
}