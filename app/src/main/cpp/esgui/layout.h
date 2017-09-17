#pragma once

#include "window.h"
#include <vector>

/*
USAGE:
column(
	space() |
	row(stretch(0.2) | item(label, 0.6) | stretch(0.2)) |
	space(1) |
	row(stretch() | button, 0.3) |
	grid<2>(btn1 | btn2 | btn3 | btn4, 0.5)
);
*/
namespace esgui
{

class layout;

struct layout_opts
{
    float m_fixed;
    float m_proportional;
    float m_hmargin, m_vmargin;
    float m_separation;

    layout_opts() : m_fixed(), m_proportional(), m_hmargin(), m_vmargin(), m_separation()
    {}
    layout_opts& fixed(float f) {
        m_fixed = f; return *this;
    }
    layout_opts& proportional(float f) {
        m_proportional = f; return *this;
    }
    layout_opts& hmargin(float f) {
        m_hmargin = f; return *this;
    }
    layout_opts& vmargin(float f) {
        m_vmargin = f; return *this;
    }
    layout_opts& separation(float f) {
        m_separation = f; return *this;
    }
};

inline layout_opts fixed(float f) {
    return layout_opts().fixed(f);
}
inline layout_opts proportional(float f) {
    return layout_opts().proportional(f);
}
inline layout_opts hmargin(float f) {
    return layout_opts().hmargin(f);
}
inline layout_opts vmargin(float f) {
    return layout_opts().vmargin(f);
}
inline layout_opts separation(float f) {
    return layout_opts().separation(f);
}

//-----------------------------------------------------------

class item
{
public:
	item()
		: m_window(), m_layout(), m_prop(0), m_space(0)
	{}
	item(esgui::window* w, float p = 0)
		: item()
	{
		window(w).proportion(p);
	}
    item(esgui::window* w, const layout_opts& opts)
        : item()
    {
        window(w).proportion(opts.m_proportional).space(opts.m_fixed);
    }
    item(esgui::layout* l, float p = 0)
		: item()
	{
		layout(l).proportion(p);
	}
    item(esgui::layout* l, const layout_opts& opts)
            : item()
    {
        layout(l).proportion(opts.m_proportional).space(opts.m_fixed);
    }
    operator layout* () const { return layout(); }
	
	item& space(float s) { m_space = s; return *this; }
	float space() const { return m_space;  }
	item& proportion(float p) { m_prop = p; return *this; }
	float proportion() const { return m_prop;  }
	item& window(esgui::window* w) { m_window = w; return *this; }
	esgui::window* window() const { return m_window; }
	item& layout(esgui::layout* l) { m_layout = l; return *this; }
	esgui::layout* layout() const { return m_layout; }
	item& min_size(esgui::size s) { m_min_size = s; return *this; }

	bool visible() const;
	esgui::size min_size(bool horiz) const;
	void rect(const esgui::rect& r);

private:
	esgui::layout* m_layout;
	esgui::window* m_window;
	float m_prop; //based on all proportions/spaces in a layout
	float m_space; //based on dpi
	esgui::size m_min_size;
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

inline std::vector<item> operator| (window* a, item&& b)
{
    std::vector<item> vec;
    vec.push_back(item(a));
    vec.push_back(std::move(b));
    return vec;
}

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
		: m_visible(true), m_hmargin(), m_vmargin(), m_sep()
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
	void hmargin(float m);
    void vmargin(float m);
	void separation(float m);
	virtual esgui::size min_size() const = 0;
	virtual void refresh() = 0;

protected:
	std::vector<item> m_items;
	esgui::rect m_rect;	
	bool m_visible;
	float m_hmargin, m_vmargin, m_sep;
};

//-------------------------------------------------------------------

class column_layout : public layout
{
public:
    using layout::layout;
    esgui::size min_size() const;
    void refresh();
};

inline item column(std::vector<item>&& items, float prop = 0)
{
	return item(new column_layout(std::move(items)), prop);
}

inline item column(std::vector<item>&& items, const layout_opts& opts)
{
    layout* lay = new column_layout(std::move(items));
    lay->hmargin(opts.m_hmargin);
    lay->vmargin(opts.m_vmargin);
    lay->separation(opts.m_separation);
    return item(lay).proportion(opts.m_proportional).space(opts.m_fixed);
}

//-------------------------------------------------------------------

class row_layout : public layout
{
public:
	using layout::layout;
	esgui::size min_size() const;
	void refresh();
};

inline item row(std::vector<item>&& items, float prop = 0)
{
	return item(new row_layout(std::move(items)), prop);
}

inline item row(std::vector<item>&& items, const layout_opts& opts)
{
    layout* lay = new row_layout(std::move(items));
    lay->hmargin(opts.m_hmargin);
    lay->vmargin(opts.m_vmargin);
    lay->separation(opts.m_separation);
    return item(lay).proportion(opts.m_proportional).space(opts.m_fixed);
}


//-------------------------------------------------------------------


}
