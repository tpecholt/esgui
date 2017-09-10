#include <jni.h>
#include <string>
#include <sstream>
#include "esgui/app.h"
#include "esgui/label.h"
#include "esgui/popup.h"
#include "esgui/layout.h"

using namespace esgui;

template <class T>
std::string to_string(const T& val)
{
    std::ostringstream os;
    os << val;
    return os.str();
}

void init_controls()
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
