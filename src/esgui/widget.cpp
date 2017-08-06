#include "widget.h"
#include "esgui.h"
#include <GLES2/gl2.h>

namespace esgui
{

font::font()
	: m_texture(), m_point_size()
{}

font::font(int m_texture, int pointSize)
	: m_texture(m_texture), m_point_size(pointSize)
{}

font::font(const std::string& face, int pointSize, int style)
	: m_point_size(pointSize)
{
	m_texture = app::get().font_texture(face, style);
}

//-----------------------------------------------------------------------

color::color(const char* name)
{
	static std::map<std::string, color> colors {
		{ "black", color(0, 0, 0) },
		{ "white", color(1, 1, 1) },
		{ "grey", color(0.7, 0.7, 0.7) },
		{ "red", color(1, 0, 0) },
		{ "green", color(0, 1, 0) },
		{ "blue", color(0, 0, 1) },
	};
	*this = colors[name];
}

//-----------------------------------------------------------------------

widget::widget()
	: m_visible(true), m_vbos(), m_vbo_sizes(), m_texture(0)
{
	app::get().register_(this);
}

widget::~widget()
{
	app::get().unregister(this);
	glDeleteBuffers(2, m_vbos);
}

void widget::rect(const esgui::rect& r)
{
	if (m_rect == r) //layout optimization
		return;
	m_rect = r;
	refresh();
}

void widget::refresh()
{
	if (!m_vbos[0])
		glGenBuffers(2, m_vbos);
	do_refresh();
}

void widget::render(const int programs[])
{
	if (!visible())
		return;

	//glEnable(GL_SCISSOR_TEST);
	size client = app::get().client_size();
	glScissor(m_rect.x, client.y - m_rect.y - m_rect.h, m_rect.w, m_rect.h);

	glBindTexture(GL_TEXTURE_2D, m_texture);	
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < 2; ++i)
	{
		if (!m_vbo_sizes[i])
			continue;
		glUseProgram(programs[i]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[i]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(2 * sizeof(float)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(4 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, m_vbo_sizes[i]);		
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisable(GL_SCISSOR_TEST);
}

}