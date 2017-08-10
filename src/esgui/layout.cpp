#include "layout.h"
#include "esgui.h"
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
	esgui::size siz;
	for (const item& i : m_items) {
		esgui::size s = i.min_size(horiz);
		siz.x = std::max(siz.x, s.x);
		siz.y = std::max(siz.y, s.y);
	}
	return siz;
}

void row_layout::refresh()
{
	float sum_space = 0;
	float sum_prop = 0;
	for (const item& i : m_items)
	{
		if (!visible())
			continue;
		sum_prop += i.proportion();
		if (!i.proportion())
			sum_space += i.min_size(true).x;		
	}
	float x = m_rect.x;
	float prop_w = m_rect.w - sum_space;
	for (const item& i : m_items)
	{
		if (!i.visible())
			continue;
		float w = i.min_size(true).x;
		if (i.proportion())
			w = std::max(w, i.proportion() * prop_w / sum_prop);
		if (i.window())
			i.window()->rect({ x, m_rect.y, w, m_rect.h });
		if (i.layout())
			i.layout()->rect({ x, m_rect.y, w, m_rect.h });
		x += w;
	}
}

void column_layout::refresh()
{
	float sum_space = 0;
	float sum_prop = 0;
	for (const item& i : m_items)
	{
		if (!i.visible())
			continue;
		sum_prop += i.proportion();
		if (!i.proportion())
			sum_space += i.min_size(false).y;		
	}
	float y = m_rect.y;
	float prop_h = m_rect.h - sum_space;
	for (const item& i : m_items)
	{
		if (!i.visible())
			continue;
		float h = i.min_size(false).y;
		if (i.proportion())
			h = std::max(h, i.proportion() * prop_h / sum_prop);
		if (i.window())
			i.window()->rect({ m_rect.x, y, m_rect.w, h });
		if (i.layout()) 
			i.layout()->rect({ m_rect.x, y, m_rect.w, h });		
		y += h;
	}
}

}