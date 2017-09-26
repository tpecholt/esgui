#pragma once
#include "widget.h"
#include "event.h"

namespace esgui{

class menu : public widget
{
public:
	menu(container*, int id = 0);

	esgui::font font() const;
	template <class C>
	void items(const C& items)
	{
		m_items.assign(std::begin(items), std::end(items));
	}
	void items(std::initializer_list<std::string> items)
	{
		m_items.assign(std::begin(items), std::end(items));
	}
	const std::vector<std::string>& items() const { return m_items; }
    void label(const std::string& l) { m_label = l; }
    const std::string& label() const { return m_label; }
	void refresh();
	void touch(action act, const point& p);
	void press(int code);
	void exec(const point& p);
	
	template <class T>
	void on_menu(T&& fun) { m_on_menu = std::forward<T>(fun); }

private:
	float menu_dh() const;
	size menu_size();

    std::string m_label;
	std::vector<std::string> m_items;
    int m_highlighted;
    event<void(int)> m_on_menu;

	esgui::point m_last_p;
	bool m_moving;
};

}