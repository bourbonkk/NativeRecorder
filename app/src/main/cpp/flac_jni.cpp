#include <jni.h>
#include <string>

#include "FLAC/metadata.h"
#include "FLAC/stream_encoder.h"
#include <android/log.h>

jclass encoder_class;
jfieldID encoder_field;
jclass decoder_class;
jfieldID decoder_field;
jfieldID decoder_client_data_field;
jmethodID decoder_init_result_method;
jmethodID decoder_write_buffer_method;

#define TAG_NAME    "FlacJNI log"
#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)

FLAC__StreamEncoder *getEncoderFromInstance(JNIEnv *env, jobject instance) {
    return (FLAC__StreamEncoder *) env->GetLongField(instance, encoder_field);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bourbonkk_nativerecorder_flac_FlacEncoder_init(
        JNIEnv *env,
        jobject instance,
        jint sample_rate,
        jint channels,
        jint bps,
        jint compressionLevel,
        jstring outFile) {
	log_info("init() start .. ");

    FLAC__bool ok = true;
    FLAC__StreamEncoder *encoder = 0;
    FLAC__StreamEncoderInitStatus init_status;

    /* allocate the encoder */
    if ((encoder = FLAC__stream_encoder_new()) == NULL) {
        log_info("allocationg fail");
//        LOGE("ERROR: allocating encoder");
        return (jboolean) false;
    }

    ok &= FLAC__stream_encoder_set_verify(encoder, true);
    ok &= FLAC__stream_encoder_set_compression_level(encoder, (unsigned int) compressionLevel);
    ok &= FLAC__stream_encoder_set_channels(encoder, (unsigned int) channels);
    ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, (unsigned int) bps);
    ok &= FLAC__stream_encoder_set_sample_rate(encoder, (unsigned int) sample_rate);

    log_info("init() parameter setting success");

    const char *file = env->GetStringUTFChars(outFile, nullptr);
    if (ok) {
    	log_info("param status : %d", ok);
        init_status = FLAC__stream_encoder_init_file(encoder, file,
                                                     nullptr, /*client_data=*/NULL);
        if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
            log_info("ERROR: initializing encoder: %s",
                 FLAC__StreamEncoderInitStatusString[init_status]);
            ok = false;
        }
    }

    if (ok) {
        env->SetLongField(instance, encoder_field, (jlong) encoder);
        log_info("init() finished!");
    }
    return (jboolean) ok;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bourbonkk_nativerecorder_flac_FlacEncoder_process(
        JNIEnv *env,
        jobject instance,
        jbyteArray data,
        jint len) {
    FLAC__StreamEncoder *encoder = getEncoderFromInstance(env, instance);
	auto *bytes = new signed char[len];
    env->GetByteArrayRegion(data, 0, len, bytes);
    u_int bps = FLAC__stream_encoder_get_bits_per_sample(encoder);
    u_int samples = len * 8 / bps;
    u_int channels = FLAC__stream_encoder_get_channels(encoder);
	auto *buffer = new FLAC__int32[samples];

    for (int i = 0; i < samples; i++) {
        if (bps == 8) {
            buffer[i] = bytes[i];
          //  log_info("버퍼 읽는 중~~~~~!!");
        } else if (bps == 16) {
            buffer[i] = (bytes[2 * i] & 0xFF) | (bytes[2 * i + 1]) << 8;
        }
    }

	auto result = (jboolean) FLAC__stream_encoder_process_interleaved(encoder, buffer, samples / channels);

	delete bytes;
	delete buffer;

    return result;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bourbonkk_nativerecorder_flac_FlacEncoder_finish(JNIEnv *env, jobject instance) {
    FLAC__StreamEncoder *encoder = getEncoderFromInstance(env, instance);
    FLAC__bool ok = FLAC__stream_encoder_finish(encoder);
    FLAC__stream_encoder_delete(encoder);
    log_info("finish() start~~~~");
    return (jboolean) ok;
}


class DecoderClientData {
public:
    JNIEnv *env;
    jobject instance;
    jobject result;

    DecoderClientData(JNIEnv *env, jobject instance) {
        this->env = env;
        this->instance = instance;
    }

    void accept_stream_meta_data(const FLAC__StreamMetadata *metadata) {
        jlong total_samples = metadata->data.stream_info.total_samples;
        jint sample_rate = metadata->data.stream_info.sample_rate;
        jint channels = metadata->data.stream_info.channels;
        jint bps = metadata->data.stream_info.bits_per_sample;
        result = env->CallObjectMethod(instance, decoder_init_result_method,
                                       total_samples, sample_rate, channels, bps);
    }

    bool accept_buffer(const jbyte *buffer, int buffer_size) {
        jbyteArray a = env->NewByteArray(buffer_size);
        env->SetByteArrayRegion(a, 0, buffer_size, buffer);
        return env->CallBooleanMethod(instance, decoder_write_buffer_method, a);
    }
};

void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata,
                       void *client_data) {
    if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
        DecoderClientData *data = (DecoderClientData *) client_data;
        data->accept_stream_meta_data(metadata);
    }
}

