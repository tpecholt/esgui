#pragma once

#include "window.h"
#include <string>
#include <vector>

namespace esgui
{

class container;

class widget : public window
{
public:
	widget(container* c);
	
	void visible(bool s) { m_visible = s; }
	bool visible() const { return m_visible; }
	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_rect; }
	void refresh();
	void render(const int programs[]);	
	
protected:
	virtual void do_refresh() = 0;	

protected:
	unsigned m_vbos[2];
	int m_vbo_sizes[2];
	int m_texture;
	bool m_visible;
	esgui::rect m_rect;
};

}