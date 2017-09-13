#include "impl.h"
#include "app.h"
#include <cmath>

namespace esguid {

void PushRect(std::vector<VertexData>& vbo, float x, float y, float w, float h, const esgui::color& c)
{
	size_t size = vbo.size();
	vbo.reserve(vbo.size() + 6);
	vbo.push_back(VertexData{ x, y, c });
	vbo.push_back(VertexData{ x, y + h, c });
	vbo.push_back(VertexData{ x + w, y + h, c });
	vbo.push_back(vbo[size]);
	vbo.push_back(vbo[size + 2]);
	vbo.push_back(VertexData{ x + w, y, c });
}

void PushRadio(std::vector<VertexData>& vbo, bool on, float x1, float y1, float r, const esgui::color& c)
{
	const float da = 20*M_PI/180.0;
	float lastcs = 1, lastsn = 0;
	float rm = 0.8 * r;
	for (float a = da; a < 2.01*M_PI; a += da)
	{
		float cs = cos(a);
		float sn = -sin(a);
		size_t size = vbo.size();
		vbo.push_back({ x1 + rm*lastcs, y1 + rm*lastsn, c});
		vbo.push_back({ x1 + r*lastcs, y1 + r*lastsn, c});
		vbo.push_back({ x1 + r*cs, y1 + r*sn, c});
		vbo.push_back(vbo[size]);
		vbo.push_back(vbo[size + 2]);
		vbo.push_back({ x1 + rm*cs, y1 + rm*sn, c});
        lastcs = cs;
        lastsn = sn;
	}
    if (on) {
        rm = 0.5 * r;
        lastcs = 1;
        lastsn = 0;
        for (float a = da; a < 2.01 * M_PI; a += da) {
            float cs = cos(a);
            float sn = -sin(a);
            vbo.push_back({x1, y1, c});
            vbo.push_back({x1 + rm * lastcs, y1 + rm * lastsn, c});
            vbo.push_back({x1 + rm * cs, y1 + rm * sn, c});
            lastcs = cs;
            lastsn = sn;
        }
    }
}

void PushText(std::vector<VertexData>& vbo, float x, float y, const std::string& label, const esgui::font& f, const esgui::color& clr)
{
	using namespace esgui;
	const font_metrics& metrics = app::get().font_metrics(f);
	//size ssize = app::get().screen_size();
	int dpi = app::get().screen_dpi();
	vbo.reserve(vbo.size() + 6 * label.size());
	point p{ x, y };
	const float dy = f.point_size() / 72.f * dpi;
	const float dx = dy;
	const float duv = 1.0 / FONT_ATLAS_COLS;
	const float desc = metrics.descent * duv;
	for (size_t i = 0; i < label.size(); ++i)
	{
		size_t size = vbo.size();
		int c = label[i] - 32;
		float u = (c % FONT_ATLAS_COLS) * duv;
		float v = (c / FONT_ATLAS_COLS) * duv;

		vbo.push_back(VertexData(p.x, p.y, u, v + duv * 0.97 - desc, clr));
		vbo.push_back(VertexData(p.x, p.y + dy, u, v - desc, clr));
		vbo.push_back(VertexData(p.x + dx, p.y + dy, u + duv * 0.97, v - desc, clr));

		vbo.push_back(vbo[size]);
		vbo.push_back(vbo[size + 2]);
		vbo.push_back(VertexData(p.x + dx, p.y, u + duv * 0.97, v + duv * 0.97 - desc, clr));

		p.x += dx * metrics.char_widths[c];
	}
}

esgui::size MeasureText(const std::string& label, const esgui::font& f)
{
	using namespace esgui;
	const font_metrics& metrics = app::get().font_metrics(f);
	int dpi = app::get().screen_dpi();
	const float dy = f.point_size() / 72.f * dpi;
	const float dx = dy;

	float w = 0, h = dy * (metrics.ascent + metrics.descent);
	for (size_t i = 0; i < label.size(); ++i)
	{
		int c = label[i] - 32;
		w += dx * metrics.char_widths[c];
	}
	return { w, h };
}

}