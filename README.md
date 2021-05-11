# NativeRecorder


NativeRecorder is a native call recorder supported up to Android 8.1, supports flac.  
Tested on samsung phone.  

I made a fake_dlopen through hacking. However, it cannot be used from the Android 9 version.  

So.. I made it public.  

It's been a while since it's been made and there may be parts that need to be corrected,,,   
If it doesn't work, please fix it  


# How to use
```
private fun record() {
        val outputfile = makeOutputFile() + ".flac"

        File(outputfile)

        try {
            val audioRecord = NativeSample()
            val flacencoder = FlacEncoder()
            try {
                val audiorecordsetResult = audioRecord.audioRecordSet(
                    MediaRecorder.AudioSource.MIC,
                    frequency, audioEncoding,
                    channelConfiguration, bufferSize
                )
                Dlog.d("record set result: $audiorecordsetResult")

                flacencoder.init(
                    frequency, audioEncoding, channelConfiguration,
                    compressLevel, outputfile
                )
            } catch (e: Exception) {
                Dlog.e("error : $e")
            }
            var data: ByteArray
            audioRecord.start()
            isRecording = true
            while (isRecording) {
                try {
                    // read
                    data = audioRecord.read(bufferSize)
                    val processResult = flacencoder.process(data, bufferSize)
                    if (!processResult) {
                        Dlog.d("flac encoding error")
                        return
                    }
                } catch (e: Exception) {
                    Dlog.e("error : $e")
                }
                if (!isRecording) {
                    Dlog.d("!isRecording")
                }
            }

            audioRecord.stop()

            // audioRecord.stop() -> flacencoder.finish() -> audioRecord.release()
            val finishResult = flacencoder.finish()
            if (!finishResult) {
                Dlog.d("err")
            } else {
                Dlog.d("success stop")
            }

            audioRecord.release()

        } catch (t: Throwable) {
            Log.e("AudioRecord", "Recording Failed")
        }

    }
```
