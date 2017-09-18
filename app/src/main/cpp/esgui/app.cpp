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

	m_programs.push_back(create_program(vertexSource, fragmentSource1));
	m_programs.push_back(create_program(vertexSource, fragmentSource2));
	
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

const font& app::default_font()
{
    if (!m_default_font.texture())
        m_default_font = font("normal", 10);
    return m_default_font;
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
    bool underline = style & font::underline;
	int texture = android::CreateFontAtlas(face.c_str(), bold, italic, underline,
		font.metrics.char_widths.data(), &font.metrics.ascent, &font.metrics.descent);	
	m_fonts[texture] = font;
	return texture;
}

const font_metrics& app::font_metrics(const font& f)
{
	static esgui::font_metrics tmp;
	auto it = m_fonts.find(f.texture());
	if (it == m_fonts.end())
		return tmp;
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

void app::focus(window* w)
{
    window* last = m_focus;
    m_focus = nullptr;
    if (last)
        last->refresh();
    m_focus = w;
    update_scroll();
    android::ShowKeyboard(w);
}

void app::set_viewport(int width, int height)
{
    check_err();
	glViewport(0, 0, width, height);
    check_err();
	client_size({ (float)width, (float)height });
    check_err();
}

void app::kbd_height(int h) {
    m_kbd_h = h;
    update_scroll();
}

void app::update_scroll()
{
    container* cont = nullptr;
    for (container* c : m_containers) {
        if (c->visible()) {
            cont = c;
            break;
        }
    }
    if (!cont)
        return;
    if (m_focus) {
        float dpmm = m_dpi / 25.4;
        esgui::rect r = m_focus->rect();
        if (r.y + m_scroll.y - dpmm < 0)
            m_scroll.y = dpmm - r.y;
        if (r.y + r.h + m_scroll.y + dpmm > m_client_size.y - m_kbd_h)
            m_scroll.y = m_client_size.y - m_kbd_h - r.y - r.h - dpmm;
    }
    m_scroll.y = std::max(m_scroll.y, m_client_size.y - m_kbd_h - cont->rect().h);
    m_scroll.y = std::min(m_scroll.y, 0.f);
}

void app::render()
{
    m_now = std::chrono::system_clock::now();

    check_err();
	glClearColor(0.f, 0.f, 0.f, 1.0f);
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
		c->render(m_programs, m_scroll);
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

    container* cont = nullptr;
    for (container* c : m_containers) {
        if (c->visible()) {
            cont = c;
            break;
        }
    }
    if (!cont)
        return;

    switch (act) {
        case action::down:
            m_last_p = p;
            m_moving = false;
            break;
        case action::move: {
            if (!m_moving && std::abs(p.y - m_last_p.y) < 10)
                break;
            m_moving = true;
            m_scroll.y += p.y - m_last_p.y;
            float h = cont->rect().h;
            m_scroll.y = std::max(m_scroll.y, m_client_size.y - m_kbd_h - h);
            m_scroll.y = std::min(m_scroll.y, 0.f);
            m_last_p = p;
            return;
        }
        case action::up:
            if (m_moving)
                return;
            break;
    }

    p.x -= m_scroll.x;
    p.y -= m_scroll.y;
	if (cont->rect().contains(p)) {
        cont->touch(act, p);
        return;
    }
}

void app::press(int key)
{
    if (m_focus) {
        update_scroll();
        m_focus->press(key);
    }
    if (m_overlay)
        m_overlay->press(key);
}

}
