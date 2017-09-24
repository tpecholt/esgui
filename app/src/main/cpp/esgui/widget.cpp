#include "widget.h"
#include "app.h"
#include <GLES2/gl2.h>

namespace esgui
{

widget::widget(container* cont, int id)
: window(id)
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
	m_rect = r;
	refresh();
}

}