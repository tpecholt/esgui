#include "GLESNative.h"
#include "esgui/container.h"
#include "esgui/label.h"
#include "esgui/popup.h"
#include "esgui/layout.h"
#include "esgui/esgui.h"
#include <string>
#include <sstream>

using namespace esgui;

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

/**
* Checks for OpenGL errors.Very useful while debugging. Call it as often as required
*/
void CheckGLError(std::string funcName) 
{
	GLenum err = glGetError();
	if (err == GL_NO_ERROR) {
		return;
	}
	else {
		MyLOGF("[FAIL GL] %s", funcName.c_str());
	}

	switch (err) {

	case GL_INVALID_ENUM:
		MyLOGE("GL_INVALID_ENUM: GLenum argument out of range");
		break;

	case GL_INVALID_VALUE:
		MyLOGE("GL_INVALID_VALUE: numeric argument out of range");
		break;

	case GL_INVALID_OPERATION:
		MyLOGE("GL_INVALID_OPERATION: operation illegal in current state");
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		MyLOGE("GL_INVALID_FRAMEBUFFER_OPERATION: framebuffer object is not complete");
		break;

	case GL_OUT_OF_MEMORY:
		MyLOGE("GL_OUT_OF_MEMORY: not enough memory left to execute command");
		break;

	default:
		MyLOGE("unlisted error");
		break;
	}
}

/**
* Class constructor
*/
GLESNative::GLESNative() 
	: m_initDone(false)
{
}

GLESNative::~GLESNative()
{
}

/**
* Perform inits and set various GLES options before rendering image
*/
void GLESNative::Init() 
{
	m_initDone = true;
	
	container* page = new esgui::container();
	
	const char* colors[] = { "red", "blue", "green" };
	label* labels[3];
	for (size_t i = 0; i < 3; ++i) {
		labels[i] = new label(page);
		labels[i]->font({ "normal", 24 });
		labels[i]->color(colors[i]);
		labels[i]->text_color(i ? "black" : "white");
		labels[i]->text("Pepik" + to_string(i));
		if (!i) {
			labels[i]->on_click([&]() {
				m_pop->exec();
			});
		}
		/*if (i)
			m_labels[i].hide();*/
		//labels[i]->rect({0, 0, 0.25*1000, 0.5*1000});
	}
	/*auto r = m_labels[0].min_size();
	m_labels[0].rect({ 0, 0, r.x, r.y });*/

	m_pop = new popup(page);
	m_pop->items({ "prvni", "druha", "treti" });
	m_pop->on_popup([](int idx) {
		int a = 5;
	});	

	page->layout(column(
		item(labels[0], 0.5) |
		row(stretch() | labels[1]) |
		item(labels[2], 0.5)
	));		
}

void GLESNative::Render() 
{
	glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
	esgui::app::get().render();
}

void GLESNative::SetViewport(int width, int height) 
{

	// set the viewport
	// function is also called when user changes device orientation
	glViewport(0, 0, width, height);
	CheckGLError("GLESNative::SetViewport");

	esgui::app::get().client_size({ (float)width, (float)height });
}

void GLESNative::OnTouchEvent(int action, float x, float y)
{
	esgui::action act{};
	switch (action) {
	case 0:
		act = esgui::action::down;
		break;
	case 1:
		act = esgui::action::up;
		break;
	case 2:
		act = esgui::action::move;
		break;
	}
	esgui::app::get().touch(act, x, y);
}