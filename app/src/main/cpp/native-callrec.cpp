#include <jni.h>
#include <string>
#include <pthread.h>
#include "native-callrec.h"
#include <dlfcn.h>
#include <android/log.h>
#include <ostream>
#include <zconf.h>

AndroidAudioRecord *audiorecord;

bool getFunctionsAudioRecord(void *p_library) {
	ar_con = (AudioRecord_con) (fake_dlsym(p_library,
	                                       "_ZN7android11AudioRecordC1ERKNS_8String16E"));
	if (!ar_con) {
		LOGI("ar_con import fail");
	}

	ar_dtor = (AudioRecord_dtor) (fake_dlsym(p_library, "_ZN7android11AudioRecordD2Ev"));
	if (!ar_dtor) {
		LOGI("ar_dtor import fail");
	} else {
		LOGI("ar_dtor import success");
	}

	ar_start = (AudioRecord_start) (fake_dlsym(p_library,
	                                           "_ZN7android11AudioRecord5startENS_11AudioSystem12sync_event_tE15audio_session_t"));

	if (!ar_start)
		ar_start_below9 = (AudioRecord_start_below9) (fake_dlsym(p_library,
		                                                         "_ZN7android11AudioRecord5startEv"));

	ar_input_private = (AudioRecord_input_private) (fake_dlsym(p_library,
	                                                           "_ZNK7android11AudioRecord15getInputPrivateEv"));

	ar_stop = (AudioRecord_stop) (fake_dlsym(p_library,
	                                         "_ZN7android11AudioRecord4stopEv"));

	ar_read = (AudioRecord_read) (fake_dlsym(p_library,
	                                         "_ZN7android11AudioRecord4readEPvmb"));

	if (!ar_read)
		ar_read = (AudioRecord_read) (fake_dlsym(p_library,
		                                         "_ZN7android11AudioRecord4readEPvj"));

	if (!ar_read)
		ar_read = (AudioRecord_read) (fake_dlsym(p_library,
		                                         "_ZN7android11AudioRecord4readEPvjb"));


	return (ar_ctor24 || ar_ctor19 || ar_ctor16 || ar_ctor17 || ar_ctor8 || ar_ctor9)
	       && ar_dtor && (ar_start || ar_start_below9) && ar_stop && ar_read;

}

void getFunctionsAudioSystem(void *p_library) {
	setParameters = (int (*)(int, void *)) (fake_dlsym(p_library,
	                                                   "_ZN7android11AudioSystem13setParametersEiRKNS_7String8E"));
}

