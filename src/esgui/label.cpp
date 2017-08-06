#include "label.h"
#include "esgui.h"

namespace esgui
{

label::label()
{
	m_font = app::get().default_font();
}

label& label::text(const std::string& l)
{
	m_label = l;
	refresh();
	return *this;
}

label& label::color(const esgui::color& c)
{
	m_color = c;
	refresh();
	return *this;
}

label& label::text_color(const esgui::color& c)
{
	m_text_color = c;
	refresh();
	return *this;
}

label& label::font(const esgui::font& f)
{
	m_font = f;
	refresh();
	return *this;
}

void label::do_refresh()
{
	using namespace esguid;
	std::vector<VertexData> vbo1;
	PushRect(vbo1, m_rect.x, m_rect.y, m_rect.w, m_rect.h, m_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
	m_vbo_sizes[0] = vbo1.size();

	std::vector<VertexData> vbo(6 * m_label.size());
	PushText(vbo, m_rect.x, m_rect.y, m_label, m_font, m_text_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(VertexData), vbo.data(), GL_STATIC_DRAW);
	m_vbo_sizes[1] = vbo.size();
	m_texture = m_font.texture();
}

size label::min_size()
{
	return esguid::MeasureText(m_label, m_font);
}

}