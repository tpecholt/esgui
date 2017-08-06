#include "esgui.h"
#include "impl.h"
#include "android.h"
#include <algorithm>
#include <exception>

namespace esgui
{

static int create_program(const char* vertexSource, const char* fragmentSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		std::string err(256, ' ');
		glGetShaderInfoLog(vertexShader, err.size(), NULL, &err[0]);
		LOGE("vertex shader error");
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		std::string err(256, ' ');
		glGetShaderInfoLog(fragmentShader, err.size(), NULL, &err[0]);
		LOGE("fragment shader error");
	}

	int prog = glCreateProgram();
	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fragmentShader);
	glLinkProgram(prog);
	return prog;
}

matrix4 ortho2d(float w, float h)
{
	/*const float zNear = -1.0f;
	const float zFar = 1.0f;
	const float inv_z = 1.0f / (zFar - zNear);
	const float inv_y = 1.0f / (top - bottom);
	const float inv_x = 1.0f / (right - left);
	
	return matrix4{ {
			2*inv_x, 0, 0, 0, 
			0, 2*inv_y, 0, 0, 
			0, 0, -2*inv_z, 0,  
			-(right + left)*inv_x, -(top + bottom)*inv_y, -(zFar + zNear)*inv_z, 1
	} };*/
	return matrix4{ {
			2/w, 0, 0, 0,
			0, -2/h, 0, 0,
			0, 0, 1, 0,
			-1, 1, 0, 1
	} };
}

//----------------------------------------------------------------------------------

app& app::get()
{
	static app mgr;
	return mgr;
}

app::app()
	: m_dpi()
{
	m_mvp = ortho2d(100, 100);
}

void app::init_rendering()
{
	const char* vertexSource =
		"uniform mat4 mvp; "
		"attribute vec2 pos; "
		"attribute vec2 coords; "
		"attribute vec4 color; "
		"varying vec4 outColor; "
		"varying vec2 outCoords; "
		"void main() "
		"{ "
		"	gl_Position = mvp * vec4(pos, 0, 1.0); "
		"	outColor = color; "
		"	outCoords = coords; "
		"} ";

	const char* fragmentSource1 =
		"precision mediump float; "
		"varying vec4 outColor; "
		"varying vec2 outCoords; "
		"void main() "
		"{ "
		"   gl_FragColor = outColor; "
		"}";
	
	const char* fragmentSource2 =
		"precision mediump float; "
		"varying vec4 outColor; "
		"varying vec2 outCoords; "
		"uniform sampler2D sampler; "
		"void main() "
		"{ "
		"   gl_FragColor = texture2D(sampler, outCoords) * outColor; "
		"}";

	m_programs.push_back(create_program(vertexSource, fragmentSource1));
	m_programs.push_back(create_program(vertexSource, fragmentSource2));
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void app::register_(widget* w)
{
	m_widgets.push_back(w);
}

void app::unregister(widget* w)
{
	auto it = std::find(m_widgets.begin(), m_widgets.end(), w);
	if (it == m_widgets.end())
		return;
	m_widgets.erase(it);
}

void app::client_size(size s)
{
	m_client_size = s;
	m_mvp = ortho2d(s.x, s.y);
}

size app::screen_size()
{
	if (!m_screen_size.x) {
		auto dim = android::GetScreenSize();
		m_screen_size = { dim.first, dim.second };		
	}
	return m_screen_size;
}

int app::screen_dpi()
{
	if (!m_dpi)
		m_dpi = android::GetDPI();
	return m_dpi;
}

int app::font_texture(const std::string& face, int style)
{
	auto it = std::find_if(m_fonts.begin(), m_fonts.end(), [&](const decltype(m_fonts)::value_type& data) {
		return data.second.face == face && data.second.style == style;
	});
	if (it != m_fonts.end())
		return it->first;	
	FontData font;
	bool bold = style & font::bold;
	bool italic = style & font::italic;
	int texture = android::CreateFontAtlas(face.c_str(), bold, italic, 
		font.metrics.char_widths.data(), &font.metrics.ascent, &font.metrics.descent);	
	m_fonts[texture] = font;
	return texture;
}

const font_metrics& app::font_metrics(font f)
{
	static esgui::font_metrics tmp;
	auto it = m_fonts.find(f.texture());
	if (it == m_fonts.end())
		return tmp;
	return it->second.metrics;
}

void app::render()
{
	if (m_programs.empty())
		init_rendering();
	
	for (int prg : m_programs) {
		glUseProgram(prg);
		glUniformMatrix4fv(0, 1, false, m_mvp.data());
	}
	for (widget* w : m_widgets) {
		w->render(m_programs.data());
	}
}

}
