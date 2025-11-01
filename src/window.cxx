#include "window.hxx"
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace vec {
    Window::Window(vk::Offset2D position, vk::Extent2D size, const char* pTitle, const vk::Instance& rInstance) :rInstance(rInstance){
        glfwWindowHint(GLFW_POSITION_X, position.x);
        glfwWindowHint(GLFW_POSITION_Y, position.y);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        this->window = glfwCreateWindow(size.width, size.height, pTitle, NULL, NULL);
        if(!window){
            throw std::runtime_error("Failed to create window!!.");
        }

        VkSurfaceKHR cSurface = {};
        VkResult result = glfwCreateWindowSurface((VkInstance)rInstance, this->window, nullptr, &cSurface);
        if( result != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface!!.");
        }

        this->surface = (vk::SurfaceKHR)cSurface;

    }
    Window::~Window(){
        rInstance.destroySurfaceKHR(this->surface);
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    vk::Bool32 Window::closeWindow(){
        return (vk::Bool32)glfwWindowShouldClose(this->window);
    }
}
