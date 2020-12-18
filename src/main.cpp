//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include <cmath>
#include <cstring>
#include "window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "debugrenderer.hpp"
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <dataformat/geometry.hpp>
#include <glm/gtx/quaternion.hpp>
#include <reactphysics3d/reactphysics3d.h>

void cam_controls(Camera &cam, float dt) {
    dt *= 1.8f;

    if (glfwGetKey(window::glfw_window, GLFW_KEY_A) & GLFW_PRESS) {
        cam.position.x -= dt;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_D) & GLFW_PRESS) {
        cam.position.x += dt;
    }

    if (glfwGetKey(window::glfw_window, GLFW_KEY_W) & GLFW_PRESS) {
        cam.position.z -= dt;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_S) & GLFW_PRESS) {
        cam.position.z += dt;
    }

    if (glfwGetKey(window::glfw_window, GLFW_KEY_F) & GLFW_PRESS) {
        cam.position.y -= dt;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_R) & GLFW_PRESS) {
        cam.position.y += dt;
    }

    if (glfwGetKey(window::glfw_window, GLFW_KEY_LEFT) & GLFW_PRESS) {
        cam.yaw -= dt * 45;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_RIGHT) & GLFW_PRESS) {
        cam.yaw += dt * 45;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_UP) & GLFW_PRESS) {
        cam.pitch += dt * 45;
    }
    if (glfwGetKey(window::glfw_window, GLFW_KEY_DOWN) & GLFW_PRESS) {
        cam.pitch -= dt * 45;
    }
}

int test_format() {
    std::ifstream in;
    in.open("data/slevel.obj", std::ifstream::in);
    GeometryFormat geo = GeometryFormat::import_from_obj(in);
    in.close();
    strcpy(geo.header.identifier, "slevel@obj");

    std::ofstream out;
    out.open("data/slevel.bin", std::ofstream::trunc | std::ofstream::binary | std::ofstream::out);
    geo.export_to_file(out);
    out.close();

    return 1;
}

int main() {
    if (test_format() == 71) {
        return 1;
    }

    auto physx = rp3d::PhysicsCommon();
    auto settings = rp3d::PhysicsWorld::WorldSettings();
    settings.gravity = {0.0f, -9.81f, 0.f};
    auto world = physx.createPhysicsWorld(settings);
    physx.destroyPhysicsWorld(world);

    window::init();

    double lastTime = 0;

    std::ifstream in;
    in.open("data/slevel.bin", std::ifstream::in | std::ifstream::binary);
    GeometryFormat g = GeometryFormat::import_from_file(in);
    in.close();
    Mesh m(g.header.num_indices, g.indices, g.header.num_vertices, g.vertices, g.texcoords, g.normals);

    auto t = Texture::from_file("data/slevel.png");

    auto vs = utils::file_to_string("data/common_vs.glsl");
    auto fs = utils::file_to_string("data/common_fs.glsl");
    Shader s(vs, fs);


    Camera cam;
    cam.position = glm::vec3(0, 0, 2);
    window::add_on_resize([&cam](int w, int h) {
        cam.projection = glm::perspective(glm::radians(90.0f), window::get_aspect_ratio(), 0.1f, 100.f);
    });

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(4);
    glLineWidth(4);

    std::ifstream in2;
    in2.open("data/monkey.bin", std::ifstream::in | std::ifstream::binary);
    GeometryFormat g2 = GeometryFormat::import_from_file(in2);
    in2.close();
    Mesh m2(g2.header.num_indices, g2.indices, g2.header.num_vertices, g2.vertices, g2.texcoords, g2.normals);
    auto t2 = Texture::from_file("data/monkey.png");


    while (!glfwWindowShouldClose(window::glfw_window)) {
        double now = glfwGetTime();
        double delta = now - lastTime;
        lastTime = now;

        glClearColor(std::clamp((std::sin(lastTime * 3.0) + 1.0) / 2.0, 0., 0.2), 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.use();
        s.set_uniform("u_proj", cam.get_proj_mat());
        s.set_uniform("u_view", cam.get_view_mat());
        m.bind();
        t.bind();

        cam_controls(cam, delta);

        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                for (int z = 0; z < 3; ++z) {
                    const float dis = 26.6f;
                    auto rot = glm::mat4(1.0f);
                    //rot = glm::rotate(rot, (float) std::sin(lastTime * 1.0) * glm::pi<float>(),glm::vec3(0.0, 1.0f, 0.0f));
                    //rot = glm::rotate(rot, (float) std::cos(lastTime * 11.1f) * 0.2f, glm::vec3(0.0, 0.0f, 1.0f));
                    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(-x * dis, y * dis, -z * dis));
                    s.set_uniform("u_model", model * rot);
                    glDrawElements(GL_TRIANGLES, m.get_num_elements(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }


        m2.bind();
        t2.bind();

        for (int x = 0; x < 7; ++x) {
            for (int y = 0; y < 7; ++y) {
                const float dis = 3.6f;
                auto model = glm::translate(glm::mat4(1.0f), glm::vec3(-x * dis, y * dis, 30.0));
                s.set_uniform("u_model", model);
                glDrawElements(GL_TRIANGLES, m2.get_num_elements(), GL_UNSIGNED_INT, nullptr);
            }
        }


        debug::set_cam(cam);
        debug::set_color(1.0f, 1.0f, 1.0f);
        debug::sphere(glm::vec3(0, 0, 0), 1.0f);

        debug::set_color(1.0f, 0.3, 0.0f);
        debug::point({0.0, 0.0});

        debug::set_color(1.0f, 0.3f, 1.0f);
        debug::triangle({3, 3, 0}, {3, 6, 1}, {4, 6, -1});


        debug::set_color(0.0f, 0.7f, 0.8f);
        for (int i = 0; i < g.header.num_indices; i += 3) {
            int ta = g.indices[i];
            int tb = g.indices[i + 1];
            int tc = g.indices[i + 2];

            auto a = glm::vec3(g.vertices[ta * 3], g.vertices[ta * 3 + 1], g.vertices[ta * 3 + 2]);
            auto b = glm::vec3(g.vertices[tb * 3], g.vertices[tb * 3 + 1], g.vertices[tb * 3 + 2]);
            auto c = glm::vec3(g.vertices[tc * 3], g.vertices[tc * 3 + 1], g.vertices[tc * 3 + 2]);
            debug::triangle(a, b, c);
        }

        debug::set_color(0.0f, 0.4f, 1.0f);
        for (int i = 0; i < g.header.num_vertices; i++) {
            auto a = glm::vec3(g.vertices[i * 3], g.vertices[i * 3 + 1], g.vertices[i * 3 + 2]);
            debug::point(a);
        }

        glfwSwapBuffers(window::glfw_window);
        glfwPollEvents();
    }

    window::close();
    return 0;
}
