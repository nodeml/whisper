
const whisper = require('./lib')
const path = require('node:path')
const audio = require('@nodeml/audio');
const Stream = require('stream');

/** @template T */
class AsyncQueue {
    /**
     * @private
     * @type {T[]}
     */
    pending = []

    /**
     * @private
     * @type {((data: T) => void) | undefined}
     */
    getCallback = undefined;

    /**
     * @param {T} data
     */
    push(data){
        if(this.getCallback !== undefined){
            this.getCallback(data);
            this.getCallback = undefined;
        }
        else
        {
            this.pending.push(data);
        }
    }

    /**
     * @return {Promise<T>} 
     */
    async get(){
        if(this.pending.length > 0){
            return this.pending.pop();
        }
        else
        {
            return await new Promise((res) => {
                this.getCallback = (d) =>  {
                    res(d);
                }
            })
        }
    }
}


/**
 * @param {Float32Array} arr1
 * @param {Float32Array} arr2
*/
function combineFloatArrays(arr1,arr2){
    const newArr = new Float32Array(arr1.length + arr2.length);
    newArr.set(arr1);
    newArr.set(arr2,arr1.length);
    return newArr;
}


/**
 * @type {AsyncQueue<Float32Array>}
*/
const pendingQueue = new AsyncQueue()

function createMicStream() {
    const defaultHost = audio.getHosts()[audio.getDefaultHostIndex()];

    const defaultInputDevice = audio.getHostDevices(defaultHost.id).find(c => c.id === defaultHost.defaultInputDevice);

    const sampleRate = 16000;

    const frameDuration = 30; // milliseconds

    const chunkSize = Math.floor(sampleRate * frameDuration / 1000);

    return audio.createStream({
        device: defaultInputDevice.id,
        sampleFormat: audio.formats.float32,
        suggestLatency: defaultInputDevice.defaultLowInputLatency,
        channelCount: 1,
        callback: (data) => {
            pendingQueue.push(data)
        }
    }, undefined, sampleRate, chunkSize)
}

async function main() {

    const modelPath = path.join(process.cwd(), 'ggml-base.en.bin')
    console.log("Starting predict")
    

    const whisperModel = whisper.createModel(modelPath,true);

    const stream = createMicStream()
    stream.start()

    const pending = Stream.PassThrough();

    let toTranscribe = new Float32Array();
    let isCollectingAudio = false;
    let windowSize = 3;
    const window = [];
    const threshOpen = 5;
    const threshClose = 0.1;
    while(true){
        
        const audioChunk = await pendingQueue.get();
        const maxDb = Math.max(...Array.from(audioChunk).map(c => Math.abs(c))) * 1000
        window.push(maxDb)
        if(window.length < windowSize){
            continue;
        }
        
        if(window.length > windowSize){
            window.shift()
        }

        const windowAvg = window.reduce((t,c) => t + c,0) / windowSize

        if(windowAvg > threshOpen && !isCollectingAudio){
            console.log("Starting Recording",maxDb,windowAvg,window)
            isCollectingAudio = true;
            toTranscribe = combineFloatArrays(toTranscribe,audioChunk);
            continue;
        }

        if(isCollectingAudio){
            toTranscribe = combineFloatArrays(toTranscribe,audioChunk);
        }

        if(windowAvg < threshClose && isCollectingAudio){
            console.log("Stopping Recoding",maxDb,windowAvg,window)
            isCollectingAudio = false;
            if(toTranscribe.length > 0){
                console.log("Ready to transcribe",toTranscribe.length,"samples",(toTranscribe.length / 16000),"Seconds")
                console.time("inference")
                await whisperModel.predict(toTranscribe,{
                            language: "en",
                            numThreads: 12,
                            speedUp: true,
                            tinydiarize: true
                        }).then(c => console.log("Predict Result",c))
                console.timeEnd("inference")
                toTranscribe = new Float32Array();
            }
        }

        // if(pending.length > 0){
        //     console.log("Predicting")
        //     console.time("predict")
        //     const data = pending.pop();
        //     const result = await whisperModel.predict(data,{
        //         language: "en",
        //         numThreads: 6,
        //         speedUp: true,
        //         tinydiarize: true
        //     });
        //     console.timeEnd("predict")

        //     console.log("Infered",result)
        // }
        // else
        // {
        //     await new Promise((r) => setTimeout(r,100))
        // }
    }
}

main()

