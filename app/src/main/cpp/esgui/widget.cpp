#include "widget.h"
#include "app.h"
#include <GLES2/gl2.h>

namespace esgui
{

widget::widget(container* cont)
{
	cont->register_(this);
}

widget::~widget()
{
    for (auto& vbo : m_vbos)
        glDeleteBuffers(1, &vbo.id);
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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < m_vbos.size(); ++i)
	{
        const auto& vbo = m_vbos[i];
		if (!vbo.size)
			continue;
        if (!vbo.scissor.invalid()) {
            glEnable(GL_SCISSOR_TEST);
            const esgui::rect& s = vbo.scissor;
            glScissor(s.x, s.y, s.w, s.h);
        }
        int prog = programs[vbo.texture ? 1 : 0];
		glUseProgram(prog);
		int loc = glGetUniformLocation(prog, "mvp");
        glUniformMatrix4fv(loc, 1, false, mvp);
        loc = glGetUniformLocation(prog, "scroll");
        glUniform2f(loc, vbo.scroll.x, vbo.scroll.y);
		glBindTexture(GL_TEXTURE_2D, vbo.texture);

        //always use glGetAttribLocation as locations might be different on different platforms
		glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
		int id = glGetAttribLocation(prog, "pos");
        glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), 0);
        id = glGetAttribLocation(prog, "coords");
        if (id >= 0) {
            glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void *) (2 * sizeof(float)));
        }
        id = glGetAttribLocation(prog, "color");
        glVertexAttribPointer(id, 4, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(4 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, vbo.size);
		glDisable(GL_SCISSOR_TEST);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	check_err();
}

}