void getConstructorsAudioRecord(void *p_library) {/** Audio Record **/
	ar_ctor24 =
			(AudioRecord_ctor24) (fake_dlsym(p_library,
			                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EmPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tjiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor24 =
				(AudioRecord_ctor24) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC2E14audio_source_tj14audio_format_tjRKNS_8String16EmPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tjiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor24 =
				(AudioRecord_ctor24) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EmPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor24 =
				(AudioRecord_ctor24) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor24 =
				(AudioRecord_ctor24) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_jiNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor24 =
				(AudioRecord_ctor24) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tjiPK18audio_attributes_t"));
	if (!ar_ctor24)
		ar_ctor19 =
				(AudioRecord_ctor19) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_iiNS0_13transfer_typeE19audio_input_flags_t"));
	if (!ar_ctor19)
		ar_ctor17 =
				(AudioRecord_ctor17) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_ii"));
	if (!ar_ctor17)
		ar_ctor16 =
				(AudioRecord_ctor16) (fake_dlsym(p_library,
				                                 "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiNS0_12record_flagsEPFviPvS4_ES4_ii"));
	if (!ar_ctor16)
		ar_ctor9 = (AudioRecord_ctor9) (fake_dlsym(p_library,
		                                           "_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_ii"));
	if (!ar_ctor9)
		ar_ctor8 = (AudioRecord_ctor8) (fake_dlsym(p_library,
		                                           "_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_i"));
}

bool getFunctionsLibUtils(void *p_libraryutils) {
	string8 = (CreateString8) fake_dlsym(p_libraryutils, "_ZN7android7String8C2EPKc");
	string16 = (CreateString16) (fake_dlsym(p_libraryutils, "_ZN7android8String16C1EPKc"));

	string8(&str8_1, kvp_as4);
	if (!str8_1) {
		LOGI("Failed to create string8");
		return false;
	} else {
		LOGI("Successfully created string8");
	}
	//todo set your app name
	string16(&str16, "com.bourbonkk.nativerecorder");
	if (!str16) {
		LOGI("Failed to create string16");
		return false;
	} else {
		LOGI("Successfully created string16");
	}
	return true;
}


extern "C" JNIEXPORT jint JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_audioRecordSet(
		JNIEnv *env,
		jobject obj,
		jint inputsource, jint frequency, jint channel, jint audioEncoding, jint bufferSize
		/* this */) {
	audiorecord = new AndroidAudioRecord();

	int setResult = 0;
	setResult = audiorecord->set(inputsource, (uint32_t) (frequency), channel,
	                             (unsigned int) (audioEncoding), (unsigned int) bufferSize);

	return setResult;
}

extern "C" JNIEXPORT void JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_start(
		JNIEnv *env,
		jobject obj
		/* this */) {
	if (audiorecord != nullptr) {
		as = audiorecord->inputPrivate();
		int startResult = audiorecord->start();
		LOGI("audiostart result: %d", startResult);
		int setParam2 = setParameters(as, &str8_1);
		LOGI("setParam2 %d ", setParam2);
	} else {
		LOGI("audiorecord nullptr");
	}
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_read(
		JNIEnv *env,
		jobject obj,
		jint bufferSize
		/* this */) {
	audiorecord->lbuffer = malloc((size_t) bufferSize);
	audiorecord->size = bufferSize;
	audiorecord->read(audiorecord->lbuffer, bufferSize);
	auto *tmp = new jbyte[audiorecord->size];
	memcpy(tmp, audiorecord->lbuffer, size_t(audiorecord->size));
	jbyteArray lbuffer = env->NewByteArray(audiorecord->size);
	env->SetByteArrayRegion(lbuffer, 0, audiorecord->size, tmp);
	delete[] tmp;
	free(audiorecord->lbuffer);
	return lbuffer;
}


extern "C" JNIEXPORT void JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_stop(
		JNIEnv *env,
		jobject obj
		/* this */) {
	int stopResult = audiorecord->stop();
	LOGI("audiostop result: %d", stopResult);
}

extern "C" JNIEXPORT void JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_release(
		JNIEnv *env,
		jobject obj
		/* this */) {
	audiorecord->stop();
	audiorecord->close();
	delete audiorecord;
	LOGI("audiorecord close");
}


extern "C" JNIEXPORT jint JNICALL
Java_com_bourbonkk_nativerecorder_NativeSample_soFileLoad(
		JNIEnv *env,
		jobject obj/* this */
) {
	void *p_library;
	LOGI("dlopen_Audio_libaudioclient or libmedia");
	p_library = fake_dlopen("/system/lib64/libaudioclient.so", RTLD_NOW);
	if (!p_library) {
		p_library = fake_dlopen("/system/lib64/libmedia.so", RTLD_NOW);
		if (p_library) {
			LOGI("dlopen_Audio_libmedia success");
		} else {
			LOGI("can't dlopen Audio_libaudioclient or libmedia");
		}
	} else {
		LOGI("dlopen_Audio_libaudioclient success");
	}

	void *p_libraryutils;
	p_libraryutils = fake_dlopen("/system/lib64/libutils.so", RTLD_NOW);
	//p_libraryutils = dlopen("libutils.so", RTLD_NOW);
	if (p_libraryutils) {
		LOGI("dlopen_Audio_libutils success");
	} else {
		LOGI("dlopen_Audio_libutils fail");
	}

	if (!p_library || !p_libraryutils) {
		LOGI("Error: %s", dlerror());
		return JNI_FALSE;
	}

	getConstructorsAudioRecord(p_library);
	getFunctionsAudioRecord(p_library);
	getFunctionsAudioSystem(p_library);

	getFunctionsLibUtils(p_libraryutils);

	fake_dlclose(p_library);
	fake_dlclose(p_libraryutils);


	return JNI_TRUE;
}

AndroidAudioRecord::AndroidAudioRecord() :
		mAudioRecord(NULL) {
}

AndroidAudioRecord::~AndroidAudioRecord() {
	close();
}

void AndroidAudioRecord::close() {
	if (mAudioRecord) {
		if (ar_dtor) {
			ar_dtor(mAudioRecord);
		}
		free(mAudioRecord);
		mAudioRecord = NULL;
	} else {
		LOGI("mAudioRecord is Null");
	}
}

int
AndroidAudioRecord::set(int audio_source, uint32_t sampleRate, int format, unsigned int channels,
                        unsigned int size) {
	close();
	int result = 0;

	mAudioRecord = malloc(800);
	ar_con(mAudioRecord, &str16);

	LOGI("mAudioRecord is : %p", mAudioRecord);
	if (ar_ctor24) {
		result = ar_ctor24(mAudioRecord, 1, sampleRate, format, channels,
		                   &str16, size, NULL, NULL, 0, 0, TRANSFER_DEFAULT, AUDIO_INPUT_FLAG_NONE,
		                   NULL,
		                   NULL,
		                   nullptr);
	} else if (ar_ctor19) {
		ar_ctor19(mAudioRecord, audio_source, sampleRate, format, channels,
		          size, NULL, NULL, 0, 0, TRANSFER_DEFAULT,
		          AUDIO_INPUT_FLAG_NONE);
	} else if (ar_ctor17)
		ar_ctor17(mAudioRecord, audio_source, sampleRate, format, channels,
		          size, NULL, NULL, 0, 0);
	else if (ar_ctor16)
		ar_ctor16(mAudioRecord, audio_source, sampleRate, format, channels,
		          size, (record_flags) 0, NULL, NULL, 0, 0);
	else if (ar_ctor9)
		ar_ctor9(mAudioRecord, audio_source, sampleRate, format, channels, size,
		         0, NULL, NULL, 0, 0);
	else if (ar_ctor8)
		ar_ctor8(mAudioRecord, audio_source, sampleRate, format, channels, size,
		         0, NULL, NULL, 0);
	else
		return result;

	return result;
}

int AndroidAudioRecord::inputPrivate() {
	if (mAudioRecord && ar_input_private) {
		LOGI("start AndroidAudioRecord::inputPrivate");
		int res = ar_input_private(mAudioRecord);
		LOGI("stop AndroidAudioRecord::inputPrivate %i", res);
		return res;
	} else {
		LOGI("AndroidAudioRecord::inputPrivate err");
		return 0;
	}
}

int AndroidAudioRecord::start() {
	if (mAudioRecord) {
		if (ar_start) {
			LOGI("ar_start start");
			int startResult = ar_start(mAudioRecord, SYNC_EVENT_NONE, 0);
			LOGI("ar_start startResult : %d", startResult);
		} else {
			LOGI("ar_start_below9 start");
			ar_start_below9();
			return 1;
		}
		return 0;
	} else {
		LOGI("mAudioRecord is null in start");
		return 0;
	}
}

int AndroidAudioRecord::stop() {
	if (mAudioRecord && ar_stop) {
		LOGI("stop start return 1 ");
		ar_stop(mAudioRecord);
	} else {
		LOGI("stop err return 0 ");
		return 0;
	}
	return 1;
}

int AndroidAudioRecord::read(void *buffer, int size) {
	if (mAudioRecord && ar_read) {
		int read = ar_read(mAudioRecord, buffer, (unsigned int) size, true);
		return read;
	} else {
		LOGI("ar_read err");
		return 0;
	}
}
