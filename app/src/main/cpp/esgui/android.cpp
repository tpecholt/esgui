#include "android.h"
#include "impl.h"
#include <jni.h>

namespace android {

struct ThreadEnv {
    JavaVM *jvm;
    JNIEnv *env;

    ThreadEnv(JavaVM *vm)
            : jvm(vm), env(nullptr) {
        if (jvm)
            jvm->AttachCurrentThread(&env, nullptr);
    }

    ~ThreadEnv() {
        if (jvm)
            jvm->DetachCurrentThread();
    }
};

static JavaVM *jvm;

JNIEnv *GetEnv() {
    static thread_local ThreadEnv e(jvm);
    return e.env;
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad");
    jvm = vm;
    /*if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
        return -1;	*/
    return JNI_VERSION_1_6; /* the required JNI version */
}

int CreateFontAtlas(const char *face, bool bold, bool italic, bool underline, float *spacing, float *ascent,
                    float *descent) {
    JNIEnv *env = GetEnv();
    if (!env)
        return 0;
    jclass cls = env->FindClass("com/tope/esgui/core");
    if (!cls)
        return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "createFontAtlas", "(Ljava/lang/String;ZZZ)I");
    if (!mid)
        return 0;
    jstring jface = env->NewStringUTF(face);
    int texture = env->CallStaticIntMethod(cls, mid, jface, bold, italic, underline);
    mid = env->GetStaticMethodID(cls, "getFontSpacing", "(Ljava/lang/String;ZZZ)[F");
    if (!mid)
        return 0;
    jfloatArray arr = (jfloatArray) env->CallStaticObjectMethod(cls, mid, jface, bold, italic, underline);
    jfloat *data = env->GetFloatArrayElements(arr, 0);
    int size = esguid::FONT_ATLAS_COLS * esguid::FONT_ATLAS_COLS;
    *ascent = data[size - 2];
    *descent = data[size - 1];
    for (int i = 0; i < size - 2; ++i)
        spacing[i] = data[i];
    env->ReleaseFloatArrayElements(arr, data, 0);
    return texture;
}

int LoadTexture(const char* folder, const char* name, float& w, float& h)
{
    JNIEnv *env = GetEnv();
    if (!env)
        return 0;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "loadTexture", "(Ljava/lang/String;Ljava/lang/String;)[I");
    if (!mid)
        return 0;
    jstring jfolder = env->NewStringUTF(folder);
    jstring jname = env->NewStringUTF(name);
    jintArray arr = (jintArray)env->CallStaticObjectMethod(cls, mid, jfolder, jname);
    jint* data = env->GetIntArrayElements(arr, 0);
    int texture = data[0];
    w = data[1];
    h = data[2];
    env->ReleaseIntArrayElements(arr, data, 0);
    return texture;
}

int GetStatusBarHeight()
{
    JNIEnv* env = GetEnv();
    if (!env)
        return 0;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "getStatusBarHeight", "()I");
    if (!mid)
        return 0;
    return env->CallStaticIntMethod(cls, mid);
}

void SetDarkStatusBar(bool dark)
{
    JNIEnv* env = GetEnv();
    if (!env)
        return;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return;
    jmethodID mid = env->GetStaticMethodID(cls, "setDarkStatusBar", "(Z)V");
    if (!mid)
        return;
    env->CallStaticVoidMethod(cls, mid, dark);
}

void ToastMessage(const char *msg)
{
    JNIEnv* env = GetEnv();
    if (!env)
        return;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return;
    jmethodID mid = env->GetStaticMethodID(cls, "toastMessage", "(Ljava/lang/String;)V");
    if (!mid)
        return;
    jstring jmsg = env->NewStringUTF(msg);
    env->CallStaticVoidMethod(cls, mid, jmsg);
}


void ShowKeyboard(int kb)
{
    JNIEnv* env = GetEnv();
    if (!env)
        return;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return;
    jmethodID mid = env->GetStaticMethodID(cls, "showKeyboard", "(I)V");
    if (!mid)
        return;
    env->CallStaticVoidMethod(cls, mid, kb);
}

int OpenCamera(bool open)
{
    JNIEnv* env = GetEnv();
    if (!env)
        return 0;
    jclass cls = env->FindClass("com/tope/esgui/MainActivity");
    if (!cls)
        return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "openCamera", "(Z)I");
    if (!mid)
        return 0;
    return env->CallStaticIntMethod(cls, mid, open);
}

std::pair<float, float> GetScreenSize()
{
	JNIEnv* env = GetEnv();
	if (!env)
		return {};
	jclass cls = env->FindClass("com/tope/esgui/core");
	if (!cls)
		return {};
	jmethodID mid = env->GetStaticMethodID(cls, "getScreenSize", "()[I");
	if (!mid)
		return {};
	jintArray arr = (jintArray)env->CallStaticObjectMethod(cls, mid);
	jint *data = env->GetIntArrayElements(arr, 0);
	std::pair<float, float> ret{ data[0], data[1] };
	env->ReleaseIntArrayElements(arr, data, 0);
	return ret;
}

int GetDPI()
{
	JNIEnv* env = GetEnv();
	if (!env)
		return {};
	jclass cls = env->FindClass("com/tope/esgui/core");
	if (!cls)
		return {};
	jmethodID mid = env->GetStaticMethodID(cls, "getDPI", "()I");
	if (!mid)
		return {};
	return env->CallStaticIntMethod(cls, mid);	
}

}