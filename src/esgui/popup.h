#pragma once
#include "widget.h"

namespace esgui{

class popup : public widget
{
public:
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
private:
	void do_refresh();

	std::vector<std::string> m_items;
};

}