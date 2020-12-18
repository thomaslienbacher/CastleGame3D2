//
// Created by Thomas Lienbacher on 15.12.2020.
//

#include "debugrenderer.hpp"
#include "shader.hpp"
#include <glm/gtx/common.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace debug {
    static float color[3] = {1.0, 1.0, 1.0};
    static GLint current_program = 0;
    static float *view_matrix = nullptr;
    static float *proj_matrix = nullptr;

    static void prepare() {
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        Shader::use(0);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(color[0], color[1], color[2]);
    }

    static void restore() {
        Shader::use(current_program);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
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

        glBegin(GL_LINE_LOOP);
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
}