FLAC__StreamDecoderWriteStatus
decoder_write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame,
                       const FLAC__int32 *const buffer[], void *client_data) {
    log_info("decoder_write_callback");
    int bits_per_sample = frame->header.bits_per_sample;
    int frame_count = frame->header.blocksize;
    int channels = frame->header.channels;
    int byte_count = frame_count * channels * bits_per_sample / 8;
    jbyte *result = new jbyte[byte_count];
    if (bits_per_sample == 8) {
        for (int i = 0; i < frame_count; ++i) {
            for (int j = 0; j < channels; ++j) {
                result[i * channels + j] = (jbyte) buffer[j][i];
            }
        }
    } else if (bits_per_sample == 16) {
        for (int i = 0; i < frame_count; ++i) {
            for (int j = 0; j < channels; ++j) {
                int index = i * channels + j;
                int buffer_value = buffer[j][i];
                result[2 * index] = (jbyte) buffer_value;
                result[2 * index + 1] = (jbyte) (buffer_value >> 8);
            }
        }
    }

    bool accepted = ((DecoderClientData *) client_data)->accept_buffer(result, byte_count);

	delete result;

    if (accepted) {
        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
    } else {
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
}

void
decoder_error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status,
                       void *client_data) {
    log_info("ERROR: running decoder: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_bourbonkk_nativerecorder_flac_FlacDecoder_init(JNIEnv *env, jobject instance, jstring input_file) {
    FLAC__bool ok = true;
    FLAC__StreamDecoder *decoder = 0;
    FLAC__StreamDecoderInitStatus init_status;

    if ((decoder = FLAC__stream_decoder_new()) == NULL) {
        log_info("ERROR: allocating decoder\n");
        return NULL;
    }

    (void) FLAC__stream_decoder_set_md5_checking(decoder, true);

    const char *input_file_chars = env->GetStringUTFChars(input_file, 0);
    DecoderClientData *client_data = new DecoderClientData(env, instance);
    init_status = FLAC__stream_decoder_init_file(decoder, input_file_chars, decoder_write_callback,
                                                 metadata_callback, decoder_error_callback,
                                                 client_data);
    if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
        log_info("ERROR: initializing decoder: %s\n",
             FLAC__StreamDecoderInitStatusString[init_status]);
        ok = false;
    }

    if (ok) {
        ok = FLAC__stream_decoder_process_until_end_of_metadata(decoder);
    }

    if (ok) {
        env->SetLongField(instance, decoder_field, (jlong) decoder);
        env->SetLongField(instance, decoder_client_data_field, (jlong) client_data);
        return client_data->result;
    } else {
        return NULL;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bourbonkk_nativerecorder_flac_FlacDecoder_nativeDecode(JNIEnv *env, jobject instance) {
    DecoderClientData *data = (DecoderClientData *) env->GetLongField(instance,
                                                                      decoder_client_data_field);
    data->env = env;    // Reset in new thread.
    data->instance = instance;

    FLAC__StreamDecoder *decoder = (FLAC__StreamDecoder *) env->GetLongField(instance,
                                                                             decoder_field);
    jboolean result = (jboolean) FLAC__stream_decoder_process_until_end_of_stream(decoder);
    FLAC__stream_decoder_delete(decoder);
    return result;
}

JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }

    encoder_class = env->FindClass("com/bourbonkk/nativerecorder/flac/FlacEncoder");
    encoder_field = env->GetFieldID(encoder_class, "encoderPointer", "J");

    decoder_class = env->FindClass("com/bourbonkk/nativerecorder/flac/FlacDecoder");
    decoder_field = env->GetFieldID(decoder_class, "decoderPointer", "J");
    decoder_client_data_field = env->GetFieldID(decoder_class, "clientDataPointer", "J");
    decoder_init_result_method = env->GetMethodID(decoder_class, "newStreamData",
                                                  "(JIII)Ljava/lang/Object;");
    decoder_write_buffer_method = env->GetMethodID(decoder_class, "writeData", "([B)Z");

    return JNI_VERSION_1_6;
}
