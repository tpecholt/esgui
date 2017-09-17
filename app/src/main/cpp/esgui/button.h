#pragma once
#include "widget.h"
#include "container.h"

namespace esgui
{

class button : public widget
{
public:
    enum style {
        flat,
        raised,
        round
    };
	button(container* c, int id = 0);
    void style(style);
	void text(const std::string& l);
	void color(const esgui::color& color);
	void text_color(const esgui::color& color);
	void font(const esgui::font& f);
	void icon(const std::string& uri, float scale = 1);
	
	size min_size();
	void refresh();
	bool touch(action act, const point& p);

	template <class T>
	void on_click(T&& fun) { m_on_click = std::forward<T>(fun); }

protected:
    enum style m_style;
	std::string m_label;
	esgui::font m_font;
	esgui::color m_text_color;
	esgui::color m_color;
	int m_texture;
	esgui::size m_texture_size;
	event<void()> m_on_click;
};

}
