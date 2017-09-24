//
// Created by tomas on 23.09.2017.
//

#include "camera_preview.h"
#include "impl.h"
#include "android.h"
#include "app.h"

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
    esgui::color c = "white";
    int rot = android::GetDisplayRotation();
    using namespace esguid;

    std::vector<VertexData> vbo;
    esgui::point st, dstx, dsty;
    if (rot == 0 || rot == 180) {
        st = {0, 1};
        dstx = {0, -1};
        dsty = {1, 0};
    }
    else if (rot == 90) {
        st = {0, 0};
        dstx = {1, 0};
        dsty = {0, 1};
    }
    else if (rot == 270) {
        st = {1, 1};
        dstx = {-1, 0};
        dsty = {0, -1};
    }
    vbo.push_back(VertexData{ r.x, r.y, st.x, st.y, c });
    vbo.push_back(VertexData{ r.x, r.y + r.h, st.x + dsty.x, st.y + dsty.y, c });
    vbo.push_back(VertexData{ r.x + r.w, r.y + r.h, st.x + dstx.x + dsty.x, st.y + dstx.y + dsty.y, c });
    vbo.push_back(vbo[0]);
    vbo.push_back(vbo[2]);
    vbo.push_back(VertexData{ r.x + r.w, r.y, st.x + dstx.x, st.y + dstx.y, c });
    m_vbos[0].size = SendBuffer(m_vbos[0].id, vbo);
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