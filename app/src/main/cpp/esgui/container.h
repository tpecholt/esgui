#pragma once
#include "window.h"
#include "button.h"
#include "layout.h"

namespace esgui {

class button;

class container : public window
{
public:
	container(container* cont = nullptr, int id = 0);
	virtual ~container();

	void rect(const esgui::rect& r);
	const esgui::rect& rect() const { return m_rect; }
    void shrink();
	void layout(esgui::layout* l);
	esgui::layout* layout() { return m_layout; }
    esgui::size min_size() { return m_layout->min_size(); }
	void color(const esgui::color& c);

    const std::vector<window*>& children() { return m_children; }
	window* find_child(int id);
    void delete_children();
	void ensure_visible(window* w);
	void action_button(esgui::button* but);
	esgui::button* action_button() const { return m_action_button; }

    void touch(action act, const point& p);
	void register_(window* w);
	void render(const std::vector<int>& programs, const point& scroll = point{});
	void refresh();

    template <class T>
    void on_back(T&& fun) { m_on_back = std::forward<T>(fun); }
    void on_back() { if (m_on_back) m_on_back(); }

protected:
	esgui::layout* m_layout;
    esgui::rect m_rect;
	std::vector<window*> m_children;
	esgui::button* m_action_button;
	esgui::color m_color;
	esgui::point m_scroll;
	esgui::point m_last_p;
	bool m_moving;
    event<void()> m_on_back;
};

}