#include "container.h"
#include "app.h"
#include <cmath>

namespace esgui {

container::container(container* parent, int id)
	: window(id), m_layout()
{
	m_layout = new row_layout({});
    m_color = app::get().theme().background;
	if (parent)
		parent->register_(this);
	else
		esgui::app::get().register_(this);
}

container::~container()
{
	delete m_layout;
	for (window* w : m_children)
		delete w;
}

void container::register_(window* w)
{
	m_children.push_back(w);
}

void container::layout(esgui::layout* l)
{
	delete m_layout;
	m_layout = l;
	refresh();
}

window* container::find_child(int id)
{
	for (window* w : m_children)
		if (w->id() == id)
			return w;
	return nullptr;
}

void container::delete_children()
{
    for (window* w : m_children)
        delete w;
    m_children.clear();
}

void container::rect(const esgui::rect& r)
{
	m_layout->rect(r);
    refresh();
}

void container::color(const esgui::color& c)
{
    m_color = c;
    refresh();
}

void container::ensure_visible(window *w)
{
    esgui::rect r = rect();
    float kbd_h = app::get().kbd_height();
    point client_size = app::get().client_size();
    float dpmm = app::get().screen_dpi() / 25.4;
    float clip_y = client_size.y - kbd_h;
    if (w) {
        float margin = 3*dpmm;
        esgui::rect wr = w->rect();
        if (wr.y + m_scroll.y - margin < r.y)
            m_scroll.y = r.y + margin - wr.y;
        if (wr.y + wr.h + m_scroll.y + margin > clip_y)
            m_scroll.y = clip_y - wr.y - wr.h - margin;
    }
    float clip_h = clip_y - r.y;
    m_scroll.y = std::max(m_scroll.y, clip_h - r.h);
    m_scroll.y = std::min(m_scroll.y, 0.f);
    refresh(); //to update slider
}

void container::refresh()
{
    const esgui::color cslider(0.4, 0.4, 0.4);

    float kbd_h = app::get().kbd_height();
    point client_size = app::get().client_size();
    float dpmm = app::get().screen_dpi() / 25.4;
    if (m_vbos.size() != 2) {
        m_vbos.resize(2);
        glGenBuffers(1, &m_vbos[0].id);
        glGenBuffers(1, &m_vbos[1].id);
    }
    const auto &r = rect();
    using namespace esguid;
    //background color
    std::vector<VertexData> vbo;
    PushRect(vbo, r.x, r.y, r.w, r.h, m_color);
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo);

    //
    vbo.clear();
    const float th = 0.5 * dpmm;
    float clip_h = client_size.y - kbd_h - r.y;
    m_vbos[1].size = 0;
    if (r.h > clip_h) {
        float h = clip_h * clip_h / r.h;
        PushRect(vbo, r.x + r.w - th, r.y, th, h, cslider);
        m_vbos[1].size = SendBuffer(m_vbos[1].id, vbo);
        //m_vbos[1].scissor = r;
        float dh = -m_scroll.y * clip_h / r.h;
        m_vbos[1].scroll = {0, dh};
        LOGE("slider.h = %f dh = %f scrolly = %f", h, dh, m_scroll.y);
    } else
        LOGE("slider.h = 0");


    m_layout->refresh();
}

void container::render(const std::vector<int>& programs, const point& scroll)
{
    if (!visible())
        return;

    window::render(programs, scroll);

    for (window* w : m_children)
        w->render(programs, scroll + m_scroll);
}

void container::touch(action act, const point& pp)
{
    //intrecept scrolling events
    point p = pp;
    switch (act) {
        case action::down:
            m_last_p = p;
            m_moving = false;
            break;
        case action::move: {
            if (!m_moving && std::abs(p.y - m_last_p.y) < 10)
                break;
            m_moving = true;
            m_scroll.y += p.y - m_last_p.y;
            m_last_p = p;
            esgui::rect r = rect();
            float kbd_h = app::get().kbd_height();
            esgui::point client_size = app::get().client_size();
            float clip_h = client_size.y - kbd_h - r.y;
            m_scroll.y = std::max(m_scroll.y, clip_h - r.h);
            m_scroll.y = std::min(m_scroll.y, 0.f);
            float dh = -m_scroll.y * clip_h / r.h;
            m_vbos[1].scroll = esgui::point{0, dh};
            return;
        }
        case action::up:
            if (m_moving)
                return;
            break;
    }
    p.x -= m_scroll.x;
    p.y -= m_scroll.y;

    for (window *w : m_children) {
        if (w->visible() && w->rect().contains(p)) {
            w->touch(act, p);
            break;
        }
    }
}

}