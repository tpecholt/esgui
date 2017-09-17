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
	widget(container* c, int id = 0);
	~widget();
	
	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_rect; }

protected:
	esgui::rect m_rect;
};

}