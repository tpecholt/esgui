#include "widget.h"
#include "app.h"
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

void widget::render(const int programs[], const float mvp[])
{
	if (!visible())
		return;

	//glEnable(GL_SCISSOR_TEST);
	//size client = app::get().client_size();
	//glScissor(m_rect.x, client.y - m_rect.y - m_rect.h, m_rect.w, m_rect.h);

	check_err();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	check_err();

	for (int i = 0; i < m_vbos.size(); ++i)
	{
		if (!m_vbos[i].size)
			continue;
        int prog = programs[m_vbos[i].texture ? 1 : 0];
		glUseProgram(prog);
		check_err();
        int loc = glGetUniformLocation(prog, "mvp");
        glUniformMatrix4fv(loc, 1, false, mvp);
		check_err();
		glBindTexture(GL_TEXTURE_2D, m_vbos[i].texture);
		check_err();

        //always use glGetAttribLocation as locations might be different on different platforms
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[i].id);
		check_err();
        int id = glGetAttribLocation(prog, "pos");
        glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), 0);
        check_err();
        id = glGetAttribLocation(prog, "coords");
        if (id >= 0) {
            glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void *) (2 * sizeof(float)));
            check_err();
        }
        id = glGetAttribLocation(prog, "color");
        glVertexAttribPointer(id, 4, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(4 * sizeof(float)));
		check_err();

		glDrawArrays(GL_TRIANGLES, 0, m_vbos[i].size);
		check_err();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glDisable(GL_SCISSOR_TEST);
	check_err();
}

}