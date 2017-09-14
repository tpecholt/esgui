#include "button.h"
#include "app.h"

namespace esgui
{

button::button(container* cont)
	: widget(cont)
{
	m_font = app::get().default_font();
}

button& button::text(const std::string& l)
{
	m_label = l;
	refresh();
	return *this;
}

button& button::color(const esgui::color& c)
{
	m_color = c;
	refresh();
	return *this;
}

button& button::text_color(const esgui::color& c)
{
	m_text_color = c;
	refresh();
	return *this;
}

button& button::font(const esgui::font& f)
{
	m_font = f;
	refresh();
	return *this;
}

button& button::image(const std::string& uri)
{
	m_image = app::get().texture(uri, m_image_size);
	refresh();
	return *this;
}

void button::touch(action act, const point& p)
{
	if (act == action::up && m_on_click)
		m_on_click();
}

void button::refresh()
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

	PushRect(vbo2, m_rect.x + (m_rect.w - m_image_size.x) / 2, m_rect.y + (m_rect.h - m_image_size.y) / 2, m_image_size.x, m_image_size.y, "white");
    check_err();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1].id);
	glBufferData(GL_ARRAY_BUFFER, vbo2.size() * sizeof(VertexData), vbo2.data(), GL_STATIC_DRAW);
    check_err();
    m_vbos[1].size = vbo2.size();
	m_vbos[1].texture = m_image;
}

size button::min_size()
{
    return m_image_size;
}

}