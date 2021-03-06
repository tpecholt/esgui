#pragma once
#include "widget.h"
#include "container.h"
#include "edit_text.h"
#include "impl.h"
#include <string>
#include <vector>
#include <array>
#include <map>
#include <chrono>
#include "app_bar.h"
#include "theme.h"

namespace esgui
{

struct font_metrics
{
	std::array<float, esguid::FONT_ATLAS_COLS * esguid::FONT_ATLAS_COLS> char_widths;
	float ascent;
	float descent;
	font_metrics() : ascent(), descent(), char_widths() {}
};
using matrix4 = std::array<float, 16>;

class app
{
public:
	app();
	static app& get();

	void clear_resources();
    void register_(container* w);
    void activate(container *);
    void overlay(window* w);
    void focus(edit_text* w);
	window* focus() const { return m_focus; }
	int icon_texture(const std::string& uri, esgui::size& siz);
	int font_texture(const font& font);
	const font_metrics& font_metrics(const font& font);
	void client_size(size s);
	size client_size() const { return m_client_size; }
	size screen_size();
	int screen_dpi();
	void toast(const std::string& msg);
    int status_bar_h();
    void theme(const esgui::theme& t);
    const esgui::theme& theme() const { return m_theme; }
    std::chrono::system_clock::time_point now() const { return m_now; }
	void kbd_height(int h);
	int kbd_height() const { return m_kbd_h; }

    void set_viewport(int w, int h);
	void touch(action action, float x, float y);
	void press(int key);
	void render();	

private:
	void init_rendering();

	struct FontData
	{
		int texture;
		esgui::font_metrics metrics;
	};
	struct IconData
	{
		esgui::size size;
		int texture;
	};
	std::vector<container*> m_containers;
    app_bar* m_app_bar;
    window* m_overlay;
    window* m_focus;
	std::map<std::pair<std::string, int>, FontData> m_fonts;
	std::map<std::string, IconData> m_icons;
	std::vector<int> m_programs;
	matrix4 m_mvp;
	size m_screen_size;
	size m_client_size;
    int m_kbd_h;
	int m_dpi;
    int m_status_bar_h;
    esgui::theme m_theme;
    std::chrono::system_clock::time_point m_now;
};

}