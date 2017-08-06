#pragma once
#include "widget.h"

namespace esgui
{

class label : public widget
{
public:
	label();
	label& text(const std::string& l);
	label& color(const esgui::color& color);
	label& text_color(const esgui::color& color);
	label& font(const esgui::font& f);

	size min_size();

protected:
	void do_refresh();	

	std::string m_label;
	esgui::font m_font;
	esgui::color m_text_color;
	esgui::color m_color;
};

}
