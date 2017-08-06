#include <jni.h>
#include "GLESNative.h"

extern "C" {

GLESNative *gGlesObject;

JNIEXPORT void JNICALL
Java_com_esgui_MainActivity_CreateObjectNative(JNIEnv *env,	jobject instance) {
	gGlesObject = new GLESNative();

}

JNIEXPORT void JNICALL
Java_com_esgui_MainActivity_DeleteObjectNative(JNIEnv *env,	jobject instance) {
	delete gGlesObject;
	gGlesObject = nullptr;
}

JNIEXPORT jboolean JNICALL
Java_com_esgui_MainActivity_IsInitsDoneNative(JNIEnv *env,	jobject instance) {

	if (!gGlesObject || !gGlesObject->IsInitDone()) {
		return JNI_FALSE;
	}
	else {
		return JNI_TRUE;
	}

}

}
