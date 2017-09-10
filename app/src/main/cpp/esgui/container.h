#pragma once
#include "window.h"
#include "layout.h"

namespace esgui {

class container : public window
{
public:
	container(container* cont = {});
	virtual ~container();

	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_layout->rect(); }
	void layout(esgui::layout* l);
	esgui::layout* layout() { return m_layout; }
	const std::vector<window*>& children() { return m_children; }

	virtual esgui::size min_size() { return m_layout->min_size(true); }
	virtual void touch(action act, const point& p);

	void register_(window* w);
	void render(const int programs[], const float mvp[]);
	void refresh();

protected:
	esgui::layout* m_layout;
	std::vector<window*> m_children;
};

}