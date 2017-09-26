//
// Created by tomas on 17.09.2017.
//

#ifndef ESGUI_THEME_H
#define ESGUI_THEME_H

#include "window.h"

namespace esgui {

struct theme {
    color background;
    color app_bar;
    color button_text;
    color button;
    color text;
    color edit_text;
    color focus;
    color disabled;
    font ftext;
    font fsmall_text;
};

theme get_theme(const std::string& name);

}

#endif //ESGUI_THEME_H
