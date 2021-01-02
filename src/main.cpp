//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "glalheaders.hpp"
#include "window.hpp"
#include "game.hpp"

int main() {
    window::init();

    {
        Game game;

        double lastTime = 0;
        bool maximized = false;

        while (!glfwWindowShouldClose(window::glfw_window)) {
            double now = glfwGetTime();
            auto delta = (float) (now - lastTime);
            lastTime = now;

            if (window::is_key_pressed(GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(window::glfw_window, true);
            }

            if (window::is_key_pressed(GLFW_KEY_F) && !maximized) {
                glfwMaximizeWindow(window::glfw_window);
                maximized = true;
            }

            game.update(delta);

            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            game.render();

            glfwSwapBuffers(window::glfw_window);
            glfwPollEvents();
        }
    }

    window::close();
    return 0;
}
