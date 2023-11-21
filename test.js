
const whisper = require('./lib')
const path = require('node:path')

async function main() {

    const modelPath = path.join(process.cwd(),'ggml-base.en.bin')
    console.log("Starting predict")
    console.time("predict")

    const whisperModel = whisper.Model.create(modelPath)

    console.timeEnd("predict")

    // while(true){
    //     await new Promise((r) => setTimeout(r,5000))
    // }
}

main()

