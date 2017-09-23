//
// Created by tomas on 17.09.2017.
//

#include "edit_text.h"
#include "app.h"
#include "impl.h"
#include "android.h"

namespace esgui {

edit_text::edit_text(container* cont, int id)
        : widget(cont, id)
{
    m_input_type = all;
    m_style = normal;
    m_font = app::get().default_font();
    m_color = color::transparent;
    m_text_color = app::get().theme().edit_text;
}

void edit_text::style(enum style s)
{
    m_style = s;
    refresh();
}

void edit_text::input_type(enum input_type s)
{
    m_input_type = s;
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

void edit_text::touch(action act, const point& p) {
    esgui::rect r = m_rect;
    if (act == action::up) {
        if (m_rect.contains(p)) {
            m_sel = esguid::TextPos(m_text, m_font, p.x - r.x);
            m_tp = app::get().now();
            app::get().focus(this);
            refresh();
        } else {
            app::get().focus(nullptr);
            refresh();
        }
    }
}

void edit_text::press(int key)
{
    if (key == '\x8' && m_sel) {
        --m_sel;
        m_text.erase(m_text.begin() + m_sel);
        refresh();
    }
    else if (key >= '0' && key <= '9') {
        m_text.insert(m_text.begin() + m_sel, key);
        ++m_sel;
        refresh();
    }
    else if (m_input_type == all && key >= 32 && key <= 127) {
        m_text.insert(m_text.begin() + m_sel, key);
        ++m_sel;
        refresh();
    }
}

void edit_text::refresh()
{
    bool focused = app::get().focus() == this;
    const esgui::color cline = focused ?
                               app::get().theme().focus : app::get().theme().disabled;

    float dpmm = app::get().screen_dpi() / 25.4;
    check_err();
    if (m_vbos.size() != 3) {
        m_vbos.resize(3);
        glGenBuffers(1, &m_vbos[0].id);
        glGenBuffers(1, &m_vbos[1].id);
        glGenBuffers(1, &m_vbos[2].id);
    }

    using namespace esguid;
    esgui::rect r = rect();
    float th = (focused ? 0.25 : 0.15) * dpmm;
    std::vector<VertexData> vbo1;
    PushRect(vbo1, r.x, r.y, r.w, r.h, m_color);
    PushRect(vbo1, r.x, r.y + r.h - th, r.w, th, cline);
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo1);

    std::vector<VertexData> vbo2;
    if (focused) {
        size s = esguid::MeasureText(m_text.substr(0, m_sel), m_font);
        PushRect(vbo2, r.x + s.x, r.y + dpmm, 0.3*dpmm, r.h - 2*dpmm, cline);
    }
    m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo2);
    //m_vbos[1].scissor = m_rect;

    std::vector<VertexData> vbo3;
    if (!m_text.empty()) {
        PushText(vbo3, r.x, r.y + dpmm, m_text, m_font, m_text_color);
    }
    else {
        PushText(vbo3, r.x, r.y + dpmm, m_hint, m_font, app::get().theme().disabled);
    }
    m_vbos[2].size = SendBuffer(m_vbos[2].id, vbo3);
    m_vbos[2].texture = m_font.texture();
}

size edit_text::min_size()
{
    float dpmm = app::get().screen_dpi() / 25.4;
    float h = esguid::MeasureText("Test", m_font).y;
    h += 2*dpmm;
    return { 20*dpmm, h };
}

void edit_text::animate(std::chrono::system_clock::time_point tp)
{
    bool focused = app::get().focus() == this;
    if (!focused)
        return;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp - m_tp).count();
    bool show = (ms % 1000) < 500;
    m_vbos[1].scroll.y = show ? 0 : m_rect.y;
}

}
