//
// Created by tomas on 17.09.2017.
//

#include "edit_text.h"
#include "app.h"
#include "impl.h"
#include "android.h"

namespace esgui {

edit_text::edit_text(container* cont, int id)
        : widget(cont, id), m_style(all), m_focused()
{
    m_font = app::get().default_font();
    m_color = app::get().theme().background;
    m_text_color = app::get().theme().edit_text;
}

void edit_text::style(enum style s)
{
    m_style = s;
    refresh();
}

void edit_text::color(const esgui::color& c)
{
    m_color = c;
    refresh();
}

void edit_text::text_color(const esgui::color& c)
{
    m_text_color = c;
    refresh();
}

void edit_text::font(const esgui::font& f)
{
    m_font = f;
    refresh();
}

void edit_text::text(const std::string& l)
{
    m_text = l;
    refresh();
}

void edit_text::hint(const std::string& l)
{
    m_hint = l;
    refresh();
}

void edit_text::focused(bool f)
{
    m_focused = f;
    refresh();
}

bool edit_text::touch(action act, const point& p)
{
    esgui::rect r = m_rect;
    switch (act) {
        case action::down:
            m_focused = true;
            refresh();
            android::ShowKeyboard(true);
            break;
    }
    return true;
}

void edit_text::refresh()
{
    const esgui::color cline = m_focused ?
                               app::get().theme().focus : app::get().theme().disabled;

    float dpmm = app::get().screen_dpi() / 25.4;
    check_err();
    if (m_vbos.size() != 2) {
        m_vbos.resize(2);
        glGenBuffers(1, &m_vbos[0].id);
        glGenBuffers(1, &m_vbos[1].id);
    }

    using namespace esguid;
    esgui::rect r = rect();
    float th = 0.15 * dpmm;
    std::vector<VertexData> vbo1;
    PushRect(vbo1, r.x, r.y, r.w, r.h, m_color);
    PushRect(vbo1, r.x, r.y + r.h - th, r.w, th, cline);
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo1);

    std::vector<VertexData> vbo2;
    if (!m_text.empty())
        PushText(vbo2, r.x, r.y + dpmm, m_text, m_font, m_text_color);
    else
        PushText(vbo2, r.x, r.y + dpmm, m_hint, m_font, app::get().theme().disabled);
    m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo2);
    m_vbos[1].texture = m_font.texture();
}

size edit_text::min_size()
{
    float dpmm = app::get().screen_dpi() / 25.4;
    float h = esguid::MeasureText("Test", m_font).y;
    h += 2*dpmm;
    return { 20*dpmm, h };
}

}
