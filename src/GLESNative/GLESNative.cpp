#include "GLESNative.h"
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
	: m_initDone(false), m_layout()
{
}

GLESNative::~GLESNative()
{
	delete m_layout;
}

/**
* Perform inits and set various GLES options before rendering image
*/
void GLESNative::Init() 
{
	m_initDone = true;
	
	const char* colors[] = { "red", "blue", "green" };
	for (size_t i = 0; i < m_labels.size(); ++i) {
		m_labels[i]
			.font({ "normal", 24 })
			.color(colors[i])
			.text_color("white")
			.text("Pepik" + to_string(i));
		/*if (i)
			m_labels[i].hide();*/
		m_labels[i].rect({0, 0, 0.25*1000, 0.5*1000});
	}
	/*auto r = m_labels[0].min_size();
	m_labels[0].rect({ 0, 0, r.x, r.y });*/

	m_popup.items({ "prvni", "druha", "treti" });
	//m_popup.visible(false);

	m_layout = column(
		item(m_labels[0], 0.5) |
		row(stretch() | m_labels[1]) |
		item(m_labels[2], 0.5)
	);		
}

void GLESNative::Render() 
{
	glClearColor(1.0f, 1.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*float v[] = { -1, -1, 1, 0, 0,
				  1, 1, 1, 0, 0,
				  -1, 1, 0, 1, 0,
				  1, -1, 0, 1, 0,
	};
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. Must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5*sizeof(float),    // stride
		v					// array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 1. Must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5*sizeof(float),    // stride
		v + 2				// array buffer offset
	);
	glDrawArrays(GL_LINES, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//CheckGLError("GLESNative::Render");*/

	esgui::app::get().render();
}

void GLESNative::SetViewport(int width, int height) 
{

	// set the viewport
	// function is also called when user changes device orientation
	glViewport(0, 0, width, height);
	CheckGLError("GLESNative::SetViewport");

	esgui::app().get().client_size({ (float)width, (float)height });
	m_layout->rect({ 0, 0, (float)width, (float)height });	
	m_popup.refresh();
}