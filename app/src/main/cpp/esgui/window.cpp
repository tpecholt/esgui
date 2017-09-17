#include "window.h"
#include "app.h"

namespace esgui
{

font::font()
	: m_texture(), m_style(), m_point_size()
{}

font::font(int m_texture, int pointSize)
	: m_texture(m_texture), m_point_size(pointSize), m_face(), m_style()
{}

font::font(const std::string& face, int pointSize, int style)
	: m_face(face), m_point_size(pointSize), m_style(style)
{
	m_texture = app::get().font_texture(face, style);
}

font font::make_bold() const
{
    return font(m_face, m_point_size, m_style | bold);
}

//-----------------------------------------------------------------------

const color color::transparent(0, 0, 0, 0);

color::color(const char* name)
{
	static std::map<std::string, color> colors{
		{ "black", color(0, 0, 0) },
		{ "white", color(1, 1, 1) },
		{ "grey", color(0.7, 0.7, 0.7) },
		{ "dark grey", color(0.4, 0.4, 0.4) },
		{ "light grey", color(0.8, 0.8, 0.8) },
		{ "red", color(1, 0, 0) },
		{ "green", color(0, 1, 0) },
		{ "blue", color(0, 0, 1) },
		{ "yellow", color(1, 1, 0) },
		{ "orange", color(1, 0.5, 0)},
		{ "brown", color(1, 0.5, 0.25)}
	};
	*this = colors[name];
}

//----------------------------------------------------------------------

void window::render(const std::vector<int>& programs)
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
		/*int loc = glGetUniformLocation(prog, "mvp");
        glUniformMatrix4fv(loc, 1, false, mvp);*/
		int loc = glGetUniformLocation(prog, "scroll");
		glUniform2f(loc, vbo.scroll.x, vbo.scroll.y);
		glBindTexture(GL_TEXTURE_2D, vbo.texture);

		//always use glGetAttribLocation as locations might be different on different platforms
		glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
		loc = glGetAttribLocation(prog, "pos");
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), 0);
		loc = glGetAttribLocation(prog, "coords");
		if (loc >= 0) {
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void *) (2 * sizeof(float)));
		}
		loc = glGetAttribLocation(prog, "color");
		glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(esguid::VertexData), (void*)(4 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, vbo.size);
		glDisable(GL_SCISSOR_TEST);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	check_err();
}


}