#include "widget.h"
#include "esgui.h"
#include <GLES2/gl2.h>

namespace esgui
{

widget::widget(container* cont)
	: m_visible(true), m_vbos(), m_vbo_sizes(), m_texture(0)
{
	cont->register_(this);
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