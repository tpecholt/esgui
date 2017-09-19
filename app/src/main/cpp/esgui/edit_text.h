//
// Created by tomas on 17.09.2017.
//

#ifndef ESGUI_EDIT_TEXT_H
#define ESGUI_EDIT_TEXT_H

#include <chrono>
#include "widget.h"

namespace esgui {

class edit_text : public widget
{
public:
    enum style {
        all,
        number,
        decimal,
        phone,
        email,
        password
    };
    edit_text(container* parent, int id = 0);
    void style(enum style);
    enum style style() const { return m_style; }
    void color(const esgui::color& color);
    void text_color(const esgui::color& color);
    void font(const esgui::font& f);
    void text(const std::string& l);
    void hint(const std::string& l);

    size min_size();
    void refresh();
    void touch(action act, const point& p);
    void press(int key);
    void animate(std::chrono::system_clock::time_point tp);

protected:
    enum style m_style;
    std::string m_text;
    std::string m_hint;
    esgui::font m_font;
    esgui::color m_text_color;
    esgui::color m_color;
    int m_sel;
    std::chrono::system_clock::time_point m_tp;
};

}

#endif //ESGUI_TEXT_FIELD_H
