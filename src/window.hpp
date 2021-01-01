//
// Created by Thomas Lienbacher on 05.12.2020.
//

#ifndef CASTLEGAME3D2_WINDOW_HPP
#define CASTLEGAME3D2_WINDOW_HPP

#include <functional>
#include "glalheaders.hpp"

namespace window {
    extern GLFWwindow *glfw_window;

    void init();

    float get_aspect_ratio();

    void close();

    void add_on_resize(const std::function<void(int, int)>& func);//TODO: remove hacky workaround

    bool is_key_pressed(int key);

    void pause_audio();

    void resume_audio();
}

#endif //CASTLEGAME3D2_WINDOW_HPP
