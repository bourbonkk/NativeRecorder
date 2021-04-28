package com.bourbonkk.nativerecorder

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.bourbonkk.nativerecorder.common.Dlog
import com.bourbonkk.nativerecorder.flac.FlacEncoder
import java.io.File
import java.text.SimpleDateFormat
import java.util.*


class MainActivity : AppCompatActivity(), View.OnClickListener {
    private var isRecording: Boolean = true
    private val frequency = 16000
    private val channelConfiguration = AudioFormatNative.AUDIO_CHANNEL_IN_MONO
    private val audioEncoding = AudioFormatNative.AUDIO_FORMAT_PCM_16_BIT
    private val compressLevel = 5
    protected var filePathNoFormat: String = ""

    private val bufferSize: Int = AudioRecord.getMinBufferSize(
        frequency,
        AudioFormat.CHANNEL_IN_MONO,
        AudioFormat.ENCODING_PCM_16BIT
    )

    private var mTimer: Timer? = null
    private var mTimerTask: TimerTask? = null

    private var count = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // jni test
        val soFileLoadResult = NativeSample().soFileLoad()
        Dlog.d(". file load result :$soFileLoadResult")

        val start = findViewById<Button>(R.id.start)
        start.setOnClickListener {
            start()
        }
        val stop = findViewById<Button>(R.id.stop)
        stop.setOnClickListener {
            stop()
        }


    }

    override fun onClick(view: View) {
        when (view.id) {
            R.id.start -> start()
            R.id.stop -> stop()
        }
    }

    private fun test() {
        Dlog.d("mTimer  = $mTimer")
        mTimer = Timer()
        mTimerTask = timerTaskMaker()
        Dlog.d("mTimer created = $mTimer")
        mTimer?.schedule(mTimerTask, 30, (1000 * 10).toLong())
    }

    private fun timerTaskMaker(): TimerTask {
        return object : TimerTask() {
            override fun run() {
                if (mTimer == null) {
                    cancel()
                } else {
                    Dlog.i("")
                    stop()
                    Thread.sleep(300)
                    start()
                }
            }
        }
    }

    private fun start() {
        Dlog.d("recording start ... ")
        Thread() {
            kotlin.run {
                try {
                    record()
                    Thread.currentThread().interrupt()
                    Thread.sleep(100)
                } catch (e: InterruptedException) {
                    e.printStackTrace()
                }
            }
        }.start()
    }

    private fun stop() {
        Dlog.d("recording stop ...")
        if (isRecording) {
            isRecording = !isRecording
        } else {
            Dlog.d("recording stop")
        }
    }

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

    private fun makeFileName(): String {
        val calendar = Calendar.getInstance()
        val df = SimpleDateFormat("yyyyMMddHHmmss", Locale.KOREA)
        val date = calendar.time
        return String.format("%s", df.format(date))
    }

    private fun makeOutputFile(): String {
        val dirStorage =
            Environment.getExternalStorageDirectory().getAbsolutePath() + "" + "/"
        File(dirStorage).mkdir()


        if (!File(dirStorage).exists()) {
            throw Exception()
        }

        filePathNoFormat = dirStorage + "nativerecord_" + makeFileName() + "_" + count
        Dlog.d("" + filePathNoFormat)

        count++
        return filePathNoFormat
    }
}
