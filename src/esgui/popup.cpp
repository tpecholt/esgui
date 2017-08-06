#include "popup.h"
#include "esgui.h"

namespace esgui {

void popup::do_refresh()
{
	const color cshadow(0.2, 0.2, 0.2, 0.8);
	const color cpanel(0.3, 0.3, 0.3);
	
	size client = app::get().client_size();
	m_rect = { 0, 0, client.x, client.y };
	float dpmm = app::get().screen_dpi() / 25.4;
	float w = 70 * dpmm; //7cm
	w = std::min(w, client.x - 10 * dpmm); //0.5cm margin on both sides
	const float dh = 12 * dpmm;
	float h = m_items.size() * dh;
	h = std::min(h, client.y - 8 * dpmm);
	
	using namespace esguid;
	std::vector<VertexData> vbo1;
	float mx = (client.x - w) / 2;
	float my = (client.y - h) / 2;
	PushRect(vbo1, 0, 0, client.x, my, cshadow);
	PushRect(vbo1, 0, my, mx, h, cshadow);
	PushRect(vbo1, 0, client.y - my, client.x, my, cshadow);
	PushRect(vbo1, client.x - mx, my, mx, h, cshadow);
	PushRect(vbo1, mx, my, w, h, cpanel);	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vbo1.size() * sizeof(VertexData), vbo1.data(), GL_STATIC_DRAW);
	m_vbo_sizes[0] = vbo1.size();

	std::vector<VertexData> vbo;
	font font("normal", 18);
	float df = MeasureText("test", font).y;
	for (size_t i = 0; i < m_items.size(); ++i)
	{
		PushText(vbo, mx + 2*dpmm, my + i*dh + (dh-df)/2, m_items[i], font, "white");
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(VertexData), vbo.data(), GL_STATIC_DRAW);
	m_vbo_sizes[1] = vbo.size();
	m_texture = font.texture();
}

}