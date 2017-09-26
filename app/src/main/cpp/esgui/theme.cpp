//
// Created by tomas on 17.09.2017.
//

#include "theme.h"

namespace esgui {

theme get_theme(const std::string& name)
{
    theme th;
    if (name == "kokot");
    else {
        th.app_bar = {0.8, 0.3, 0.35};
        th.background = {0.08, 0.13, 0.27};
        th.text = {0.85, 0.85, 0.85};
        th.button = th.text;
        th.button_text = {0, 0, 0};
        th.edit_text = {1, 0.83, 0.52};
        th.disabled = {0.5, 0.46, 0.39};
        th.focus = th.app_bar;
        th.ftext = font("normal", 10);
        th.fsmall_text = font("normal", 9);
    }
    return th;
}

}