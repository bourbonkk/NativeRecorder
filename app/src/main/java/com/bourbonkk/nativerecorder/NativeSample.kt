package com.bourbonkk.nativerecorder

class NativeSample {
    companion object {
        init {
            System.loadLibrary("native-callrec")
        }
    }
    // 1. so 파일 로드 및 사용하는 파라미터 static 변수로 로드
    external fun soFileLoad(): Int

    // 2. AudioRecord 생성자, set() , inputPrivate(), callRecLoad()-쓰레드생성 실행
    external fun audioRecordSet(inputsource: Int, frequency: Int, channel: Int, audioEncoding:Int,bufferSize:Int): Int

    // 3. start() 하고 startParam() 실행
    external fun start()

    // 4. 버퍼에 있는 데이터 리드 (메모리 누수 잡음)
    external fun read(bufferSize: Int):ByteArray

    // 5. stopParam() 후 stop() 실행됨
    external fun stop()

    // 6. stop(), close(), delete audiorecord; 실행
    external fun release()

}


