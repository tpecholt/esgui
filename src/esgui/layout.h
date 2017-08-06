#pragma once

#include "widget.h"
#include <vector>

/*
USAGE:
column(
	space() |
	row(stretch(0.2) | item(label, 0.6) | stretch(0.2)) |
	space() |
	row(stretch() | button, 0.3) |
	grid<2>(btn1 | btn2 | btn3 | btn4, 0.5)
);
*/
namespace esgui
{

class layout;

class item
{
public:
	item()
		: m_widget(), m_layout(), m_prop(0), m_space(0)
	{}
	item(esgui::widget& w, float p = 0)
		: item()
	{
		widget(&w).proportion(p);
	}
	item(esgui::layout* l, float p)
		: item()
	{
		layout(l).proportion(p);
	}
	operator layout* () const { return layout(); }
	
	item& space(float s) { m_space = s; return *this; }
	float space() const { return m_space;  }
	item& proportion(float p) { m_prop = p; return *this; }
	float proportion() const { return m_prop;  }
	item& widget(esgui::widget* w) { m_widget = w; return *this; }
	esgui::widget* widget() const { return m_widget; }
	item& layout(esgui::layout* l) { m_layout = l; return *this; }
	esgui::layout* layout() const { return m_layout; }

	bool visible() const;
	esgui::size min_size(bool horiz) const;

private:
	esgui::layout* m_layout;
	esgui::widget* m_widget;
	float m_prop; //based on all proportions/spaces in a layout
	float m_space; //based on dpi
};

inline item stretch(float p = 1.0)
{
	return item().proportion(p);
}

inline item space(float s)
{
	return item().space(s);
}

//-------------------------------------------------------------------------------

inline std::vector<item> operator| (item&& a, item&& b)
{
	std::vector<item> vec;
	vec.push_back(std::move(a));
	vec.push_back(std::move(b));
	return vec;
}

inline std::vector<item> operator| (std::vector<item>&& a, item&& b)
{
	std::vector<item> vec(std::move(a));
	vec.push_back(std::move(b));
	return vec;
}

//----------------------------------------------------------------------

class layout
{
public:
	using iterator = std::vector<item>::iterator;

	layout()
		: m_visible(true)
	{}
	layout(std::vector<item>&& items)
		: layout()
	{
		m_items = std::move(items);
	}
	virtual ~layout() {}
	size_t size() const { return m_items.size();  }
	bool empty() const { return m_items.empty(); }
	void clear() { m_items.clear(); }
	iterator begin() { return m_items.begin();  }
	iterator end() { return m_items.end(); }
	void push_back(item&& i) { m_items.push_back(std::move(i)); }
	void pop_back() { m_items.pop_back(); }
	void insert(iterator it, item&& i) { m_items.insert(it, std::move(i)); }
	void erase(iterator it) { m_items.erase(it); }

	void visible(bool v) { m_visible = v; }
	bool visible() const { return m_visible; }
	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_rect; }
	esgui::size min_size(bool horiz) const;
	virtual void refresh() = 0;

protected:
	std::vector<item> m_items;
	esgui::rect m_rect;	
	bool m_visible;
};

//-------------------------------------------------------------------

class column_layout : public layout
{
public:
	using layout::layout;
	void refresh();
};

inline item column(std::vector<item>&& items, float prop = 0)
{
	return item(new column_layout(std::move(items)), prop);
}

//-------------------------------------------------------------------

class row_layout : public layout
{
public:
	using layout::layout;
	void refresh();
};

inline item row(std::vector<item>&& items, float prop = 0)
{
	return item(new row_layout(std::move(items)), prop);
}

//-------------------------------------------------------------------


}
