//
// Created by Thomas Lienbacher on 05.12.2020.
//

#include "glalheaders.hpp"
#include "window.hpp"
#include <cstdio>
#include <iostream>
#include "utils.hpp"

namespace window {
    GLFWwindow *glfw_window = nullptr;
    static ALCdevice *al_device;
    static ALCcontext *al_context;
    static std::vector<std::function<void(int, int)>> on_resizes;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        for (const auto &f : on_resizes) {
            f(width, height);
        }
    }

#ifdef DEBUG_BUILD

    static void debug_msg_callback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
        const char *severity_str = nullptr;

        switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:
                severity_str = "LOW";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                severity_str = "MEDIUM";
                break;
            case GL_DEBUG_SEVERITY_HIGH:
                severity_str = "HIGH";
                break;
            default:
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                severity_str = "NONE";
                break;
        }

        const char *source_str = nullptr;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                source_str = "API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                source_str = "WINDOW_SYSTEM";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                source_str = "SHADER_COMPILER";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                source_str = "THIRD_PARTY";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                source_str = "APPLICATION";
                break;
            default:
            case GL_DEBUG_SOURCE_OTHER:
                source_str = "OTHER";
                break;
        }

        const char *type_str = nullptr;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                type_str = "ERROR";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                type_str = "DEPRECATED_BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                type_str = "UNDEFINDED_BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                type_str = "PORTABILITY";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                type_str = "PERFORMANCE";
                break;
            case GL_DEBUG_TYPE_MARKER:
                type_str = "MARKER";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                type_str = "PUSH_GROUP";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                type_str = "POP_GROUP";
                break;
            default:
            case GL_DEBUG_TYPE_OTHER:
                type_str = "OTHER";
                break;
        }

        printf("OpenGL Debug: %s %s, id %x, severity %s, message %s\n", source_str, type_str, id,
               severity_str, message);
    }

#endif

    void init() {
        //opengl and window init
        auto result = glfwInit();
        if (result != GLFW_TRUE) {
            const char *desc = nullptr;
            int code = glfwGetError(&desc);
            std::cerr << "glfwInit() failed: [" << code << "] " << desc << panic;
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
#if DEBUG_BUILD
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        //glfw_window = glfwCreateWindow(std::lround(videomode->width * 0.75), std::lround(videomode->height * 0.75),
        //                               "CastleGame3D2", nullptr, nullptr);

        //fullscreen:
        glfw_window = glfwCreateWindow(videomode->width, videomode->height,
                                       "CastleGame3D2", glfwGetPrimaryMonitor(), nullptr);

        if (!glfw_window) {
            const char *desc = nullptr;
            int code = glfwGetError(&desc);
            std::cerr << "Window creation failed with OpenGL context: [" << code << "] " << desc << panic;
        }

        glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback);
        glfwMakeContextCurrent(glfw_window);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glfwSwapInterval(1);

        glfwFocusWindow(glfw_window);
#ifdef RELEASE_BUILD
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

#ifdef DEBUG_BUILD
        if (glad_glDebugMessageCallback && glad_glDebugMessageControl) {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback((GLDEBUGPROC) debug_msg_callback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
#endif

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(2);
        glLineWidth(2);

        //openal init
        al_device = alcOpenDevice(nullptr);
        al_context = alcCreateContext(al_device, nullptr);
        alcMakeContextCurrent(al_context);
    }

    void close() {
        alcDestroyContext(al_context);
        alcCloseDevice(al_device);
        glfwTerminate();
    }

    float get_aspect_ratio() {
        int width, height;
        glfwGetFramebufferSize(glfw_window, &width, &height);
        return (float) width / (float) height;
    }

    void add_on_resize(const std::function<void(int, int)> &func) {
        on_resizes.emplace_back(func);
    }

    bool is_key_pressed(int key) {
        return glfwGetKey(glfw_window, key) & GLFW_PRESS;
    }

    void pause_audio() {
        alcDevicePauseSOFT(al_device);
    }

    void resume_audio() {
        alcDeviceResumeSOFT(al_device);
    }

    glm::vec2 size() {
        int width, height;
        glfwGetFramebufferSize(glfw_window, &width, &height);
        return glm::vec2((float) width, (float) height);
    }
}
