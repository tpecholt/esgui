#pragma once
#include "widget.h"
#include "container.h"
#include "impl.h"
#include <string>
#include <vector>
#include <array>
#include <map>

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
	
	void register_(container* w);
	int font_texture(const std::string& face, int style);
	font default_font() { return m_defaultFont; }
	const font_metrics& font_metrics(font font);
	void client_size(size s);
	size client_size() const { return m_client_size; }
	size screen_size();
	int screen_dpi();
	
	void touch(action action, float x, float y);	
	void render();	

private:
	void init_rendering();

	std::vector<container*> m_containers;
	font m_defaultFont;
	struct FontData
	{
		std::string face;
		int style;
		esgui::font_metrics metrics;
	};
	std::map<int, FontData> m_fonts;
	std::vector<int> m_programs;
	matrix4 m_mvp;
	size m_screen_size;
	size m_client_size;
	int m_dpi;
};

}