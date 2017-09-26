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
    m_color = color::transparent;
    m_text_color = color::transparent;
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

esgui::color edit_text::text_color() const
{
    if (m_text_color != color::transparent)
        return m_text_color;
    if (m_style == normal)
        return app::get().theme().edit_text;
    else
        return app::get().theme().button_text;
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

esgui::font edit_text::font() const
{
    if (!m_font.invalid())
        return m_font;
    if (m_style == normal)
        return app::get().theme().ftext;
    else
        return app::get().theme().fsmall_text;
}

void edit_text::touch(action act, const point& p) {
    esgui::rect r = rect();
    float dpmm = app::get().screen_dpi() / 25.4;
    if (act == action::up) {
        if (m_rect.contains(p)) {
            if (m_style == search && !m_text.empty() && p.x >= r.x + r.w - r.h) {
                m_sel = 0;
                m_text.clear();
            }
            else {
                float margin = m_style == search ? 1.5*dpmm : 0;
                m_sel = esguid::TextPos(m_text, font(), p.x - r.x - margin);
            }
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
    const auto& theme = app::get().theme();
    const esgui::color cline = focused ? theme.focus : theme.disabled;
    const esgui::color ctext = m_text.empty() ? theme.disabled : text_color();
    const esgui::font fo = font();

    float dpmm = app::get().screen_dpi() / 25.4;
    check_err();
    if (m_vbos.size() != 5) {
        m_vbos.resize(5);
        glGenBuffers(1, &m_vbos[0].id);
        glGenBuffers(1, &m_vbos[1].id);
        glGenBuffers(1, &m_vbos[2].id);
        glGenBuffers(1, &m_vbos[3].id);
        glGenBuffers(1, &m_vbos[4].id);
    }

    using namespace esguid;
    esgui::rect r = rect();
    float th = (focused ? 0.25 : 0.15) * dpmm;
    float rd = 0.8 * dpmm;
    float margin = 0;
    if (m_style == search)
        margin = m_text.empty() ? dpmm : 1.5*dpmm;
    //frame
    std::vector<VertexData> vbo;
    if (m_style == normal) {
        PushRect(vbo, r.x, r.y, r.w, r.h, m_color);
        PushRect(vbo, r.x, r.y + r.h - th, r.w, th, cline);
    }
    else if (m_style == search) {
        PushRoundRect(vbo, r.x, r.y, r.w, r.h, rd, "white");
    }
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo);

    //cursor
    vbo.clear();
    if (focused) {
        size s = esguid::MeasureText(m_text.substr(0, m_sel), fo);
        PushRect(vbo, r.x + margin + s.x, r.y + dpmm, 0.3*dpmm, r.h - 2*dpmm, cline);
    }
    m_vbos[3].size = SendBuffer(m_vbos[3].id, vbo);
    //m_vbos[1].scissor = m_rect;

    //icons
    m_vbos[1].size = m_vbos[2].size = 0;
    if (m_style == search) {
        esgui::size siz;
        if (m_text.empty()) {
            vbo.clear();
            m_vbos[1].texture = app::get().icon_texture("@drawable/ic_search", siz);
            siz *= 0.8;
            PushRect(vbo, r.x + dpmm, r.y + (r.h - siz.y) / 2, siz.x, siz.y, "white");
            m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo);
            margin += siz.x;
        }
        else {
            vbo.clear();
            m_vbos[2].texture = app::get().icon_texture("@drawable/ic_clear", siz);
            siz *= 0.8;
            PushRect(vbo, r.x + r.w - rd - siz.x, r.y + (r.h - siz.y) / 2, siz.x, siz.y, "white");
            m_vbos[2].size = SendBuffer(m_vbos[2].id, vbo);
        }
    }

    //text
    vbo.clear();
    float marginy = m_style == search ? 1.1*dpmm : dpmm;
    if (!m_text.empty()) {
        PushText(vbo, r.x + margin, r.y + marginy, m_text, fo, ctext);
    }
    else {
        PushText(vbo, r.x + margin, r.y + marginy, m_hint, fo, ctext);
    }
    m_vbos[4].size = SendBuffer(m_vbos[4].id, vbo);
    m_vbos[4].texture = fo.texture();

}

size edit_text::min_size()
{
    float dpmm = app::get().screen_dpi() / 25.4;
    float h = esguid::MeasureText("Test", app::get().theme().ftext).y;
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
    m_vbos[3].scroll.y = show ? 0 : m_rect.y;
}

}
