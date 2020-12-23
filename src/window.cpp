//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "master.hpp"
#include "window.hpp"
#include <cstdio>

namespace window {
    GLFWwindow *glfw_window = nullptr;
    static std::vector<std::function<void(int, int)>> on_resizes;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        for (const auto& f : on_resizes) {
            f(width, height);
        }
    }

    static void debug_msg_callback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
        if (type == GL_DEBUG_TYPE_ERROR) {
            printf("OpenGL Debug: ERROR src 0x%x, type 0x%x, id 0x%x, severity 0x%x, message %s\n", source, type, id,
                   severity, message);
        } else {
            printf("OpenGL Debug: INFO src 0x%x, type 0x%x, id 0x%x, severity 0x%x, message %s\n", source, type, id,
                   severity, message);
        }
    }

    void init() {
        glfwInit();

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        glfw_window = glfwCreateWindow(1280, 720, "CastleGame3D2", nullptr, nullptr);

        glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
        glfwMakeContextCurrent(glfw_window);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glfwSwapInterval(1);

#ifdef DEBUG_BUILD
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback((GLDEBUGPROC) debug_msg_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(2);
        glLineWidth(2);
    }

    void close() {
        glfwTerminate();
    }

    float get_aspect_ratio() {
        int width, height;
        glfwGetFramebufferSize(glfw_window, &width, &height);
        return (float) width / (float) height;
    }

    void add_on_resize(const std::function<void(int, int)>& func) {
        on_resizes.emplace_back(func);
    }

    bool is_key_pressed(int key) {
        return glfwGetKey(glfw_window, key) & GLFW_PRESS;
    }
}
