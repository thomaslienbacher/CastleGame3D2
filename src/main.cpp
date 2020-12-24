//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "master.hpp"
#include "window.hpp"
#include "game.hpp"
#include "geometry.hpp"
#include "utils.hpp"
#include "debug.hpp"

int main() {
    window::init();
    Game game;

    double lastTime = 0;

    while (!glfwWindowShouldClose(window::glfw_window)) {
        double now = glfwGetTime();
        auto delta = (float) (now - lastTime);
        lastTime = now;

        if (window::is_key_pressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window::glfw_window, true);
        }

        game.update(delta);

        glClearColor(std::sin(lastTime * 4.0) * 0.2 + 0.5, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.render();

        glfwSwapBuffers(window::glfw_window);
        glfwPollEvents();
    }

    window::close();
    return 0;
}
