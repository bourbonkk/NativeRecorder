package com.bourbonkk.nativerecorder.flac

class FlacEncoder {
    private val encoderPointer: Long = 0

//    init {
//        System.loadLibrary("native-callrec")
//    }

    external fun init(
        sampleRate: Int,
        channels: Int,
        bps: Int,
        compressionLevel: Int,
        fileName: String
    ): Boolean

    external fun process(data: ByteArray, len: Int): Boolean
    //    public native boolean process(int len);

    external fun finish(): Boolean
}