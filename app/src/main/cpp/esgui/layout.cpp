#include "layout.h"
#include "app.h"
#include <algorithm>

namespace esgui {

bool item::visible() const
{
	if (window())
		return window()->visible();
	else if (layout())
		return layout()->visible();
	return true;
}

void item::rect(const esgui::rect& r)
{
    if (window())
        window()->rect(r);
    else if (layout())
        layout()->rect(r);
}

esgui::size item::min_size(bool horiz) const
{
	esgui::size siz;
	if (window())
		siz = window()->min_size();
	else if (layout())
		siz = layout()->min_size(horiz);
	
	float sp = space() * esgui::app::get().screen_dpi() / 25.4;
	if (horiz)
		siz.x += sp;
	else
		siz.y += sp;

	return siz;
}

//--------------------------------------------------------------------------

void layout::rect(const esgui::rect& r)
{
	m_rect = r;
	refresh();
}

esgui::size layout::min_size(bool horiz) const
{
    check_err();
	esgui::size siz;
	for (const item& i : m_items) {
		esgui::size s = i.min_size(horiz);
		siz.x = std::max(siz.x, s.x);
		siz.y = std::max(siz.y, s.y);
	}
    check_err();
	return siz;
}

void row_layout::refresh()
{
    float sum_min = 0;
    float sum_fixed = 0;
    float sum_prop = 0;
    check_err();
	for (const item& i : m_items)
	{
		if (!visible())
			continue;
		sum_prop += i.proportion();
        float w = i.min_size(true).x;
        sum_min += w;
		if (!i.proportion())
			sum_fixed += w;
	}
    check_err();
    bool overflow = sum_min > m_rect.w;
    float x = m_rect.x;
	float prop_w = m_rect.w - sum_fixed;
	for (item& i : m_items)
	{
		if (!i.visible())
			continue;
		float w = i.min_size(true).x;
		if (!overflow && i.proportion())
			w = std::max(w, i.proportion() * prop_w / sum_prop);
		i.rect({ x, m_rect.y, w, m_rect.h });
		x += w;
	}
    check_err();
}

void column_layout::refresh()
{
    float sum_min = 0;
    float sum_fixed = 0;
    float sum_prop = 0;
    check_err();
    for (const item& i : m_items)
	{
		if (!i.visible())
			continue;
		sum_prop += i.proportion();
        float h = i.min_size(true).y;
        sum_min += h;
        if (!i.proportion())
			sum_fixed += i.min_size(false).y;		
	}
    check_err();
    bool overflow = sum_min > m_rect.h;
	float y = m_rect.y;
	float prop_h = m_rect.h - sum_fixed;
	for (item& i : m_items)
	{
		if (!i.visible())
			continue;
		float h = i.min_size(false).y;
		if (!overflow && i.proportion())
			h = std::max(h, i.proportion() * prop_h / sum_prop);
        check_err();
		i.rect({ m_rect.x, y, m_rect.w, h });
        check_err();
		y += h;
	}
    check_err();
}

}