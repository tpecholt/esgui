#pragma once
#include "window.h"
#include "layout.h"

namespace esgui {

class container : public window
{
public:
	container(container* cont = nullptr, int id = 0);
	virtual ~container();

	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_layout->rect(); }
	void layout(esgui::layout* l);
	esgui::layout* layout() { return m_layout; }
    esgui::size min_size() { return m_layout->min_size(); }
	void color(const esgui::color& c);

    const std::vector<window*>& children() { return m_children; }
	window* find_child(int id);
    void delete_children();
	void ensure_visible(window* w);

    void touch(action act, const point& p);
	void register_(window* w);
	void render(const std::vector<int>& programs, const point& scroll = point{});
	void refresh();

protected:
	esgui::layout* m_layout;
	std::vector<window*> m_children;
	esgui::color m_color;
	esgui::point m_scroll;
	esgui::point m_last_p;
	bool m_moving;
};

}