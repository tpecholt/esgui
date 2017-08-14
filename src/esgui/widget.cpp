#include "widget.h"
#include "esgui.h"
#include <GLES2/gl2.h>

namespace esgui
{

widget::widget(container* cont)
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

void widget::render(const int programs[])
{
	if (!visible())
		return;

	//glEnable(GL_SCISSOR_TEST);
	//size client = app::get().client_size();
	//glScissor(m_rect.x, client.y - m_rect.y - m_rect.h, m_rect.w, m_rect.h);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < m_vbos.size(); ++i)
	{
		if (!m_vbos[i].size)
			continue;
        int prog = programs[m_vbos[i].texture ? 1 : 0];
		glUseProgram(prog);
		glBindTexture(GL_TEXTURE_2D, m_vbos[i].texture);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[i].id);
        int id = glGetAttribLocation(prog, "pos");
        glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), 0);
        id = glGetAttribLocation(prog, "coords");
        glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(2 * sizeof(float)));
        id = glGetAttribLocation(prog, "color");
        glVertexAttribPointer(id, 4, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(4 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, m_vbos[i].size);		
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisable(GL_SCISSOR_TEST);
}

}