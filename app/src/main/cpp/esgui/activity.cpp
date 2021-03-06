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
	esgui::app::get().press(0x1b);
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainActivity_OnKeyNative(JNIEnv *env, jobject instance, jint code)
{
	esgui::app::get().press(code);
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainActivity_SetKbdHeightNative(JNIEnv *env, jobject instance, jint h)
{
    esgui::app::get().kbd_height(h);
}

JNIEXPORT void JNICALL
Java_com_tope_esgui_MainRenderer_SurfaceCreatedNative(JNIEnv *env,
                                                      jobject instance)
{
	static bool done = false;
	if (done) //display rotation
		return;
	done = true;
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
            //LOGI("down");
			break;
		case 1:
			act = esgui::action::up;
            //LOGI("up");
            break;
		case 2:
			act = esgui::action::move;
            //LOGI("move");
            break;
	}
	esgui::app::get().touch(act, x, y);
}

}
