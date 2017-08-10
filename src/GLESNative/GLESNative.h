#ifndef GLESCONTEXTANDROID_GLES_CONTEXT_H
#define GLESCONTEXTANDROID_GLES_CONTEXT_H

#include <sstream>
#include <iostream>
#include <GLES2/gl2.h>
#include <string>
#include <array>
#include <android/log.h>
#include "esgui/popup.h"

#define LOG_TAG "GLESContextAndroid"
#define  MyLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  MyLOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  MyLOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,__VA_ARGS__)
#define  MyLOGI(...)  __android_log_print(ANDROID_LOG_INFO   , LOG_TAG,__VA_ARGS__)
#define  MyLOGW(...)  __android_log_print(ANDROID_LOG_WARN   , LOG_TAG,__VA_ARGS__)
#define  MyLOGF(...)  __android_log_print(ANDROID_LOG_FATAL   , LOG_TAG,__VA_ARGS__)
#define MyLOGSIMPLE(...)

class GLESNative {
public:
	GLESNative();
	~GLESNative();
	void Init();
	void Render();
	void SetViewport(int width, int height);
	void OnTouchEvent(int action, float x, float y);
	bool IsInitDone() { return m_initDone; }	

private:
	bool m_initDone;
	esgui::popup* m_pop;
};

#endif //GLESCONTEXTANDROID_GLES_CONTEXT_H