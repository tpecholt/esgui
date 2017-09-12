#include "popup.h"
#include "app.h"

namespace esgui {

popup::popup(container* c)
	: widget(c)
{
	visible(false);
}

void popup::exec()
{
	size client = app::get().client_size();
	m_rect = { 0, 0, client.x, client.y };
	m_sel = -1;
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
				m_sel = idx;
				refresh();
			}
			break;
		case action::up: {
            int last_sel = m_sel;
            m_sel = -1;
            refresh();
            if (r.contains(p) && idx == last_sel) {
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
	return 12 * dpmm;
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
	
	size client = app::get().client_size();
    /*if (!client.y)
        return;*/
	float dpmm = app::get().screen_dpi() / 25.4;
	float dh = menu_dh();
	esgui::rect r = menu_rect();
	
	if (m_vbos.size() != 2) {
		m_vbos.resize(2);
		glGenBuffers(1, &m_vbos[0].id);
		glGenBuffers(1, &m_vbos[1].id);		
	}
	
	using namespace esguid;
	std::vector<VertexData> vbo1;
    //PushRect(vbo1, 0, 0, client.x, client.y, cshadow);
	PushRect(vbo1, 0, 0, client.x, r.y, cshadow);
	PushRect(vbo1, 0, r.y, r.x, r.h, cshadow);
	PushRect(vbo1, 0, client.y - r.y, client.x, r.y, cshadow);
	PushRect(vbo1, client.x - r.x, r.y, r.x, r.h, cshadow);
    PushRect(vbo1, r.x, r.y, r.w, r.h, cpanel);
	if (m_sel >= 0) {
		//PushRect(vbo1, r.x, r.y, r.w, dh * m_sel, cpanel);
		PushRect(vbo1, r.x, r.y + dh * m_sel, r.w, dh, cselect);
        //PushRect(vbo1, r.x, r.y + dh * (m_sel + 1), r.w, dh * (m_items.size() - m_sel - 1), cpanel);
	}
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0].id);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
	m_vbos[0].size = vbo1.size();

	std::vector<VertexData> vbo2;
	font font("normal", 14);
	float df = MeasureText("test", font).y;
    for (size_t i = 0; i < m_items.size(); ++i)
	{
        PushText(vbo2, r.x + 2*dpmm, r.y + i*dh + (dh-df)/2, m_items[i], font, ctext);
		//PushRect(vbo2, r.x + 2 * dpmm, r.y + i*dh + (dh - df) / 2, r.w, dh - (dh - df) / 2, "white");
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1].id);
	glBufferData(GL_ARRAY_BUFFER, vbo2.size() * sizeof(VertexData), vbo2.data(), GL_STATIC_DRAW);
	m_vbos[1].size = vbo2.size();
	m_vbos[1].texture = font.texture();
}

}