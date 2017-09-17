#include "container.h"
#include "app.h"

namespace esgui {

container::container(container* parent, int id)
	: window(id), m_layout()
{
	m_layout = new row_layout({});
    m_color = "black";
	if (parent)
		parent->register_(this);
	else
		esgui::app::get().register_(this);
}

container::~container()
{
	delete m_layout;
	for (window* w : m_children)
		delete w;
}

void container::register_(window* w)
{
	m_children.push_back(w);
}

void container::layout(esgui::layout* l)
{
	delete m_layout;
	m_layout = l;
	refresh();
}

window* container::find_child(int id)
{
	for (window* w : m_children)
		if (w->id() == id)
			return w;
	return nullptr;
}

void container::delete_children()
{
    for (window* w : m_children)
        delete w;
    m_children.clear();
}

void container::rect(const esgui::rect& r)
{
	m_layout->rect(r);
    refresh();
}

void container::color(const esgui::color& c)
{
    m_color = c;
    refresh();
}

void container::refresh()
{
    if (m_vbos.size() != 1) {
        m_vbos.resize(1);
        glGenBuffers(1, &m_vbos[0].id);
    }
    const auto &r = rect();
    using namespace esguid;
    std::vector<VertexData> vbo;
    PushRect(vbo, r.x, r.y, r.w, r.h, m_color);
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo);

    m_layout->refresh();
}

void container::render(const std::vector<int>& programs)
{
	window::render(programs);

	for (window* w : m_children)
        w->render(programs);
}

bool container::touch(action act, const point& p)
{
    for (window *w : m_children) {
        if (w->visible() && w->rect().contains(p)) {
            w->touch(act, p);
            return true;
        }
    }
    return false;
}

}