#pragma once
#include "widget.h"
#include "event.h"

namespace esgui{

class popup : public widget
{
public:
	popup(container*);

	int z() const	{ return 5; }
	template <class C>
	popup& items(const C& items)
	{
		m_items.assign(std::begin(items), std::end(items));
		return *this;
	}
	popup& items(std::initializer_list<std::string> items)
	{
		m_items.assign(std::begin(items), std::end(items));
		return *this;
	}
	const std::vector<std::string>& items() const { return m_items; }
	popup& selection(int sel) { m_sel = sel; return* this; }
	int selection() const { return m_sel; }
	void refresh();
	void touch(action act, const point& p);
	void exec();    
	
	template <class T>
	void on_popup(T&& fun) { m_on_popup = std::forward<T>(fun); }	

private:
	float menu_dh() const;
	esgui::rect menu_rect() const;

	std::vector<std::string> m_items;
	event<void(int)> m_on_popup;
	int m_sel;

	int m_highlighted;
    float m_scroll;
	esgui::point m_last_p;
	bool m_moving;
};

}