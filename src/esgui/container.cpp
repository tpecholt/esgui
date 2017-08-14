#include "container.h"
#include "esgui.h"

namespace esgui {

container::container(container* cont)
	: m_layout()
{
	m_layout = row({});
	if (cont)
		cont->register_(this);
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

void container::rect(const esgui::rect& r)
{
	m_layout->rect(r);
}

void container::refresh()
{
	m_layout->refresh();
}

void container::render(const int programs[])
{
    for (size_t i = 0; i < 7; ++i) {
        for (window* w : m_children)
            if (w->z() == i)
                w->render(programs);
    }
}

void container::touch(action act, const point& p)
{
	window* win{};
	int maxZ = -INT_MAX;
	for (window* w : m_children) {
		if (!w->visible() || !w->rect().contains(p))
			continue;
		if (w->z() > maxZ) {
			maxZ = w->z();
			win = w;
		}
	}
	if (win)
		win->touch(act, p);
}

}