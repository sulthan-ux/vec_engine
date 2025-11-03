#pragma once
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>

namespace vec {
    class Window{
    public:
        Window(vk::Offset2D position, vk::Extent2D size, const char* pTitle, const vk::Instance& rInstance);
        ~Window();

        vk::Bool32 closeWindow();

        vk::SurfaceKHR getSurface() const { return this->surface;}
        GLFWwindow* getWindow() const { return this->window;}

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

    private:
        const vk::Instance& rInstance;
        GLFWwindow* window;
        vk::SurfaceKHR surface;

    };

}
