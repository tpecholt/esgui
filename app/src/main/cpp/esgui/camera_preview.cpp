//
// Created by tomas on 23.09.2017.
//

#include "camera_preview.h"
#include "impl.h"
#include "android.h"

namespace esgui {

camera_preview::camera_preview(container* parent, int id)
: widget(parent, id)
{}

void camera_preview::refresh()
{
    if (m_vbos.size() != 1) {
        m_vbos.resize(1);
        glGenBuffers(1, &m_vbos[0].id);
    }
    esgui::rect r = rect();
    std::vector<esguid::VertexData> vbo;
    esguid::PushRect(vbo, r.x, r.y, r.w, r.h, "white");
    m_vbos[0].size = esguid::SendBuffer(m_vbos[0].id, vbo);
    m_vbos[0].texture = -m_texture;
}

void camera_preview::start()
{
    m_texture = android::OpenCamera(true);
    refresh();
}

void camera_preview::stop()
{
    android::OpenCamera(false);
}

}