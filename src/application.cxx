#include "window.hxx"
#include <application.hxx>
#include <device.hxx>
#include <GLFW/glfw3.h>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace vec{
    Application::Application(const char* pApplicationName, uint32_t applicationVersion, VkBool32 enableValidator){
        vk::ApplicationInfo appInfo = {pApplicationName, applicationVersion, pEngineName, engineVersion, apiVersion};
        createInstance(appInfo, enableValidator);
        createDebugMessenger();
    }

    void Application::createInstance(vk::ApplicationInfo appInfo ,VkBool32 enableValidator){
        if(!glfwInit()){
            throw std::runtime_error("Failed to init GLFW!!.");
        }
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions( glfwExtensions, glfwExtensions + glfwExtensionCount);
        if(enableValidator) extensions.push_back("VK_EXT_debug_utils");

        std::vector<const char*> layers = {};
        if(enableValidator) layers.push_back("VK_LAYER_KHRONOS_validation");

        vk::InstanceCreateInfo instanceInfo = {{}, &appInfo, layers, extensions, nullptr};

        vk::Result result = vk::createInstance(&instanceInfo, NULL, &instance);
        if(result != vk::Result::eSuccess){
            throw std::runtime_error("Failed to create instance!!.");
        }
    }
    void Application::createDebugMessenger(){

    }

    void Application::running(){
        Window* window = new Window({0,0},{800, 600}, "Ember Vulkan", this->instance);
        GPUDevice* device = new GPUDevice(this->instance, *window);

        while(!window->closeWindow()){
            glfwPollEvents();
        }

        delete window;
    }
    Application::~Application(){
        instance.destroy();
        glfwTerminate();

    }
}
