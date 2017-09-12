#pragma once
#include "widget.h"
#include "container.h"

namespace esgui
{

class label : public widget
{
public:
	label(container* c);
	label& text(const std::string& l);
	label& color(const esgui::color& color);
	label& text_color(const esgui::color& color);
	label& font(const esgui::font& f);
	label& alignment(int align);

	size min_size();
	void refresh();
	void touch(action act, const point& p);

	template <class T>
	void on_click(T&& fun) { m_on_click = std::forward<T>(fun); }

protected:
	std::string m_label;
	esgui::font m_font;
	esgui::color m_text_color;
	esgui::color m_color;
	int m_alignment;
	event<void()> m_on_click;
};

}