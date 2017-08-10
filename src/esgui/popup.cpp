#include "popup.h"
#include "esgui.h"

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
	if (!r.contains(p))
		return;
	int idx = int((p.y - r.y) / dh);
	switch (act) {
	case action::down:
		m_sel = idx;
		refresh();
		break;
	case action::up:
		m_sel = -1;
		refresh();
		if (m_on_popup)
			m_on_popup(idx);
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

void popup::do_refresh()
{
	const color cshadow(0.2, 0.2, 0.2, 0.8);
	const color cpanel(0.3, 0.3, 0.3);
	const color cselect(0.5, 0.5, 0.5);
	
	size client = app::get().client_size();
	float dpmm = app::get().screen_dpi() / 25.4;
	float dh = menu_dh();
	esgui::rect r = menu_rect();
	
	using namespace esguid;
	std::vector<VertexData> vbo1;
	PushRect(vbo1, 0, 0, client.x, r.y, cshadow);
	PushRect(vbo1, 0, r.y, r.x, r.h, cshadow);
	PushRect(vbo1, 0, client.y - r.y, client.x, r.y, cshadow);
	PushRect(vbo1, client.x - r.x, r.y, r.x, r.h, cshadow);
	/*if (m_sel >= 0) {
		PushRect(vbo1, r.x, r.y, r.w, dh * m_sel, cpanel);
		PushRect(vbo1, r.x, r.y + dh * m_sel, r.w, dh, cselect);
	}*/
	PushRect(vbo1, r.x, r.y, r.w, r.h, cpanel);
	if (m_sel >= 0)
		PushRect(vbo1, r.x, r.y + dh * (m_sel + 1), r.w, (m_items.size() - m_sel - 1) * dh, cpanel);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
	m_vbo_sizes[0] = vbo1.size();

	std::vector<VertexData> vbo;
	font font("normal", 18);
	float df = MeasureText("test", font).y;
	for (size_t i = 0; i < m_items.size(); ++i)
	{
		PushText(vbo, r.x + 2*dpmm, r.y + i*dh + (dh-df)/2, m_items[i], font, "white");
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(VertexData), vbo.data(), GL_STATIC_DRAW);
	m_vbo_sizes[1] = vbo.size();
	m_texture = font.texture();
}

}