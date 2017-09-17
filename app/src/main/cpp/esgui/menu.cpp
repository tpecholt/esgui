#include "menu.h"
#include "app.h"
#include <cmath>

namespace esgui {

menu::menu(container* c, int id)
	: widget(c, id), m_highlighted(-1)
{
	visible(false);
}

void menu::exec(const point& p)
{
    float dpmm = app::get().screen_dpi() / 25.4;
    size client = app::get().client_size();
    m_rect = { 0, 0, client.x, client.y };
    size siz = menu_size();
    point pos = p;
    pos.x = std::min(pos.x, client.x - siz.x - 1*dpmm);
    pos.y = std::min(pos.y, client.y - siz.y - 1*dpmm);
	m_rect = { pos.x, p.y, siz.x, siz.y };
	m_highlighted = -1;
    refresh();
	visible(true);
    app::get().overlay(this);
}

bool menu::touch(action act, const point& p)
{
    if (!visible())
        return false;
    float dh = menu_dh();
	esgui::rect r = m_rect;
	int idx = int((p.y - r.y) / dh);
	switch (act) {
		case action::down:
			if (r.contains(p)) {
                m_last_p = p;
                m_moving = false;
                m_highlighted = idx;
                refresh();
			}
			break;
        case action ::move:
            if (!m_moving) {
                if (std::abs(p.y - m_last_p.y) < 10)
                    break;
                m_highlighted = -1;
                refresh();
            }
            m_moving = true;
            break;
		case action::up: {
            if (!m_moving && r.contains(p)) {
                if (m_on_menu)
                    m_on_menu(idx);
            }
            visible(false);
            app::get().overlay(nullptr);
            break;
        }
		case action::cancel:
			if (m_on_menu)
				m_on_menu(-1);
			visible(false);
            app::get().overlay(nullptr);
			break;
	}
    return true;
}

const esgui::font& menu::font() const
{
    return app::get().default_font();
}

float menu::menu_dh() const
{
	float dpmm = app::get().screen_dpi() / 25.4;
	return 9 * dpmm;
}

size menu::menu_size()
{
    float dpmm = app::get().screen_dpi() / 25.4;
    float w = 0;
    for (const auto& it : m_items)
        w = std::max(w, esguid::MeasureText(it, font()).x);
    w += 6*dpmm;
    float h = m_items.size() * menu_dh();
    return {w, h};
}

void menu::refresh()
{
	const esgui::color cpanel = app::get().theme().background;
    const esgui::color cselect = cpanel.lighter();
    const esgui::color ctext = app::get().theme().text;

	float dpmm = app::get().screen_dpi() / 25.4;
	float dh = menu_dh();
	esgui::rect r = m_rect;
	
	if (m_vbos.size() != 2) {
		m_vbos.resize(2);
		glGenBuffers(1, &m_vbos[0].id);
        glGenBuffers(1, &m_vbos[1].id);
    }

    //panels
	using namespace esguid;
	std::vector<VertexData> vbo1;
    float ro = 1*dpmm;
    float th = 0.15*dpmm;
    PushRoundRect(vbo1, r.x, r.y, r.w, r.h, ro, ctext);
    if (m_highlighted >= 0) {
        PushRect(vbo1, r.x + th, r.y + th + dh * m_highlighted, r.w - 2*th, dh - 2*th, cselect);
    }
    PushRoundRect(vbo1, r.x + th, r.y + th, r.w - 2*th, r.h - 2*th, ro - th, cpanel);
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo1);

    //labels
    std::vector<VertexData> vbo2;
	esgui::font f = font();
	float df = MeasureText("test", f).y;
    for (size_t i = 0; i < m_items.size(); ++i)
	{
        PushText(vbo2, r.x + 3*dpmm, r.y + i*dh + (dh-df)/2, m_items[i], f, ctext);
	}
	m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo2);
	m_vbos[1].texture = f.texture();
}

}