//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_WINDOW_HPP
#define CASTLEGAME3D2_WINDOW_HPP

#include <functional>
#include "master.hpp"

namespace window {
    extern GLFWwindow *glfw_window;

    void init();

    float get_aspect_ratio();

    void close();

    void add_on_resize(const std::function<void(int, int)>& func);//TODO: remove hacky workaround

    bool is_key_pressed(int key);
}

#endif //CASTLEGAME3D2_WINDOW_HPP
