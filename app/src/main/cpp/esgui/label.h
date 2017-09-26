#pragma once
#include "widget.h"
#include "container.h"

namespace esgui
{

class label : public widget
{
public:
	label(container* c, int id = 0);

	void text(const std::string& l);
	void color(const esgui::color& color);
    esgui::color color() const { return m_color; }
	void font(const esgui::font& f);
    esgui::font font() const { return m_font; };
	void  text_color(const esgui::color& color);
	void alignment(int align);

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
