#include <jni.h>
#include "GLESNative.h"

extern "C" {

extern GLESNative *gGlesObject;

JNIEXPORT void JNICALL
	Java_com_esgui_MainView_OnTouchEvent(JNIEnv *env,
		jobject instance,
		jint action,
		jfloat x,
		jfloat y) 
{
	if (gGlesObject == NULL) {
		return;
	}
	gGlesObject->OnTouchEvent(action, x, y);

}

}