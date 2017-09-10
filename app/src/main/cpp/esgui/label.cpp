#include "label.h"
#include "app.h"

namespace esgui
{

label::label(container* cont)
	: widget(cont)
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

void label::touch(action act, const point& p) 
{
	if (act == action::up && m_on_click)
		m_on_click();
}

void label::refresh()
{
    check_err();
	if (m_vbos.size() != 2) {
		m_vbos.resize(2);
		glGenBuffers(1, &m_vbos[0].id);
		glGenBuffers(1, &m_vbos[1].id);		
	}

    check_err();
    using namespace esguid;
	std::vector<VertexData> vbo1;
	PushRect(vbo1, m_rect.x, m_rect.y, m_rect.w, m_rect.h, m_color);
    check_err();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0].id);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
    check_err();
    m_vbos[0].size = vbo1.size();

    std::vector<VertexData> vbo2;
	PushText(vbo2, m_rect.x, m_rect.y, m_label, m_font, m_text_color);
    check_err();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1].id);
	glBufferData(GL_ARRAY_BUFFER, vbo2.size() * sizeof(VertexData), vbo2.data(), GL_STATIC_DRAW);
    check_err();
    m_vbos[1].size = vbo2.size();
	m_vbos[1].texture = m_font.texture();
}

size label::min_size()
{
    check_err();
	size s = esguid::MeasureText(m_label, m_font);
    check_err();
    return s;
}

}