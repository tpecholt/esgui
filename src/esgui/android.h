#pragma once
#include <string>

namespace android
{

int CreateFontAtlas(const char* face, bool bold, bool italic, float* spacing, float* ascent, float* descent);

std::pair<float, float> GetScreenSize();

int GetDPI();

}