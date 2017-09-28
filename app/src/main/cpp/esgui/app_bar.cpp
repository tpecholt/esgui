//
// Created by tomas on 15.09.2017.
//

#include "app_bar.h"
#include "app.h"
#include "button.h"
#include "label.h"
#include "android.h"

namespace esgui {

class side_panel : public container
{
public:
    using container::container;

    void touch(action act, const point& p)
    {
        esgui::rect r = rect();
        if (!r.contains(p)) {
            app::get().overlay(nullptr);
            visible(false);
        }
        else {
            container::touch(act, p);
        }
    }
    void press(int key)
    {
        if (key == 0x1b) {
            app::get().overlay(nullptr);
            visible(false);
        }
    }
};

//-----------------------------------------------------------

app_bar::app_bar()
: container(nullptr, sid)
{
    layout(column(
            space(4) | row({}) | space(1)
    ));
    m_color = app::get().theme().app_bar;
}

size app_bar::min_size()
{
    float dpmm = app::get().screen_dpi() / 25.4;
    size client = app::get().client_size();
    size s{client.x, layout()->min_size().y};
    //int h = app::get().status_bar_height();
    //s.y += 3*dpmm;
    //if (h) s.y += h + 0*dpmm;
    return s;
}

void app_bar::refresh()
{
    container::refresh();
    android::SetDarkStatusBar(m_color.luminance() < 0.3);
}

void app_bar::style(int s)
{
    float dpmm = app::get().screen_dpi() / 25.4;
    m_style = s;
    if ((m_style & left_side_bar) && (m_style & right_side_bar))
        m_style &= ~right_side_bar;

    esgui::layout* lay = layout()->begin()[1].layout();
    lay->hmargin(2);
    lay->separation(2);
    lay->clear();
    delete_children();

    button *bside = nullptr;
    if (m_style & (left_side_bar | right_side_bar)) {
        container* side = new side_panel(this, id_side_bar);
        side->visible(false);
        side->layout(column({}));
        bside = new button(this, id_side_button);
        bside->color(color::transparent);
        bside->text("side");
        bside->icon("@drawable/ic_menu", {4*dpmm, 4*dpmm});
        bside->on_click([=] {
            size client = app::get().client_size();
            float w = 0.7 * std::min(client.x, client.y);
            float h = client.y - app::get().status_bar_h();
            side->rect({client.x - w, client.y - h, w, h});
            side->visible(true);
            app::get().overlay(side);
        });
    }

    if (m_style & back_button) {
        button *back = new button(this, id_back_button);
        back->color(color::transparent);
        back->text("back");
        back->icon("@drawable/ic_chevron_left");
        lay->push_back(item(back));
    }

    if (m_style & left_side_bar)
        lay->push_back(item(bside));

    label *lab1 = new label(this, id_label1);
    lab1->color(color::transparent);
    lab1->text_color(app::get().theme().button_text);
    lab1->alignment(left | vcenter);
    lab1->font(lab1->font().make_bold());
    lay->push_back(item(lab1, 1));

    if (m_style & menu_button) {
        esgui::menu* m = new esgui::menu(this, id_menu);
        button *but = new button(this, menu_button);
        but->color(color::transparent);
        but->icon("@drawable/ic_more_vert", {4*dpmm, 4*dpmm});
        but->text("menu");
        but->on_click([=]{
            m->exec({ but->rect().x, but->rect().y + dpmm });
        });
        lay->push_back(item(but));
    }

    if (m_style & right_side_bar)
        lay->push_back(item(bside));

    refresh();
}

void app_bar::text(const std::string &t)
{
    window *w = find_child(id_label1);
    if (w) ((label *) w)->text(t);
}

void app_bar::small_text(const std::string &t)
{
    window *w = find_child(id_label2);
    if (w) ((label *) w)->text(t);
}



}