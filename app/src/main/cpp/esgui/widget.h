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
	
	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_rect; }
	void refresh() = 0;
	void render(const int programs[], const float mvp[]);	
	
protected:
	struct vbo
	{
		unsigned id;
		int size;
		int texture;
		esgui::rect scissor;
        esgui::point scroll;
		vbo() : id(), size(), texture() {}
	};
	std::vector<vbo> m_vbos;
	esgui::rect m_rect;
};

}