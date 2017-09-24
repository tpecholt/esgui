#include "button.h"
#include "app.h"
#include <cmath>

namespace esgui
{

button::button(container* cont, int id)
	: widget(cont, id), m_style(flat)
{
	m_font = app::get().default_font();
    m_text_color = app::get().theme().button_text;
}

void button::style(enum style s)
{
    m_style = s;
    refresh();
}

void button::color(const esgui::color& c)
{
	m_color = c;
	refresh();
}

void button::text_color(const esgui::color& c)
{
	m_text_color = c;
	refresh();
}

void button::font(const esgui::font& f)
{
	m_font = f;
	refresh();
}

void button::text(const std::string& l)
{
    m_label = l;
    refresh();
}

void button::icon(const std::string& uri, const esgui::size& siz)
{
    m_uri = uri;
    m_img_size = siz;
    refresh();
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
    /*for (const auto vbo : m_vbos)
        glDeleteBuffers(1, &vbo.id);*/
	if (m_vbos.size() != 2) {
		m_vbos.resize(2);
		glGenBuffers(1, &m_vbos[0].id);
		glGenBuffers(1, &m_vbos[1].id);		
	}

    check_err();
    using namespace esguid;
	std::vector<VertexData> vbo;
    switch (m_style) {
        case flat:
            PushRoundRect(vbo, m_rect.x, m_rect.y, m_rect.w, m_rect.h, 0.5*dpmm, m_color);
            break;
        case round:
            float r = std::min(m_rect.w, m_rect.h) / 2;
            PushPie(vbo, m_rect.x + m_rect.w/2, m_rect.y + m_rect.h/2, r, 0, 2*M_PI, m_color);
            break;
    }
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo);

    vbo.clear();
    if (!m_uri.empty()) {
        esgui::size siz;
        m_vbos[1].texture = app::get().icon_texture(m_uri, siz);
        if (m_img_size.x >= 0)
            siz.x = m_img_size.x;
        if (m_img_size.y >= 0)
            siz.y = m_img_size.y;
        PushRect(vbo,
                 m_rect.x + (m_rect.w - siz.x) / 2,
                 m_rect.y + (m_rect.h - siz.y) / 2,
                 siz.x, siz.y,
                 "white");
    }
    else {
        size siz = esguid::MeasureText(m_label, m_font);
        PushText(vbo,
                 m_rect.x + (m_rect.w - siz.x) / 2,
                 m_rect.y + (m_rect.h - siz.y) / 2,
                 m_label, m_font, m_text_color);
        m_vbos[1].texture = m_font.texture();
    }
    m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo);
}

size button::min_size()
{
    size siz;
    if (!m_uri.empty()) {
        app::get().icon_texture(m_uri, siz);
        if (m_img_size.x >= 0)
            siz.x = m_img_size.x;
        if (m_img_size.y >= 0)
            siz.y = m_img_size.y;

    }
    else {
        siz = esguid::MeasureText(m_label, m_font);
    }
    float dpmm = app::get().screen_dpi() / 25.4;
    return { siz.x + 2*dpmm, siz.y + 2*dpmm };
}

}