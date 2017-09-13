#include "popup.h"
#include "app.h"

namespace esgui {

popup::popup(container* c)
	: widget(c), m_highlighted(-1), m_sel()
{
	visible(false);
}

void popup::exec()
{
	size client = app::get().client_size();
	m_rect = { 0, 0, client.x, client.y };
	m_highlighted = -1;
	refresh();
	visible(true);
}

void popup::touch(action act, const point& p)
{
	float dh = menu_dh();
	esgui::rect r = menu_rect();
	int idx = int((p.y - r.y) / dh);
	switch (act) {
		case action::down:
			if (r.contains(p)) {
                m_highlighted = idx;
				refresh();
			}
			break;
		case action::up: {
            int last_highlighted = m_highlighted;
            m_highlighted = -1;
            refresh();
            if (r.contains(p) && idx == last_highlighted) {
                if (m_on_popup)
                    m_on_popup(idx);
                visible(false);
            }
            break;
        }
		case action::cancel:
			if (m_on_popup)
				m_on_popup(-1);
			visible(false);
			break;
	}	
}

float popup::menu_dh() const
{
	float dpmm = app::get().screen_dpi() / 25.4;
	return 11 * dpmm;
}

esgui::rect popup::menu_rect() const
{
	size client = app::get().client_size();
	float dpmm = app::get().screen_dpi() / 25.4;
	float w = 70 * dpmm; //7cm
	w = std::min(w, client.x - 10 * dpmm); //0.5cm margin on both sides
	float dh = menu_dh();
	float h = m_items.size() * dh;
	h = std::min(h, client.y - 8 * dpmm); //0.4cm margin both sides
	float mx = (client.x - w) / 2;
	float my = (client.y - h) / 2;
	return { mx, my, w, h };
}

void popup::refresh()
{
	const color cshadow(0.2, 0.2, 0.2, 0.8);
	const color cpanel(0.3, 0.3, 0.3);
	const color cselect(0.5, 0.5, 0.5);
    const color ctext(1, 1, 1);
    const color cradio1(0.7, 0.7, 0.7);
    const color cradio2(0.4, 0.8, 0.8);
	
	size client = app::get().client_size();
    /*if (!client.y)
        return;*/
	float dpmm = app::get().screen_dpi() / 25.4;
	float dh = menu_dh();
	esgui::rect r = menu_rect();
	
	if (m_vbos.size() != 2) {
		m_vbos.resize(3);
		glGenBuffers(1, &m_vbos[0].id);
		glGenBuffers(1, &m_vbos[1].id);
        glGenBuffers(1, &m_vbos[2].id);
    }

    //panels
	using namespace esguid;
	std::vector<VertexData> vbo1;
    //PushRect(vbo1, 0, 0, client.x, client.y, cshadow);
	PushRect(vbo1, 0, 0, client.x, r.y, cshadow);
	PushRect(vbo1, 0, r.y, r.x, r.h, cshadow);
	PushRect(vbo1, 0, client.y - r.y, client.x, r.y, cshadow);
	PushRect(vbo1, client.x - r.x, r.y, r.x, r.h, cshadow);
    PushRect(vbo1, r.x, r.y, r.w, r.h, cpanel);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0].id);
    glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
    m_vbos[0].size = vbo1.size();

    //selection + radios
    std::vector<VertexData> vbo2;
    if (m_highlighted >= 0) {
        PushRect(vbo2, r.x, r.y + dh * m_highlighted, r.w, dh, cselect);
    }
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        color cradio = i == m_sel ? cradio2 : cradio1;
        PushRadio(vbo2, i == m_sel, r.x + r.w - 3.5*dpmm, r.y + i*dh + dh/2, 1.5*dpmm, cradio);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1].id);
    glBufferData(GL_ARRAY_BUFFER, vbo2.size() * sizeof(VertexData), vbo2.data(), GL_STATIC_DRAW);
    m_vbos[1].size = vbo2.size();
    m_vbos[1].scissor = r;

    //labels
    std::vector<VertexData> vbo3;
	font font("normal", 13);
	float df = MeasureText("test", font).y;
    for (size_t i = 0; i < m_items.size(); ++i)
	{
        PushText(vbo3, r.x + 2*dpmm, r.y + i*dh + (dh-df)/2, m_items[i], font, ctext);
        //PushRect(vbo3, r.x + 2 * dpmm, r.y + i*dh + (dh - df) / 2, r.w, dh - (dh - df) / 2, "white");
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2].id);
	glBufferData(GL_ARRAY_BUFFER, vbo3.size() * sizeof(VertexData), vbo3.data(), GL_STATIC_DRAW);
	m_vbos[2].size = vbo3.size();
	m_vbos[2].texture = font.texture();
    m_vbos[2].scissor = r;
}

}