#include "button.h"
#include "app.h"

namespace esgui
{

button::button(container* cont)
	: widget(cont), m_image()
{
	m_font = app::get().default_font();
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

button& button::text(const std::string& l)
{
    m_label = l;
    m_image = 0;
    refresh();
    return *this;
}

button& button::image(const std::string& uri)
{
    m_label.clear();
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
    float dpmm = app::get().screen_dpi() / 25.4;
    check_err();
	if (m_vbos.size() != 2) {
		m_vbos.resize(2);
		glGenBuffers(1, &m_vbos[0].id);
		glGenBuffers(1, &m_vbos[1].id);		
	}

    check_err();
    using namespace esguid;
	std::vector<VertexData> vbo1;
	PushRoundRect(vbo1, m_rect.x, m_rect.y, m_rect.w, m_rect.h, 0.5*dpmm, m_color);
    check_err();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0].id);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
    check_err();
    m_vbos[0].size = vbo1.size();

    std::vector<VertexData> vbo2;

    if (m_image) {
        PushRect(vbo2,
                 m_rect.x + (m_rect.w - m_image_size.x) / 2,
                 m_rect.y + (m_rect.h - m_image_size.y) / 2,
                 m_image_size.x, m_image_size.y,
                 "white");
        m_vbos[1].texture = m_image;
    }
    else {
        size siz = esguid::MeasureText(m_label, m_font);
        PushText(vbo2,
                 m_rect.x + (m_rect.w - siz.x) / 2,
                 m_rect.y + (m_rect.h - siz.y) / 2,
                 m_label, m_font, m_text_color);
        m_vbos[1].texture = m_font.texture();
    }
    check_err();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1].id);
	glBufferData(GL_ARRAY_BUFFER, vbo2.size() * sizeof(VertexData), vbo2.data(), GL_STATIC_DRAW);
    check_err();
    m_vbos[1].size = vbo2.size();

}

size button::min_size()
{
    size siz;
    if (m_image)
        siz = m_image_size;
    else
        siz = esguid::MeasureText(m_label, m_font);
    float dpmm = app::get().screen_dpi() / 25.4;
    return { siz.x + 2*dpmm, siz.y + 2*dpmm };
}

}