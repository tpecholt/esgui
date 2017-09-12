#include <jni.h>
#include <string>
#include <sstream>
#include "esgui/app.h"
#include "esgui/label.h"
#include "esgui/popup.h"
#include "esgui/layout.h"
#include <algorithm>

using namespace esgui;

template <class T>
std::string to_string(const T& val)
{
    std::ostringstream os;
    os << val;
    return os.str();
}

std::string to_upper(const std::string& s)
{
    std::string ret;
    std::transform(s.begin(), s.end(), std::back_inserter(ret), [](char c){
        return std::toupper(c);
    });
    return ret;
}

std::vector<label*> axis;
int last_axis = 2;

void on_axis(label* lab)
{
    axis[last_axis]->text_color("grey");
    last_axis = std::find(axis.begin(), axis.end(), lab) - axis.begin();
    lab->text_color("orange");
}

void init_controls()
{
    container *page = new container();
    std::vector<label*> buts;
    for (size_t i = 0; i < 3; ++i) {
        label* but = new label(page);
        but->color("dark grey");
        but->text_color("white");
        but->font({"normal", 10, font::bold});
        but->alignment(hcenter | vcenter);
        buts.push_back(but);
    }
    size but_size(0, 100);
    buts[0]->text("BITCOIN");
    buts[1]->text("USD");
    buts[2]->text("GDAX");

    popup* pop1 = new popup(page);
    pop1->items({"Bitcoin", "Ethereum", "Bitcoin Cash", "Ripple", "Litecoin", "Dash", "Iota", "NEM", "neo", "Monero", "Ethereum Classic",
                "Qtum", "OmiseGo", "BitConnect", "Lisk"});
    pop1->on_popup([=](int idx){
        if (idx >= 0)
            buts[0]->text(to_upper(pop1->items()[idx]));
    });
    popup* pop2 = new popup(page);
    pop2->items({"USD", "EUR", "GBP", "CAD", "CNY", "AUD", "PLN", "JPY"});
    pop2->on_popup([=](int idx){
        if (idx >= 0)
            buts[1]->text(to_upper(pop2->items()[idx]));
    });
    popup* pop3 = new popup(page);
    pop3->items({"Kraken", "Bitstamp", "GDAX", "Bitfinex"});
    pop3->on_popup([=](int idx){
        if (idx >= 0)
            buts[2]->text(to_upper(pop3->items()[idx]));
    });

    buts[0]->on_click([=]{
        pop1->exec();
    });
    buts[1]->on_click([=]{
        pop2->exec();
    });
    buts[2]->on_click([=]{
        pop3->exec();
    });

    std::vector<label*> labs;
    for (size_t i = 0; i < 4; ++i) {
        label* lab = new label(page);
        lab->color("black");
        lab->text_color("white");
        lab->font({"normal", 10, font::bold});
        labs.push_back(lab);
    }
    labs[0]->text("4195.00");
    labs[0]->font({"normal", 28, font::bold});
    labs[1]->text("-264.19 -5.8901 %");
    labs[1]->text_color("red");
    labs[2]->text("24H Volume: ");
    labs[2]->text_color("grey");
    labs[2]->font({"normal", 10});
    labs[3]->text("9259");

    for (size_t i = 0; i < 2*4; ++i) {
        int odd = i & 1;
        label* lab = new label(page);
        lab->color("black");
        lab->text_color(odd ? "white" : "grey");
        lab->font({"normal", 10, odd ? font::bold : 0});
        labs.push_back(lab);
    }
    labs[4]->text("HIGH: ");
    labs[5]->text("4356.10");
    labs[6]->text("LOW: ");
    labs[7]->text("4105.00");
    labs[8]->text("BID: ");
    labs[9]->text("4320.00");
    labs[10]->text("ASK: ");
    labs[11]->text("4328.00");

    const char* LBL[] = {"10MIN", "1H", "24H", "1W", "1M", "3M", "1Y", "2Y"};
    for (size_t i = 0; i < 8; ++i) {
        label* lab = new label(page);
        lab->text(LBL[i]);
        lab->color("black");
        lab->text_color(i == last_axis ? "orange" : "grey");
        lab->font({"normal", 10, font::bold});
        lab->on_click([=]{ on_axis(lab); });
        axis.push_back(lab);
    }

    page->layout(column(
        row(
                item(buts[0], 1) | space(1) | item(buts[1], 1) | space(1) | item(buts[2], 1), milimeters(6)
        ) |
        row(
                column(
                        item(labs[0]) |
                        labs[1] |
                        row(item(labs[2]) | labs[3]),
                        1.0
                ) |
                column(
                        item(labs[4]) |
                        labs[6] |
                        labs[8] |
                        labs[10]
                ) |
                column(
                        item(labs[5]) |
                        labs[7] |
                        labs[9] |
                        labs[11]
                )
        ) |
        row(
                item(axis[0], 1.5) | item(axis[1], 1) | item(axis[2], 1) | item(axis[3], 1) |
                item(axis[4], 1) | item(axis[5], 1) | item(axis[6], 1) | item(axis[7], 0)
        )
    ));
}

void init_controls2()
{
    container* page = new container();
    popup *pop = new popup(page);

    const char *colors[] = {"red", "blue", "green"};
    label *labels[3];
    for (int i = 0; i < 3; ++i) {
        labels[i] = new label(page);
        labels[i]->font({"normal", 16 + i*4});
        labels[i]->color(colors[i]);
        labels[i]->text_color(i ? "black" : "white");
        labels[i]->text("Pepik" + to_string(i));
        if (!i) {
            labels[i]->on_click([=]() { //copy pop pointer
                pop->exec();
            });
        }
        /*if (i)
            m_labels[i].hide();*/
        //labels[i]->rect({0, 0, 0.25*1000, 0.5*1000});
    }
    /*auto r = m_labels[0].min_size();
    m_labels[0].rect({ 0, 0, r.x, r.y });*/

    pop->items({"prvni", "druha", "treti"});
    pop->on_popup([=](int idx) { //copy labels[0] pointer
        labels[0]->text(pop->items()[idx]);
    });

    page->layout(column(
            item(labels[0], 0.5) |
            row(stretch() | labels[1]) |
            item(labels[2], 0.5)
    ));
}
