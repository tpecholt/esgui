#pragma once

#include "widget.h"
#include <android/log.h>
#include <GLES2/gl2.h>

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "esgui", __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "esgui", __VA_ARGS__)

namespace esguid
{

constexpr int FONT_ATLAS_COLS = 10;
constexpr float FONT_ATLAS_POINT_SIZE = 24;

#pragma pack(4)
struct VertexData
{
	float x, y;
	float u, v;
	float r, g, b, a;
	VertexData() {}
	VertexData(float x, float y, const esgui::color& c) : x(x), y(y), r(c.r), g(c.g), b(c.b), a(c.a) {}
	VertexData(float x, float y, float u, float v, const esgui::color& c) : x(x), y(y), u(u), v(v), r(c.r), g(c.g), b(c.b), a(c.a) {}
};

void PushRect(std::vector<VertexData>& vbo, float x1, float y1, float x2, float y2, const esgui::color& c);

void PushText(std::vector<VertexData>& vbo, float x, float y, const std::string& label, const esgui::font& f, const esgui::color& clr);

esgui::size MeasureText(const std::string& label, const esgui::font& f);

}