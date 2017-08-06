#ifndef GLESCONTEXTANDROID_GLES_CONTEXT_H
#define GLESCONTEXTANDROID_GLES_CONTEXT_H

#include "esgui/esgui.h"
#include "esgui/label.h"
#include "esgui/popup.h"
#include "esgui/layout.h"
#include <sstream>
#include <iostream>
#include <GLES2/gl2.h>
#include <string>
#include <array>
#include <android/log.h>

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
	bool IsInitDone() { return m_initDone; }	

private:
	bool m_initDone;
	std::array<esgui::label, 3> m_labels;
	esgui::popup m_popup;
	esgui::layout* m_layout;
};

#endif //GLESCONTEXTANDROID_GLES_CONTEXT_H