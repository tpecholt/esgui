#include <jni.h>
#include "app.h"

extern void init_controls();

extern "C" {

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainActivity_CreateObjectNative(JNIEnv *env, jobject instance)
{
     //too early for any opengl stuff here
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainActivity_DeleteObjectNative(JNIEnv *env, jobject instance)
{
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainActivity_OnBackPressedNative(JNIEnv *env, jobject instance)
{
	esgui::app::get().touch(esgui::action::cancel, 0, 0);
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainRenderer_SurfaceCreatedNative(JNIEnv *env,
                                                      jobject instance)
{
    init_controls();
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainRenderer_SurfaceChangedNative(JNIEnv *env,
		jobject instance,
		jint width,
		jint height)
{
	esgui::app::get().set_viewport(width, height);
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainRenderer_DrawFrameNative(JNIEnv *env,
		jobject instance)
{
	esgui::app::get().render();
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainView_OnTouchEventNative(JNIEnv *env,
		jobject instance,
		jint action,
		jfloat x,
		jfloat y) 
{
	esgui::action act{};
	switch (action) {
		case 0:
			act = esgui::action::down;
			break;
		case 1:
			act = esgui::action::up;
			break;
		case 2:
			act = esgui::action::move;
			break;
	}
	esgui::app::get().touch(act, x, y);
}

}
