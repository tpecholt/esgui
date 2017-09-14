#pragma once
#include "widget.h"
#include "container.h"

namespace esgui
{

class button : public widget
{
public:
	button(container* c);
	button& text(const std::string& l);
	button& color(const esgui::color& color);
	button& text_color(const esgui::color& color);
	button& font(const esgui::font& f);
	button& image(const std::string& uri);
	
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
	int m_image;
	esgui::size m_image_size;
	event<void()> m_on_click;
};

}
