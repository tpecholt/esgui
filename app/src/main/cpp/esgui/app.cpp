#include "app.h"
#include "impl.h"
#include "android.h"
#include <algorithm>
#include <exception>
#include <chrono>

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
		LOGE("vertex shader error: %s", err.c_str());
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		std::string err(256, ' ');
		glGetShaderInfoLog(fragmentShader, err.size(), NULL, &err[0]);
		LOGE("fragment shader error: %s", err.c_str());
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
	: m_app_bar(), m_overlay(), m_focus()
{
    m_dpi = 0;
    m_status_bar_height = 0;
    m_mvp = ortho2d(100, 100);
    m_theme = get_theme("");
}

void app::init_rendering()
{
    for (int prg : m_programs)
        glDeleteProgram(prg);
    m_programs.clear();

    const char* vertexSource =
        "uniform mat4 mvp; "
	    "uniform vec2 scroll; "
        "attribute vec2 pos; "
        "attribute vec2 coords; "
        "attribute vec4 color; "
        "varying vec4 outColor; "
        "varying vec2 outCoords; "
        "void main() "
        "{ "
        "	gl_Position = mvp * vec4(pos + scroll, 0, 1.0); "
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

    const char* fragmentSource3 =
            "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float; "
        "varying vec4 outColor; "
        "varying vec2 outCoords; "
        "uniform samplerExternalOES sampler; "
        "void main() "
        "{ "
        "   gl_FragColor = texture2D(sampler, outCoords) * outColor; "
        "}";

    m_programs.push_back(create_program(vertexSource, fragmentSource1));
	m_programs.push_back(create_program(vertexSource, fragmentSource2));
    m_programs.push_back(create_program(vertexSource, fragmentSource3));
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check_err();
}

void app::register_(container* c)
{
    if (c->id() == app_bar::sid)
        m_app_bar = (app_bar*)c;
    else
        m_containers.push_back(c);
}

void app::clear_resources()
{
    m_icons.clear();
    m_fonts.clear();
}

void app::client_size(size s)
{
    m_client_size = s;
	m_mvp = ortho2d(s.x, s.y);
    rect r{ 0, 0, client_size().x, client_size().y };
    if (m_app_bar) {
        size siz = m_app_bar->min_size();
        m_app_bar->rect({0, 0, siz.x, siz.y});
        r.y += m_app_bar->rect().h;
        r.h -= m_app_bar->rect().h;
    }
    for (container* c : m_containers)
		c->rect(r);
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

void app::theme(const esgui::theme& th)
{
    m_theme = th;
    if (m_app_bar)
        m_app_bar->refresh();
    for (container* c : m_containers)
        c->refresh();
}

int app::icon_texture(const std::string &uri, esgui::size &siz)
{
    if (uri.empty())
        return 0;
	auto it = m_icons.find(uri);
	if (it != m_icons.end()) {
		siz = it->second.size;
		return it->second.texture;
	}
    std::string folder = "drawable", name = uri;
    if (uri[0] == '@') {
        size_t i = uri.find('/');
        folder = uri.substr(1, i - 1);
        name = uri.substr(i + 1);
    }
    IconData ico;
	ico.texture = android::LoadTexture(folder.c_str(), name.c_str(), ico.size.x, ico.size.y);
	m_icons[uri] = ico;
	siz = ico.size;
	return ico.texture;
}

int app::font_texture(const font& f)
{
	auto it = m_fonts.find({f.face(), f.style()});
	if (it != m_fonts.end())
		return it->second.texture;
	FontData fd;
	bool bold = f.style() & font::bold;
	bool italic = f.style() & font::italic;
    bool underline = f.style() & font::underline;
	fd.texture = android::CreateFontAtlas(f.face().c_str(), bold, italic, underline,
		fd.metrics.char_widths.data(), &fd.metrics.ascent, &fd.metrics.descent);
	m_fonts[{f.face(), f.style()}] = fd;
	return fd.texture;
}

const font_metrics& app::font_metrics(const font& f)
{
    auto it = m_fonts.find({f.face(), f.style()});
    if (it == m_fonts.end()) {
        FontData fd;
        bool bold = f.style() & font::bold;
        bool italic = f.style() & font::italic;
        bool underline = f.style() & font::underline;
        fd.texture = android::CreateFontAtlas(f.face().c_str(), bold, italic, underline,
                                              fd.metrics.char_widths.data(), &fd.metrics.ascent,
                                              &fd.metrics.descent);
        it = m_fonts.insert({{f.face(), f.style()}, fd}).first;
    }
    return it->second.metrics;
}

void app::toast(const std::string& msg)
{
    android::ToastMessage(msg.c_str());
}

int app::status_bar_height()
{
    if (!m_status_bar_height)
        m_status_bar_height = android::GetStatusBarHeight();
    return m_status_bar_height;
}

void app::focus(edit_text* w)
{
    window* last = m_focus;
    m_focus = nullptr;
    if (last)
        last->refresh();
    m_focus = w;
    if (!m_focus)
        return;
    for (container* c : m_containers) {
        if (c->visible())
            c->ensure_visible(m_focus);
    }
    switch (w->input_type()) {
        case edit_text::all:
            android::ShowKeyboard(1);
            break;
        case edit_text::number:
            android::ShowKeyboard(2);
            break;
        case edit_text::decimal:
            android::ShowKeyboard(3);
            break;
    }
}

void app::overlay(window* w)
{
    m_overlay = w;
    if (w) android::ShowKeyboard(0);
}

void app::set_viewport(int width, int height)
{
    //LOGE("set_viewport %d %d", width, height);
    clear_resources();
    check_err();
	glViewport(0, 0, width, height);
    check_err();
	client_size({ (float)width, (float)height });
    check_err();
}

void app::kbd_height(int h) {
    m_kbd_h = h;
    for (container* c : m_containers) {
        if (c->visible())
            c->ensure_visible(m_focus);
    }
}

void app::render()
{
    m_now = std::chrono::system_clock::now();

    check_err();
    color cb = m_theme.background;
	glClearColor(cb.r, cb.g, cb.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
	
	check_err();
	if (m_programs.empty())
		init_rendering();
    check_err();
    glUseProgram(m_programs[0]);
    if (glGetError() != GL_NO_ERROR) //after wakeup all programs are invalid
        init_rendering();
	check_err();

	for (int prg : m_programs) {
		glUseProgram(prg);
		int loc = glGetUniformLocation(prg, "mvp");
		glUniformMatrix4fv(loc, 1, false, m_mvp.data());
	}
	check_err();

    for (container* c : m_containers)
		c->render(m_programs);
    if (m_app_bar)
        m_app_bar->render(m_programs);
    if (m_overlay)
        m_overlay->render(m_programs);
	check_err();
}

void app::touch(action act, float x, float y)
{
    m_now = std::chrono::system_clock::now();
	point p{ x, y };
    if (m_overlay) {
        m_overlay->touch(act, p);
        return;
    }
    if (m_app_bar && m_app_bar->visible() &&
        m_app_bar->rect().contains(p))
    {
        m_app_bar->touch(act, p);
        return;
    }
    for (container* c : m_containers) {
        if (c->visible() && c->rect().contains(p)) {
            c->touch(act, p);
            break;
        }
    }
}

void app::press(int key)
{
    if (m_focus) {
        for (container* c : m_containers) {
            if (c->visible())
                c->ensure_visible(m_focus);
        }
        m_focus->press(key);
    }
    if (m_overlay)
        m_overlay->press(key);
}

}
