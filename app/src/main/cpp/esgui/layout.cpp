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
    float sp = space() * esgui::app::get().screen_dpi() / 25.4;
    if (sp)
        siz = horiz ? size{sp, 0} : size{0, sp};
	else if (window())
		siz = window()->min_size();
	else if (layout())
		siz = layout()->min_size();

    if (m_min_size.x)
		siz.x = m_min_size.x;
	if (m_min_size.y)
		siz.y = m_min_size.y;
	
	return siz;
}

//--------------------------------------------------------------------------

void layout::rect(const esgui::rect& r)
{
	m_rect = r;
	refresh();
}

void layout::hmargin(float m)
{
	m_hmargin = m;
	refresh();
}

void layout::vmargin(float m)
{
    m_vmargin = m;
    refresh();
}

void layout::separation(float m)
{
	m_sep = m;
	refresh();
}

//--------------------------------------------------------------------------

esgui::size row_layout::min_size() const
{
    float dpmm = esgui::app::get().screen_dpi() / 25.4;
    check_err();
    esgui::size siz{2*m_hmargin*dpmm, 2*m_vmargin};
    int n = 0;
    for (const item& i : m_items) {
        if (!i.visible())
            continue;
        esgui::size s = i.min_size(true);
        siz.x += s.x;
        siz.y = std::max(siz.y, s.y);
        ++n;
    }
    if (n)
        siz.x += (n-1) * m_sep * dpmm;
    check_err();
    return siz;
}

void row_layout::refresh()
{
    if (m_rect.invalid())
        return;
    float dpmm = esgui::app::get().screen_dpi() / 25.4;
    float sum_min = 2 * m_hmargin * dpmm;
    float sum_fixed = 2 * m_hmargin * dpmm;
    float sum_prop = 0;
    check_err();
	int n = 0;
	for (const item& i : m_items)
	{
		if (!visible())
			continue;
		sum_prop += i.proportion();
        float w = i.min_size(true).x;
        sum_min += w;
		if (!i.proportion())
			sum_fixed += w;
		++n;
	}
	if (n) {
		sum_min += (n - 1) * m_sep * dpmm;
		sum_fixed += (n - 1) * m_sep * dpmm;
	}
    check_err();
    bool overflow = sum_min > m_rect.w;
    float x = m_rect.x + m_hmargin * dpmm;
    float y = m_rect.y + m_vmargin * dpmm;
    float h = m_rect.h - 2 * m_vmargin * dpmm;
	float prop_w = m_rect.w - sum_fixed;
	for (item& i : m_items)
	{
		if (!i.visible())
			continue;
		float w = i.min_size(true).x;
		if (!overflow && i.proportion())
			w = std::max(w, i.proportion() * prop_w / sum_prop);
		i.rect({ x, y, w, h });
		x += w;
        if (n > 1)
            x += m_sep * dpmm;
        --n;
	}
    check_err();
}

//--------------------------------------------------------------------------

esgui::size column_layout::min_size() const
{
    check_err();
    float dpmm = esgui::app::get().screen_dpi() / 25.4;
    esgui::size siz{2*m_hmargin*dpmm, 2*m_vmargin*dpmm};
    int n = 0;
    for (const item& i : m_items) {
        if (!i.visible())
            continue;
        esgui::size s = i.min_size(false);
        siz.x = std::max(siz.x, s.x);
        siz.y += s.y;
        ++n;
    }
    if (n)
        siz.y += (n-1) * m_sep * dpmm;
    check_err();
    return siz;
}

void column_layout::refresh()
{
    if (m_rect.invalid())
        return;
    float dpmm = esgui::app::get().screen_dpi() / 25.4;
    float sum_min = 2 * m_vmargin * dpmm;
    float sum_fixed = 2 * m_vmargin * dpmm;
    float sum_prop = 0;
    check_err();
    int n = 0;
    for (const item& i : m_items)
	{
		if (!i.visible())
			continue;
		sum_prop += i.proportion();
        float h = i.min_size(false).y;
        sum_min += h;
        if (!i.proportion())
			sum_fixed += h;
        ++n;
	}
    if (n) {
        sum_min += (n - 1) * m_sep * dpmm;
        sum_fixed += (n - 1) * m_sep * dpmm;
    }
    check_err();
    bool overflow = sum_min > m_rect.h;
    float x = m_rect.x + m_hmargin * dpmm;
	float y = m_rect.y + m_vmargin * dpmm;
    float w = m_rect.w - 2 * m_hmargin * dpmm;
	float prop_h = m_rect.h - sum_fixed;
	for (item& i : m_items)
	{
		if (!i.visible())
			continue;
		float h = i.min_size(false).y;
		if (!overflow && i.proportion())
			h = std::max(h, i.proportion() * prop_h / sum_prop);
        check_err();
		i.rect({ x, y, w, h });
        check_err();
		y += h;
        if (n > 1)
            y += m_sep * dpmm;
        --n;
	}
    check_err();
}

}