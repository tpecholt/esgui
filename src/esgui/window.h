#pragma once

#include "event.h"
#include <string>
#include <vector>

namespace esgui
{

struct point
{
	float x, y;
	point() : x(), y() {}
	point(float x, float y) : x(x), y(y) {}
};

using size = point;

struct rect
{
	float x, y, w, h;
	rect() : x(), y(), w(-1), h(-1) {}
	rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	bool invalid() const { return w < 1 || h < 1; }
	bool operator== (const rect& r) {
		return x == r.x && y == r.y && w == r.w && h == r.h;
	}
	bool operator!= (const rect& r) {
		return !(*this == r);
	}
	float area() const {
		return w*h;
	}
	bool contains(const point& p) const {
		return x <= p.x && y <= p.y && x + w >= p.x && y + h >= p.y;
	}
	rect& enlarge(const point& p) {
		float x2 = std::max(x + w, p.x);
		float y2 = std::max(y + h, p.y);
		x = std::min(x, p.x);
		y = std::min(y, p.y);
		w = x2 - x;
		h = y2 - y;
		return *this;
	}
	rect& enlarge(const rect& r) {
		enlarge({ r.x, r.y });
		enlarge({ r.x + r.w, r.y + r.h });
		return *this;
	}
	rect& intersect(const rect& r) {
		float x2 = std::min(x + w, r.x + w);
		float y2 = std::min(y + h, r.y + h);
		x = std::max(x, r.x);
		y = std::max(y, r.y);
		w = x2 - x;
		h = y2 - h;
		return *this;
	}
};

struct color
{
	float r, g, b, a;
	color() : r(1), g(1), b(1), a(1) {}
	color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}
	color(const char* name);
};

struct font
{
	enum {
		bold = 0x1,
		italic = 0x2,
	};
	font();
	font(int texture, int pointSize);
	font(const std::string& face, int pointSize, int style = 0);
	int texture() const { return m_texture; }	
	int point_size() const { return m_point_size; }

private:
	int m_texture;
	int m_point_size;
};

class window
{
public:
	window()
		: m_visible(true) 
	{}
	virtual ~window() {}

	void visible(bool s) { m_visible = s; }
	bool visible() const { return m_visible; }
	virtual void rect(const rect& r) {}
	virtual const esgui::rect& rect() const { static esgui::rect r; return r; }
	virtual int z() const { return 0; }
	virtual size min_size() { return size(0, 0); }
	virtual void refresh() {}
	virtual void render(const int programs[]) {}
	virtual void touch(action act, const point& p) {}

protected:
	bool m_visible;
};

}