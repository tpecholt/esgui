#pragma once
#include "widget.h"
#include "event.h"

namespace esgui{

class popup : public widget
{
public:
	popup(container*);

	float z() const	{ return 10; }
	template <class C>
	popup& items(const C& items)
	{
		m_items.assign(std::begin(items), std::end(items));
		refresh();
		return *this;
	}
	popup& items(std::initializer_list<std::string> items)
	{
		m_items.assign(std::begin(items), std::end(items));
		refresh();
		return *this;
	}
	void touch(action act, const point& p);
	void exec();
	
	template <class T>
	void on_popup(T&& fun) { m_on_popup = std::forward<T>(fun); }	

private:
	float menu_dh() const;
	esgui::rect menu_rect() const;
	void do_refresh();

	std::vector<std::string> m_items;
	int m_sel;
	event<void(int)> m_on_popup;
};

}