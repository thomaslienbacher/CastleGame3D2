//
// Created by Thomas Lienbacher on 15.12.2020.
//

#include "debug.hpp"
#include "shader.hpp"
#include <glm/gtx/common.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace debug {
    static float color[3] = {1.0, 1.0, 1.0};
    static GLint current_program = 0;
    static float *view_matrix = nullptr;
    static float *proj_matrix = nullptr;
    static bool depth_test_disabled = false;

    static void prepare() {
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        Shader::use(0);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(color[0], color[1], color[2]);
    }

    static void restore() {
        Shader::use(current_program);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);
        if (depth_test_disabled) {
            glEnable(GL_DEPTH_TEST);
            depth_test_disabled = false;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void set_cam(Camera &cam) {
        proj_matrix = glm::value_ptr(cam.projection);
        view_matrix = glm::value_ptr(cam.view);
    }

    void set_color(float r, float g, float b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void point(glm::vec2 point) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_POINTS);
        glVertex2f(point.x, point.y);
        glEnd();
        restore();
    }

    void sphere(glm::vec3 position, float radius) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        const int PLANES = 16;
        for (int plane = (-PLANES / 2); plane <= (PLANES / 2); ++plane) {
            glBegin(GL_LINE_LOOP);
            for (float angle = 0; angle <= glm::two_pi<float>(); angle += glm::two_pi<float>() / 16.0f) {
                float pf = (float) plane / (float) PLANES;
                float adjusted_r = radius * std::cos(pf * glm::pi<float>());
                float plane_adjustment = radius * std::sin(pf * glm::pi<float>());

                float x = std::cos(angle) * adjusted_r;
                float z = std::sin(angle) * adjusted_r;
                glVertex3f(position.x + x, position.y + plane_adjustment, position.z + z);
            }
            glEnd();
        }


        restore();
    }

    void triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        glBegin(GL_TRIANGLES);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();

        restore();
    }

    void point(glm::vec3 point) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        glBegin(GL_POINTS);
        glVertex3f(point.x, point.y, point.z);
        glEnd();

        restore();
    }

    void line(glm::vec3 a, glm::vec3 b) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        glBegin(GL_LINES);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glEnd();

        restore();
    }

    void disable_depth_next() {
        glDisable(GL_DEPTH_TEST);
        depth_test_disabled = true;
    }

    glm::vec3 vec_into(const rp3d::Vector3 &v) {
        return glm::vec3(v.x, v.y, v.z);
    }

    rp3d::Vector3 vec_into(glm::vec3 v) {
        return rp3d::Vector3(v.x, v.y, v.z);
    }

    void geometry(GeometryFormat &g) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < g.header.num_indices; i += 3) {
            int ta = g.indices[i];
            int tb = g.indices[i + 1];
            int tc = g.indices[i + 2];

            auto a = glm::vec3(g.vertices[ta * 3], g.vertices[ta * 3 + 1], g.vertices[ta * 3 + 2]);
            auto b = glm::vec3(g.vertices[tb * 3], g.vertices[tb * 3 + 1], g.vertices[tb * 3 + 2]);
            auto c = glm::vec3(g.vertices[tc * 3], g.vertices[tc * 3 + 1], g.vertices[tc * 3 + 2]);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();

        restore();
    }

    void render_physics(rp3d::DebugRenderer *renderer) {
        prepare();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(proj_matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_matrix);

        glBegin(GL_TRIANGLES);
        for (auto t : renderer->getTriangles()) {
            float r = (float) t.color1 / 255.0f;
            float g = (float) t.color2 / 255.0f;
            float bc = (float) t.color3 / 255.0f;
            glColor3f(r, g, bc * 0.5f);
            glVertex3f(t.point1.x, t.point1.y, t.point1.z);
            glVertex3f(t.point2.x, t.point2.y, t.point2.z);
            glVertex3f(t.point3.x, t.point3.y, t.point3.z);
        }
        glEnd();

        glBegin(GL_LINES);
        for (auto t : renderer->getLines()) {
            float r = (float) t.color1 / 255.0f;
            float g = (float) t.color2 / 255.0f;
            glColor3f(r, g, 0.5f);
            glVertex3f(t.point1.x, t.point1.y, t.point1.z);
            glVertex3f(t.point2.x, t.point2.y, t.point2.z);
        }
        glEnd();

        restore();
    }

}