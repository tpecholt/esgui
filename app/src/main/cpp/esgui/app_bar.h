//
// Created by tomas on 15.09.2017.
//

#ifndef ESGUI_APP_BAR_H
#define ESGUI_APP_BAR_H

#include "container.h"
#include "layout.h"
#include "menu.h"

namespace esgui {

class app_bar : public container {
public:
    enum style {
        back_button = 0x1,
        left_side_bar = 0x2,
        right_side_bar = 0x4,
        text_box = 0x8,
        menu_button = 0x10,
    };
    static constexpr int sid = -1;

    app_bar();
    size min_size();
    void color(const esgui::color &c);

    void style(int style);
    container* side_bar() { return (container*)find_child(id_side_bar); }
    menu* menu() { return (esgui::menu*)find_child(id_menu); }
    void text(const std::string&);
    void small_text(const std::string&);
    void menu_items(const std::vector<std::string>& items);

    /*template <class T>
    void on_back(T&& fun) { m_on_back = std::forward<T>(fun); }
    template <class T>
    void on_menu(T&& fun) { m_on_menu = std::forward<T>(fun); }*/

private:
    enum id {
        id_back_button,
        id_side_button,
        id_label1,
        id_label2,
        id_menu_button,
        id_side_bar,
        id_menu,
    };

    int m_style;
};

}

#endif //ESGUI_ACTION_BAR_H
