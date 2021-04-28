package com.bourbonkk.nativerecorder.flac

import androidx.annotation.Keep
import androidx.annotation.NonNull

@Keep
class FlacDecoder {
    private val decoderPointer: Long = 0
    private val clientDataPointer: Long = 0
    private var writeDataCallback: WriteDataCallback? = null


    init {
        System.loadLibrary("callrecfix")
    }


    external fun init(inputFile: String): StreamData

    private external fun nativeDecode(): Boolean

    fun decode(@NonNull callback: WriteDataCallback): Boolean {
        writeDataCallback = callback
        val r: Boolean
        try {
            r = nativeDecode()
        } finally {
            writeDataCallback = null
        }
        return r
    }

    private fun newStreamData(totalSamples: Long, sampleRate: Int, channels: Int, bps: Int): Any {
        return StreamData(totalSamples, sampleRate, channels, bps)
    }

    private fun writeData(buffer: ByteArray): Boolean {
        return writeDataCallback!!.writeData(buffer)
    }

    class StreamData internal constructor(
        internal var totalSamples: Long,
        internal var sampleRate: Int,
        internal var channels: Int,
        internal var bps: Int
    )

    interface WriteDataCallback {
        fun writeData(buffer: ByteArray): Boolean
    }


}