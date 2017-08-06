#pragma once

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
	bool operator== (const rect& r) {
		return x == r.x && y == r.y && w == r.w && h == r.h;
	}
	bool operator!= (const rect& r) {
		return !(*this == r);
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

class widget
{
public:
	widget();
	~widget();
	void visible(bool s) { m_visible = s; }
	bool visible() const { return m_visible; }
	void rect(const rect& r);
	const esgui::rect& rect() const { return m_rect; }
	void refresh();
	void render(const int programs[]);
	
	virtual size min_size() { return size(0, 0); }
	
protected:
	virtual void do_refresh() = 0;	

protected:
	unsigned m_vbos[2];
	int m_vbo_sizes[2];
	int m_texture;
	bool m_visible;
	esgui::rect m_rect;
};

}