#pragma once
#include <string>

namespace android
{

int CreateFontAtlas(const char* face, bool bold, bool italic, float* spacing, float* ascent, float* descent);

int LoadTexture(const char* uri, float& w, float& h);

std::pair<float, float> GetScreenSize();

int GetDPI();

void ToastMessage(const char* msg);

}