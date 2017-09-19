#pragma once
#include <string>

namespace android
{

int CreateFontAtlas(const char* face, bool bold, bool italic, bool underline, float* spacing, float* ascent, float* descent);

int LoadTexture(const char* folder, const char* name, float& w, float& h);

std::pair<float, float> GetScreenSize();

int GetDPI();

int GetStatusBarHeight();

void SetDarkStatusBar(bool dark);

void ToastMessage(const char* msg);

void ShowKeyboard(int kb);

}