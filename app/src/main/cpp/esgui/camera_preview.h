//
// Created by tomas on 23.09.2017.
//

#ifndef ESGUI_CAMERA_PREVIEW_H
#define ESGUI_CAMERA_PREVIEW_H

#include "widget.h"
#include "container.h"

namespace esgui {

class camera_preview : public widget {
public:
    camera_preview(container* c, int id = 0);
    void refresh();
    void start();
    void stop();

private:
    int m_texture;
};

}

#endif //ESGUI_CAMERA_PREVIEW_H
