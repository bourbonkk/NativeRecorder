
#ifndef NATIVERECORD_NATIVE_CALLREC_H
#define NATIVERECORD_NATIVE_CALLREC_H

#define  LOG_TAG    "native-callrec"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

/** Events used by AudioRecord callback function (callback_t).
 *
 * to keep in sync with frameworks/base/media/java/android/media/AudioRecord.java
 */
enum sync_event_t {
	SYNC_EVENT_SAME = -1, SYNC_EVENT_NONE = 0, SYNC_EVENT_PRESENTATION_COMPLETE,

	SYNC_EVENT_CNT,
};

/**Type to transfer buffer, i use TRANSFER_DEFAULT as a default parameter for construtor */
enum transfer_type {
	TRANSFER_DEFAULT, // not specified explicitly; determine from other parameters
	TRANSFER_CALLBACK,  // callback EVENT_MORE_DATA
	TRANSFER_OBTAIN, // FIXME deprecated: call obtainBuffer() and releaseBuffer()
	TRANSFER_SYNC,      // synchronous read()
};

/**Input mode, use AUDIO_INPUT_FLAG_NONE as default parameter in constructor */
typedef enum {
	AUDIO_INPUT_FLAG_NONE = 0x0,        // no attributes
	AUDIO_INPUT_FLAG_FAST = 0x1, // prefer an input that supports "fast tracks"
} audio_input_flags_t;

/**A bitmask of acourstic values. It enables AGC, NS, IIR */
enum record_flags {
	RECORD_AGC_ENABLE = 0x0001,
	RECORD_NS_ENABLE = 0x0002,
	RECORD_IIR_ENABLE = 0x0004,
};

extern "C" {

typedef int (*AudioRecord_ctor24)(void *, int, unsigned int, int, unsigned int, void *,
                                   unsigned long, void (*)(int, void *, void *), void *, int, int,
                                   transfer_type, int, int, int, void *);
typedef void (*AudioRecord_ctor19)(void *, int, unsigned int, int, unsigned int,
                                   int, void (*)(int, void *, void *), void *, int, int,
                                   transfer_type,
                                   int);
typedef void (*AudioRecord_ctor17)(void *, int, unsigned int, int, unsigned int,
                                   int, void (*)(int, void *, void *), void *, int, int);
typedef void (*AudioRecord_ctor16)(void *, int, unsigned int, int, unsigned int,
                                   int, record_flags, void (*)(int, void *, void *), void *, int,
                                   int);
typedef void (*AudioRecord_ctor9)(void *, int, unsigned int, int, unsigned int,
                                  int, unsigned int, void (*)(int, void *, void *), void *, int,
                                  int);
typedef void (*AudioRecord_ctor8)(void *, int, unsigned int, int, unsigned int,
                                  int, unsigned int, void (*)(int, void *, void *), void *, int);
typedef void (*AudioRecord_dtor)(void *);
typedef int (*AudioRecord_start)(void *, int, int);
typedef int (*AudioRecord_start_below9)();
typedef void (*AudioRecord_stop)(void *);
typedef void (*CreateString16)(void *, const char *);
typedef void (*CreateString8)(void *, const char *);
typedef int (*SetParameters)(int, void *);
typedef int (*AudioRecord_read)(void *, void *, unsigned int, bool);
typedef int (*AudioRecord_input_private)(void *);
typedef void (*AudioRecord_con)(void* , void *);

static CreateString16 string16;
static CreateString16 string8;
static SetParameters setParameters;
static AudioRecord_input_private ar_input_private;

static AudioRecord_ctor24 ar_ctor24;
static AudioRecord_ctor19 ar_ctor19;
static AudioRecord_ctor17 ar_ctor17;
static AudioRecord_ctor16 ar_ctor16;
static AudioRecord_ctor9 ar_ctor9;
static AudioRecord_ctor8 ar_ctor8;
static AudioRecord_dtor ar_dtor;
static AudioRecord_start ar_start;
static AudioRecord_start_below9 ar_start_below9;
static AudioRecord_stop ar_stop;
static AudioRecord_read ar_read;
static AudioRecord_con ar_con;

static void *str16 = nullptr;
static void *str8_1 = nullptr;
static const char *kvp_def = "input_source=4";
static const char *kvp_as4 = "format=1;input_source=4;routing=-2147483584";
static const char *kvp_as4_1 = "format=1;input_source=4;routing=-2147483644";

static int as;

class AndroidAudioRecord {
private:
	void *mAudioRecord; //Instance of audiorecord
public:
	void *lbuffer;
	int size;

	AndroidAudioRecord();

	virtual ~AndroidAudioRecord();

	void close();

	int set(int audio_source, uint32_t sampleRate, int format,
	         unsigned int channels, unsigned int size);

	int inputPrivate();

	int start();

	int stop();

	int read(void *buffer, int size);

};

extern void *fake_dlopen(const char *filename, int flags);
extern int fake_dlclose(void *handle);
extern void *fake_dlsym(void *handle, const char *symbol);

extern AndroidAudioRecord *audiorecord;

}


#endif //NATIVERECORD_NATIVE_CALLREC_H
