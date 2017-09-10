#include "window.h"
#include "app.h"

namespace esgui
{

font::font()
	: m_texture(), m_point_size()
{}

font::font(int m_texture, int pointSize)
	: m_texture(m_texture), m_point_size(pointSize)
{}

font::font(const std::string& face, int pointSize, int style)
	: m_point_size(pointSize)
{
	m_texture = app::get().font_texture(face, style);
}

//-----------------------------------------------------------------------

color::color(const char* name)
{
	static std::map<std::string, color> colors{
		{ "black", color(0, 0, 0) },
		{ "white", color(1, 1, 1) },
		{ "grey", color(0.7, 0.7, 0.7) },
		{ "red", color(1, 0, 0) },
		{ "green", color(0, 1, 0) },
		{ "blue", color(0, 0, 1) },
		{ "yellow", color(1, 1, 0) },
	};
	*this = colors[name];
}

}