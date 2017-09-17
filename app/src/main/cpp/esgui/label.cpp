#include "label.h"
#include "app.h"

namespace esgui
{

label::label(container* cont, int id)
	: widget(cont, id)
{
	m_font = app::get().default_font();
    m_text_color = "white";
    m_alignment = left | top;
}

void label::text(const std::string& l)
{
	m_label = l;
	refresh();
}

void label::color(const esgui::color& c)
{
	m_color = c;
	refresh();
}

void label::text_color(const esgui::color& c)
{
	m_text_color = c;
	refresh();
}

void label::font(const esgui::font& f)
{
	m_font = f;
	refresh();
}

void label::alignment(int a)
{
	m_alignment = a;
	refresh();
}

bool label::touch(action act, const point& p)
{
    if (act == action::up && m_on_click)
		m_on_click();
    return true;
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
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo1);

    std::vector<VertexData> vbo2;
	size s = esguid::MeasureText(m_label, m_font);
	float x = 0, y = 0;
	if (m_alignment & right)
		x = m_rect.w - s.x;
	else if (m_alignment & hcenter)
		x = (m_rect.w - s.x) / 2;
	if (m_alignment & bottom)
		y = m_rect.h - s.y;
	else if (m_alignment & vcenter)
		y = (m_rect.h - s.y) / 2;

	PushText(vbo2, m_rect.x + x, m_rect.y + y, m_label, m_font, m_text_color);
    m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo2);
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