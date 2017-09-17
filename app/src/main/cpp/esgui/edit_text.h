//
// Created by tomas on 17.09.2017.
//

#ifndef ESGUI_EDIT_TEXT_H
#define ESGUI_EDIT_TEXT_H

#include "widget.h"

namespace esgui {

class edit_text : public widget
{
public:
    enum style {
        all,
        number,
        password
    };
    edit_text(container* parent, int id = 0);
    void style(style);
    void color(const esgui::color& color);
    void text_color(const esgui::color& color);
    void font(const esgui::font& f);
    void text(const std::string& l);
    void hint(const std::string& l);
    void focused(bool f);
    bool focused() const { return m_focused; }

    size min_size();
    void refresh();
    bool touch(action act, const point& p);

protected:
    enum style m_style;
    std::string m_text;
    std::string m_hint;
    esgui::font m_font;
    esgui::color m_text_color;
    esgui::color m_color;
    bool m_focused;
};

}

#endif //ESGUI_TEXT_FIELD_H
