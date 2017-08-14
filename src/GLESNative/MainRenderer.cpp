#include <jni.h>
#include "GLESNative.h"

extern "C" {

extern GLESNative *gGlesObject;

JNIEXPORT void JNICALL
Java_com_esgui_MainRenderer_DrawFrameNative(JNIEnv *env,
		jobject instance) {

	if (gGlesObject == NULL) {
		return;
	}
	gGlesObject->Render();

}

JNIEXPORT void JNICALL
Java_com_esgui_MainRenderer_SurfaceCreatedNative(JNIEnv *env,
		jobject instance) {

	if (gGlesObject == NULL) {
		return;
	}
    gGlesObject->Init();

}

JNIEXPORT void JNICALL
Java_com_esgui_MainRenderer_SurfaceChangedNative(JNIEnv *env,
		jobject instance,
		jint width,
		jint height) {

	if (gGlesObject == NULL) {
		return;
	}
	gGlesObject->SetViewport(width, height);

}

}
